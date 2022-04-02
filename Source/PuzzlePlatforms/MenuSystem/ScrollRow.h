// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UScrollRow : public UUserWidget
{
	GENERATED_BODY()

protected:
	bool Initialize() override;
	
public:
	void SetText(const FText TextToSet);
	void Setup(class UMainMenu* MainMenuWidget, int32 RowIndex);

private:
	UFUNCTION()
	void UpdateMainMenuIndex();

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerNameText;

	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UPROPERTY()
	class UMainMenu* MainMenu;
	int32 Index = -1;

};
