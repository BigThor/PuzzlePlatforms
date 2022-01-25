// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
	SetReplicateMovement(true);

	StartLocation = GetActorLocation();
	EndLocation = StartLocation + TargetLocation;
	MoveDirection = TargetLocation.GetSafeNormal();
	PathDistance = TargetLocation.Size();
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		AddActorWorldOffset(MoveDirection * Speed * DeltaSeconds);

		float DistanceTraveled = (GetActorLocation() - StartLocation).Size();
		if (PathDistance < DistanceTraveled)
		{
			FVector TemporalStartLocation = StartLocation;
			StartLocation = EndLocation;
			EndLocation = TemporalStartLocation;
			MoveDirection = (EndLocation - StartLocation).GetSafeNormal();
		}
	}
}