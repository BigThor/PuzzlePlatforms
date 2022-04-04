// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMenu.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData {
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UBaseMenu
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

public:
	void SetServersList(TArray<FServerData> ServerDataList);
	void SetSelectedIndex(uint32 Index);

private:
	UFUNCTION()
	void HostServer();
	UFUNCTION()
	void JoinServer();
	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void ShowMainMenu();
	UFUNCTION()
	void ShowJoinMenu();

	void CreateScrollTextRow(const FServerData DataToUse, int32 Index);
	void UpdateSelectedRow();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ShowJoinMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServersScrollBox;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UScrollRow> ScrollRowWidgetClass;

	TOptional<uint32> SelectedIndex;
};
