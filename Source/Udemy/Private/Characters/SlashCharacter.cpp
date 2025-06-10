// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/SlashCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	SkeletalHairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Asset"));
	SkeletalHairMesh->SetupAttachment(GetMesh());
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("SlashCharacter"));

	if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (!SlashContext.IsNull())
			{
				Subsystem->AddMappingContext(SlashContext.LoadSynchronous(), 0);
			}
		}
	}
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ASlashCharacter::Dodge);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASlashCharacter::Sprint);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(AttackHorizontalAction, ETriggerEvent::Triggered, this, &ASlashCharacter::AttackHorizontal);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::HeavyAttack);
	}
}

void ASlashCharacter::Move(const FInputActionValue &Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// find which way is forward
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASlashCharacter::Look(const FInputActionValue &Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASlashCharacter::EKeyPressed()
{
	AWeapon *OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		OverlappingItem = nullptr;
		// Drop weapon if one is already equipped
		if (EquippedWeapon){
			EquippedWeapon->DropWeapon();
		}
		
		EquippedWeapon = OverlappingWeapon;
		// Set character state based on the stance
		SetCharacterStateForEquippedWeapon();
	}
	else
	{
		if(CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			ActionState = EActionState::EAS_EquippingWeapon;

			// Update Character State to Unequipped
			CharacterState = ECharacterState::ECS_Unequipped;
			
		}
		else if(CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			ActionState = EActionState::EAS_EquippingWeapon;

			// Set character state for the currently equipped weapon
			SetCharacterStateForEquippedWeapon();
			
		}
	}
}

void ASlashCharacter::Dodge() { ToggleTwoHandedStance(); }

void ASlashCharacter::Sprint()
{
	if (CharacterState == ECharacterState::ECS_Unequipped || ActionState != EActionState::EAS_Unoccupied)  return;
	if(EquippedWeapon)
	{
		EquippedWeapon->DropWeapon();
		EquippedWeapon = nullptr;

	}
	
	SetCharacterStateForEquippedWeapon();
	
}

void ASlashCharacter::Attack()
{
	const int32 AttackType = 0;
	if (CanAttack())
	{
		PlayAttackMontage(AttackType);
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::AttackHorizontal()
{
	const int32 AttackType = 1;
	if (CanAttack())
	{
		PlayAttackMontage(AttackType);
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::HeavyAttack()
{
	const int32 AttackType = 2;
	if (CanAttack())
	{
		PlayAttackMontage(AttackType);
		ActionState = EActionState::EAS_Attacking;
	}
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		   CharacterState != ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::Die()
{
}


bool ASlashCharacter::CanDisarm() 
{
    return ActionState == EActionState::EAS_Unoccupied &&
		   CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
    return ActionState == EActionState::EAS_Unoccupied &&
	CharacterState == ECharacterState::ECS_Unequipped &&
	EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::Arm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}
void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::ToggleTwoHandedStance()
{
	if (CharacterState == ECharacterState::ECS_Unequipped) return;

	// Toggle between one-handed and two-handed stance
	bIsTwoHandedStance = !bIsTwoHandedStance;

	EquippedWeapon->SetWeaponStanceTwoHanded(bIsTwoHandedStance);

	// Update the character state for the equipped weapon
	SetCharacterStateForEquippedWeapon();

	UE_LOG(LogTemp, Log, TEXT("Toggled stance to %s"),
		bIsTwoHandedStance ? TEXT("Two-Handed") : TEXT("One-Handed"));
}

void ASlashCharacter::SetCharacterStateForEquippedWeapon()
{

	if (!EquippedWeapon)
	{
		CharacterState = ECharacterState::ECS_Unequipped;
		return;
	}

	// Update character state based on the current stance
	if (bIsTwoHandedStance)
	{
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
		UE_LOG(LogTemp, Log, TEXT("Equipped weapon in Two-Handed Stance."));
	}
	else
	{
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		UE_LOG(LogTemp, Log, TEXT("Equipped weapon in One-Handed Stance."));
	}
}

void ASlashCharacter::PlayAttackMontage(int32 AttackTypeValue)
{
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		//const int32 Selection = FMath::RandRange(0, 1);
		const int32 Selection = bIsTwoHandedStance ? 3 : AttackTypeValue;
		FName SectionName = FName();
		
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		case 3:
			SectionName = FName("Attack4");
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Default Case reached in AttackMontage Selection"));
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName);
	}

}

void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
