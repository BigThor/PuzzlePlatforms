// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMenu.h"

void UBaseMenu::Setup()
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

void UBaseMenu::Teardown()
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
	FInputModeGameOnly GameOnlyData;

	PlayerController->SetInputMode(GameOnlyData);
	PlayerController->bShowMouseCursor = false;

	this->RemoveFromViewport();
}

void UBaseMenu::SetMenuInterface(class IMenuInterface* MenuToSet)
{
	MenuInterface = MenuToSet;
}