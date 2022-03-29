// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

	if (MenuBPClass.Class == nullptr)
	{
		return;
	}

	MenuClass = MenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	if (IsValid(MenuClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
	}
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!IsValid(MenuClass))
	{
		return;
	}
	UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuClass);

	if (!IsValid(Menu))
	{
		return;
	}

	Menu->AddToViewport();
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