// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("Trigger Volume"));
	if (TriggerVolume)
	{
		RootComponent = TriggerVolume;
	}

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Trigger Mesh"));
	TriggerMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (TriggerVolume)
	{
		TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnBeginOverlap);
		TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnEndOverlap);
	}
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Activated"));

	for (AMovingPlatform* CurrentPlatform : MovingPlatformsArray)
	{
		CurrentPlatform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Deactivated"));


	for (AMovingPlatform* CurrentPlatform : MovingPlatformsArray)
	{
		CurrentPlatform->RemoveActiveTrigger();
	}
}

