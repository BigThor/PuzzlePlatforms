// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UBaseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Setup();
	UFUNCTION()
	virtual void Teardown();
	virtual void SetMenuInterface(class IMenuInterface* MenuToSet);


protected:
	class IMenuInterface* MenuInterface;
};
