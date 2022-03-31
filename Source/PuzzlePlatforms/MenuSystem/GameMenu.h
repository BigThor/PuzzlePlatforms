// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMenu.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UGameMenu : public UBaseMenu
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize();

private:
	UFUNCTION()
	void ReturnToMainMenu();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
};
