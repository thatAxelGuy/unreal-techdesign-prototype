// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarWidgetComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"


void UHealthBarWidgetComponent::ResetHealthUpdateTimer()
{
	// Clear any existing timer to reset delay
	GetWorld()->GetTimerManager().ClearTimer(DelayedHealthUpdateTimerHandle);

	// Set a new timer to start the smooth update after the delay
	GetWorld()->GetTimerManager().SetTimer(DelayedHealthUpdateTimerHandle, this, &UHealthBarWidgetComponent::SmoothUpdatedDamageBar, DelayTimer, false);
}

void UHealthBarWidgetComponent::SmoothUpdatedDamageBar()
{
	// Clear any existing smooth update timer to prevent overlap
	GetWorld()->GetTimerManager().ClearTimer(SmoothUpdateTimerHandle);

	// Start a timer to gradually reduce the damage bar to the current health value
	GetWorld()->GetTimerManager().SetTimer(SmoothUpdateTimerHandle, this, &UHealthBarWidgetComponent::UpdateDamageBar, GetWorld()->GetDeltaSeconds(), true);
}

void UHealthBarWidgetComponent::UpdateDamageBar()
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->DamageBar && HealthBarWidget->HealthBar)
	{
		float CurrentDamagePercent = HealthBarWidget->DamageBar->Percent;
		float TargetHealthPercent = HealthBarWidget->HealthBar->Percent;

		// Smoothly interpolate between DamageBar and HealthBar value
		float NewDamagePercent = FMath::FInterpTo(CurrentDamagePercent, TargetHealthPercent,GetWorld()->GetDeltaSeconds(), 5.0f);

		// Update the Damage Bar
		HealthBarWidget->DamageBar->SetPercent(NewDamagePercent);

		// Stop updating if the DamageBar has caught up to the health bar
		if (FMath::IsNearlyEqual(NewDamagePercent, TargetHealthPercent, 0.01f))
		{
			// Force to the final value to ensure they are equal
			HealthBarWidget->DamageBar->SetPercent(TargetHealthPercent);
			GetWorld()->GetTimerManager().ClearTimer(SmoothUpdateTimerHandle);
		}
	}
}

void UHealthBarWidgetComponent::SetHealthPercent(float Percent)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}

	// Reset the delay timer whenever health is updated
	ResetHealthUpdateTimer();
}

void UHealthBarWidgetComponent::SetDamagePercent(float Percent)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->DamageBar)
	{
		HealthBarWidget->DamageBar->SetPercent(Percent);
	}
}


