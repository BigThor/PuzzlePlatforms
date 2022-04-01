// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();
	UFUNCTION(BlueprintCallable)
	void LoadGameMenu();

	UFUNCTION(Exec)
	void Host() override;
	UFUNCTION(Exec)
	void Join(const FString& IPAdress) override;
	void ReturnToMenu() override;

private:
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	void OnFindSessionsComplete(bool bSuccess);

private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	TSubclassOf<class UUserWidget> GameMenuClass;

	IOnlineSessionPtr SessionInterfacePtr;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
