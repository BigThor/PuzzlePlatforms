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

void UMainMenu::SetServersList(TArray<FServerData> ServerDataList)
{
	if (!IsValid(ServersScrollBox))
	{
		return;
	}
	ServersScrollBox->ClearChildren();

	// Loop for testing scrollbox
	int32 CurrentIndex = 0;
	for (const FServerData& ServerData : ServerDataList)
	{
		CreateScrollTextRow(ServerData, CurrentIndex);
		++CurrentIndex;
	}
}

void UMainMenu::SetSelectedIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateSelectedRow();
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

	if (!SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("There was no index selected"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Selected index is %d"), SelectedIndex.GetValue());
	MenuInterface->Join(SelectedIndex.GetValue());
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

	if (MenuInterface == nullptr)
	{
		return;
	}
	MenuInterface->FindAvailableSession();
}

void UMainMenu::CreateScrollTextRow(const FServerData DataToUse, int32 Index)
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
	NewRow->Setup(this, Index);
	NewRow->SetServerNameText(FText::FromString(DataToUse.Name));
	NewRow->SetUserNameText(FText::FromString(DataToUse.HostUsername));
	FString PlayerCount;
	PlayerCount.AppendInt(DataToUse.CurrentPlayers);
	PlayerCount.AppendChar('/');
	PlayerCount.AppendInt(DataToUse.MaxPlayers);
	NewRow->SetPlayerCountText(FText::FromString(PlayerCount));

	ServersScrollBox->AddChild(NewRow);
}

void UMainMenu::UpdateSelectedRow()
{
	if (!SelectedIndex.IsSet())
	{
		return;
	}

	for (UWidget* Child : ServersScrollBox->GetAllChildren())
	{
		UScrollRow* Row = Cast<UScrollRow>(Child);
		if (!IsValid(Row))
		{
			continue;
		}
		Row->bIsSelected = Row->GetIndex() == SelectedIndex;
	}
}