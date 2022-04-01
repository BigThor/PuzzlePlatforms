// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

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
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);


	if (!IsValid(JoinButton))
	{
		return false;
	}
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!IsValid(ShowJoinMenuButton))
	{
		return false;
	}
	ShowJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::ShowJoinMenu);


	if (!IsValid(CancelButton))
	{
		return false;
	}
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::ShowMainMenu);

	if (!IsValid(QuitGameButton))
	{
		return false;
	}
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	Teardown();
}

void UMainMenu::HostServer()
{
	if (MenuInterface == nullptr)
	{
		return;
	}

	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (MenuInterface == nullptr)
	{
		return;
	}
	if (!IsValid(IPAddressField))
	{
		return;
	}

	const FString IPAddress = IPAddressField->GetText().ToString();
	MenuInterface->Join(IPAddress);
	UE_LOG(LogTemp, Warning, TEXT("I am hosting a game!"));
}

void UMainMenu::QuitGame()
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

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::ShowMainMenu()
{
	if (!IsValid(MenuSwitcher))
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::ShowJoinMenu()
{
	if (!IsValid(MenuSwitcher))
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(JoinMenu);
}