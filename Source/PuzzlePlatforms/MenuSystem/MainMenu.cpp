// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "ScrollRow.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"

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
	/*if (!IsValid(IPAddressField))
	{
		return;
	}*/

	//const FString IPAddress = IPAddressField->GetText().ToString();
	//MenuInterface->Join(IPAddress);
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

	if (!IsValid(ServersScrollBox))
	{
		return;
	}
	ServersScrollBox->ClearChildren();

	// Loop for testing scrollbox
	for (int i = 0; i < 20; i++)
	{
		FString string = FString::Printf(TEXT("Row %d"), i);
		FText text = FText::FromString(string);
		CreateScrollTextRow(text);
	}
}

void UMainMenu::CreateScrollTextRow(const FText TextToUse)
{
	if (ScrollRowWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Add a value to ScrollRowWidgetClass in MainMenu BP"));
		return;
	}
	if (!IsValid(ServersScrollBox))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find scroll box"));
		return;
	}

	UScrollRow* NewRow = CreateWidget<UScrollRow>(this, ScrollRowWidgetClass);
	if (!IsValid(NewRow))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not add row to scrollbox"));
		return;
	}
	NewRow->SetText(TextToUse);

	ServersScrollBox->AddChild(NewRow);
}