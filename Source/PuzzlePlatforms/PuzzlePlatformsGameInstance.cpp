// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/GameMenu.h"
#include "UObject/ConstructorHelpers.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

	if (MainMenuBPClass.Class == nullptr)
	{
		return;
	}

	MainMenuClass = MainMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT("/Game/MenuSystem/WBP_GameMenu"));

	if (GameMenuBPClass.Class == nullptr)
	{
		return;
	}

	GameMenuClass = GameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	if (!IsValid(MainMenuClass))
	{
		return;
	}
	UMainMenu* MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);

	if (!IsValid(MainMenu))
	{
		return;
	}

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}


void UPuzzlePlatformsGameInstance::LoadGameMenu()
{
	if (!IsValid(GameMenuClass))
	{
		return;
	}
	UGameMenu* GameMenu = CreateWidget<UGameMenu>(this, GameMenuClass);

	if (!IsValid(GameMenu))
	{
		return;
	}

	GameMenu->Setup();
	GameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!IsValid(Engine))
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
	if (!IsValid(Engine))
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

void UPuzzlePlatformsGameInstance::ReturnToMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenuLevel", ETravelType::TRAVEL_Absolute);
}