// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class AWeapon;

UCLASS()
class UDEMY_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> SlashContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* EKeyAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	/** 
	 * Callbacks for Input
	*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void Dodge();
	void Attack();

	/**
	 * Play Montage Functions 
	 */

	void PlayAttackMontage();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();

	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();

	bool CanArm();
	
	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();


	//Hair Groom Assets

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGroomComponent* Eyebrows;
	
	//Skeletal Hair Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalHairMesh;

private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	/**
	 * Animation Montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item){OverlappingItem = Item;}
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;}
	
};
