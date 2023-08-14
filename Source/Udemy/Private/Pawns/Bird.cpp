// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"


ABird::ABird() 
{
	 PrimaryActorTick.bCanEverTick = true;

	 Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	 Capsule->SetCapsuleHalfHeight(20.f);
	 Capsule->SetCapsuleRadius(15.f);
	 SetRootComponent(Capsule);

	 BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	 BirdMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	 BirdMesh->SetRelativeRotation(RootComponent->GetRelativeRotation());

	 AutoPossessPlayer = EAutoReceiveInput::Player0;
	
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if(!BirdMappingContext.IsNull())
			{
				Subsystem->AddMappingContext(BirdMappingContext.LoadSynchronous(), 0);
			}

		}
	} 
	
}
void ABird::Move(const FInputActionValue &Value)
{
	const float DirectionalValue = Value.Get<float>();

	if(Controller && (DirectionalValue != 0.f))
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionalValue);
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
	}

}

