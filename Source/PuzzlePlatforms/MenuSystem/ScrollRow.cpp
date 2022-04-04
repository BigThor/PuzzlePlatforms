// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollRow.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "MainMenu.h"

bool UScrollRow::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess)
	{
		return false;
	}

	if (!IsValid(RowButton))
	{
		return false;
	}
	RowButton->OnClicked.AddDynamic(this, &UScrollRow::UpdateMainMenuIndex);

	return true;
}

void UScrollRow::SetServerNameText(const FText TextToSet)
{
	if (!IsValid(ServerNameText))
	{
		return;
	}

	ServerNameText->SetText(TextToSet);
}

void UScrollRow::SetUserNameText(const FText TextToSet)
{
	if (!IsValid(UserNameText))
	{
		return;
	}

	UserNameText->SetText(TextToSet);
}

void UScrollRow::SetPlayerCountText(const FText TextToSet)
{
	if (!IsValid(PlayerCountText))
	{
		return;
	}

	PlayerCountText->SetText(TextToSet);
}

void UScrollRow::Setup(UMainMenu* MainMenuWidget, int32 RowIndex)
{
	MainMenu = MainMenuWidget;
	Index = RowIndex;
}

void UScrollRow::UpdateMainMenuIndex()
{
	if (!IsValid(MainMenu) && Index > -1)
	{
		return;
	}
	MainMenu->SetSelectedIndex(Index);
}

int32 UScrollRow::GetIndex()
{
	return Index;
}