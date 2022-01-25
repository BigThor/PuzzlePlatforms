// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;


private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Movement", Meta = (MakeEditWidget = true))
	FVector TargetLocation;
	FVector StartLocation;
	FVector EndLocation;
	FVector MoveDirection;
	float PathDistance;
	
};
