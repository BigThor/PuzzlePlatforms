// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;
	if (NumberOfPlayers > 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("There's 3 or more players in the session"));
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
}