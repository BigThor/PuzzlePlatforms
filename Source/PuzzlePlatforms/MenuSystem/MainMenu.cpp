// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Engine/Engine.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess)
	{
		return false;
	}

	if (!IsValid(HostButton))
	{
		return false;
	}
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::Host);

	return true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	if (!IsValid(InWorld))
	{
		return;
	}
	APlayerController* PlayerController = InWorld->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}
	FInputModeGameOnly GameOnlyData;

	PlayerController->SetInputMode(GameOnlyData);
	PlayerController->bShowMouseCursor = false;

	RemoveFromViewport();
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuToSet)
{
	MenuInterface = MenuToSet;
}


void UMainMenu::Setup()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}

	AddToViewport();
	bIsFocusable = true;

	FInputModeUIOnly UIModeData;
	UIModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UIModeData.SetWidgetToFocus(TakeWidget());

	PlayerController->SetInputMode(UIModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::Host()
{
	if (MenuInterface == nullptr)
	{
		return;
	}

	MenuInterface->Host();
	UE_LOG(LogTemp, Warning, TEXT("I am hosting a game!"));
}