// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class UDEMY_API AWeapon : public AItem
{
    GENERATED_BODY()
public:
    AWeapon();
    void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
    void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
    void DropWeapon();
    void InitializeFromDataTable();
    void SetWeaponStanceTwoHanded(bool isTwoHanded);


    TArray<AActor*>ActorsToIgnore;
protected:
    virtual void BeginPlay() override;
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
    virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

    UFUNCTION()
    void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // No definition needed for BlueprintImplementableEvent
    UFUNCTION(BlueprintImplementableEvent)
    void CreateFields(const FVector& FieldLocation);
private:
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
    USoundBase* EquipSound;

    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
    UBoxComponent* WeaponBox;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceStart;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceEnd;

    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
    float Damage = 20.f;

    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
    float DamageModifierTwoHanded = 1.f;

    UPROPERTY(EditAnywhere, Category = "Data")
    UDataTable* WeaponDataTable;

    UPROPERTY(EditAnywhere, Category = "Data")
    FName WeaponID;

    bool bIsWeaponHeldTwoHanded = false;
    float CalculateDamage();


public:
    FORCEINLINE UBoxComponent * GetWeaponBox() const { return WeaponBox; }

};
