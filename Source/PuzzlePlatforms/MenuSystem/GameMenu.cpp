// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"
#include "MenuInterface.h"

#include "Components/Button.h"

bool UGameMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess)
	{
		return false;
	}

	if (!IsValid(CancelButton))
	{
		return false;
	}
	CancelButton->OnClicked.AddDynamic(this, &UGameMenu::Teardown);

	if (!IsValid(QuitButton))
	{
		return false;
	}
	QuitButton->OnClicked.AddDynamic(this, &UGameMenu::ReturnToMainMenu);



	return true;
}

void UGameMenu::ReturnToMainMenu()
{
	if (MenuInterface == nullptr)
	{
		return;
	}

	Teardown();
	MenuInterface->ReturnToMenu();
}