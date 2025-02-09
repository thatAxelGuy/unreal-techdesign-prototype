// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Items/Weapons/Weapon.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	// Weapon Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponName;

	// Base Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage;

	// 2H Damage Modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TwoHandedDamageModifier;


	//// Strength Scaling Grade
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FString StrengthGrade;

	//// Agility Scaling Grade
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FString AgilityGrade;
};
