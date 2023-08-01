// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "UdemyProject/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	FVector Up = GetActorUpVector();
	float length = 100.f;

	/*DRAW_SPHERE(Location, FColor::Cyan)
	DRAW_VECTOR(Location, Location + Forward * -100.f)
	DRAW_ARROW(Location, Location + Forward * 200.f) */

	DRAW_3D_GIZMO(Location, Forward, Right, Up, length);
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

