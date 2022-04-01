// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollRow.h"
#include "Components/TextBlock.h"

void UScrollRow::SetText(const FText TextToSet)
{
	if (!IsValid(ServerNameText))
	{
		return;
	}

	ServerNameText->SetText(TextToSet);
}