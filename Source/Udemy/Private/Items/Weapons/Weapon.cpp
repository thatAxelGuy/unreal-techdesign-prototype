// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"
#include "Data/WeaponData.h"

AWeapon::AWeapon()
{
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
    WeaponBox->SetupAttachment(GetRootComponent());
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
    BoxTraceStart->SetupAttachment(GetRootComponent());
    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    AdjustHeightAboveGround();
}

void AWeapon::AdjustHeightAboveGround()
{
    FVector Start = GetActorLocation();
    FVector End = Start - FVector((0, 0, 10000)); // Downward Trace

    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
    {
        FVector NewLocation = GetActorLocation();
        NewLocation.Z = HitResult.ImpactPoint.Z + HeightAboveGround;
        SetActorLocation(NewLocation);
    }
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
    InitializeFromDataTable();

    AdjustHeightAboveGround();
}

void AWeapon::Equip(USceneComponent *InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
    SetOwner(NewOwner);
    SetInstigator(NewInstigator);
    AttachMeshToSocket(InParent, InSocketName);
    ItemState = EItemState::EIS_Equipped;
    if(EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            EquipSound,
            GetActorLocation()
        );
    }

    if(Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (EmbersEffect)
    {
        EmbersEffect->Deactivate();
    }
}

void AWeapon::DropWeapon()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    SetOwner(nullptr);
    SetInstigator(nullptr);
    ItemState = EItemState::EIS_Hovering;

    SetActorRotation(FRotator::ZeroRotator);

    if (Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    if (EmbersEffect)
    {
        EmbersEffect->Activate();
    }

    AdjustHeightAboveGround();
}

void AWeapon::InitializeFromDataTable()
{
    if (!WeaponDataTable) return;

    static const FString ContextString(TEXT("Weapon Data"));
    FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponID, ContextString);
    UE_LOG(LogTemp, Log, TEXT("Weapon Data Accessed"));
    if (WeaponData)
    {
        Damage = WeaponData->BaseDamage;
        DamageModifierTwoHanded = WeaponData->TwoHandedDamageModifier;
        /*StrengthScaling = ScalingGrades[WeaponData->StrengthGrade];
        AgilityScaling = ScalingGrades[WeaponData->AgilityGrade];*/

        UE_LOG(LogTemp, Log, TEXT("Weapon Initialized: %s"), *WeaponData->WeaponName.ToString());
    }
}

void AWeapon::SetWeaponStanceTwoHanded(bool isTwoHanded)
{
    bIsWeaponHeldTwoHanded = isTwoHanded;
}

void AWeapon::AttachMeshToSocket(USceneComponent *InParent, const FName &InSocketName)
{
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}
void AWeapon::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(this);
    FHitResult BoxHit;

    for (AActor* Actor : ActorsToIgnore)
    {
        IgnoredActors.AddUnique(Actor);
    }
    UKismetSystemLibrary::BoxTraceSingle(
        this,
        Start,
        End,
        FVector(5.f, 5.f, 5.f),
        BoxTraceStart->GetComponentRotation(),
        ETraceTypeQuery::TraceTypeQuery1,
        false,
        IgnoredActors,
        EDrawDebugTrace::None,
        BoxHit,
        true
    );

    if(BoxHit.GetActor())
    {
        UGameplayStatics::ApplyDamage(
            BoxHit.GetActor(),
            CalculateDamage(),
            GetInstigator()->GetController(),
            this,
            UDamageType::StaticClass()
        );

        IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
        if(HitInterface)
        {
            HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
        }
        ActorsToIgnore.AddUnique(BoxHit.GetActor());

        CreateFields(BoxHit.ImpactPoint);

       
    }
    
}

float AWeapon::CalculateDamage()
{
    if (bIsWeaponHeldTwoHanded)
    {
        return Damage * DamageModifierTwoHanded;
    }
    else 
    {
        return Damage;
    }
    
}
