// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{

}

void UPuzzlePlatformsGameInstance::Init()
{

}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!Engine)
	{
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		return;
	}

	World->ServerTravel("/Game/PuzzlePlatforms/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& IPAdress)
{
	UEngine* Engine = GetEngine();
	if (!Engine)
	{
		return;
	}
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining adress %s"), *IPAdress));

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->ClientTravel(IPAdress, ETravelType::TRAVEL_Absolute);
}