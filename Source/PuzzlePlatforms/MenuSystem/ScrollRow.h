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
	
public:
	void SetText(const FText TextToSet);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerNameText;

};
