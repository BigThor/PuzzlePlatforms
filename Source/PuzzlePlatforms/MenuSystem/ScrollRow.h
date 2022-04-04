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
	void SetServerNameText(const FText TextToSet);
	void SetUserNameText(const FText TextToSet);
	void SetPlayerCountText(const FText TextToSet); 

	void Setup(class UMainMenu* MainMenuWidget, int32 RowIndex);
	int32 GetIndex();

private:
	UFUNCTION()
	void UpdateMainMenuIndex();

public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsSelected = false;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* RowButton;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerNameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UserNameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerCountText;


	UPROPERTY()
	class UMainMenu* MainMenu;
	int32 Index = -1;

};
