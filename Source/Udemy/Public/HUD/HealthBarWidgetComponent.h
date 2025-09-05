// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class UDEMY_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);
	void SetDamagePercent(float Percent);

protected:
	UPROPERTY(EditAnywhere, Category = Timers)
	float DelayTimer = 3.f;
	UPROPERTY(EditAnywhere, Category = Timers)
	float InterpolationSpeed = 5.f;

private:
	UPROPERTY()
	class UHealthBar* HealthBarWidget;	

	FTimerHandle DelayedHealthUpdateTimerHandle;
	FTimerHandle SmoothUpdateTimerHandle;

	void SmoothUpdatedDamageBar();
	void UpdateDamageBar();
	void ResetHealthUpdateTimer();

};
