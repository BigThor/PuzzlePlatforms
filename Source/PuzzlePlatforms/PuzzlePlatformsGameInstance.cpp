// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/GameMenu.h"

const static FName SESSION_NAME = TEXT("My game session");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

	if (MainMenuBPClass.Class == nullptr)
	{
		return;
	}

	MainMenuClass = MainMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT("/Game/MenuSystem/WBP_GameMenu"));

	if (GameMenuBPClass.Class == nullptr)
	{
		return;
	}

	GameMenuClass = GameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("The online subsystem is: %s"), *OnlineSubsystem->GetSubsystemName().ToString());

	SessionInterfacePtr = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterfacePtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Session pointer is not Valid"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Setting up delegates"));
	SessionInterfacePtr->OnCreateSessionCompleteDelegates.AddUObject(
		this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
	SessionInterfacePtr->OnDestroySessionCompleteDelegates.AddUObject(
		this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
	SessionInterfacePtr->OnFindSessionsCompleteDelegates.AddUObject(
		this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
	SessionInterfacePtr->OnJoinSessionCompleteDelegates.AddUObject(
		this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	if (!IsValid(MainMenuClass))
	{
		return;
	}
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);

	if (!IsValid(MainMenu))
	{
		return;
	}

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadGameMenu()
{
	if (!IsValid(GameMenuClass))
	{
		return;
	}
	UGameMenu* GameMenu = CreateWidget<UGameMenu>(this, GameMenuClass);

	if (!IsValid(GameMenu))
	{
		return;
	}

	GameMenu->Setup();
	GameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (!SessionInterfacePtr.IsValid())
	{
		return;
	}

	FNamedOnlineSession* PreviousSession = SessionInterfacePtr->GetNamedSession(SESSION_NAME);
	if (PreviousSession != nullptr)
	{
		SessionInterfacePtr->DestroySession(SESSION_NAME);
	}
	else
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterfacePtr.IsValid())
	{
		return;
	}
	if (!SessionSearch.IsValid())
	{
		return;
	}
	FOnlineSessionSearchResult SelectedResult = SessionSearch->SearchResults[Index];
	SessionInterfacePtr->JoinSession(0, SESSION_NAME, SelectedResult);

	/*UEngine* Engine = GetEngine();
	if (!IsValid(Engine))
	{
		return;
	}
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining adress %s"), *IPAdress));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->ClientTravel(IPAdress, ETravelType::TRAVEL_Absolute);*/
}

void UPuzzlePlatformsGameInstance::ReturnToMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenuLevel", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::FindAvailableSession()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionInterfacePtr->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Find Sessions started"));
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (!SessionInterfacePtr.IsValid())
	{
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	SessionInterfacePtr->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	if (bSuccess)
	{
		UEngine* Engine = GetEngine();
		if (!IsValid(Engine))
		{
			return;
		}

		Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("Hosting"));

		UWorld* World = GetWorld();

		if (!IsValid(World))
		{
			return;
		}

		World->ServerTravel("/Game/PuzzlePlatforms/Maps/ThirdPersonExampleMap?listen");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create a session"));
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
	if (bSuccess)
	{
		CreateSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not destroy the previous session"));
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	UE_LOG(LogTemp, Warning,TEXT("Find Sessions completed"));

	if (!bSuccess && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Search went wrong!"));
		return;
	}

	if (SessionSearch->SearchResults.Num() > 0)
	{
		TArray<FString> SessionNameResults;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			SessionNameResults.Add(SearchResult.Session.GetSessionIdStr());
		}

		if (!IsValid(MainMenu))
		{
			UE_LOG(LogTemp, Warning, TEXT("Main Menu Widget does not exist"));
			return;
		}
		MainMenu->SetServersList(SessionNameResults);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Sessions Were Found"));
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterfacePtr.IsValid())
	{
		return;
	}

	FString ConnectInformation;
	switch (Result)
	{
		case EOnJoinSessionCompleteResult::Success:
			if (SessionInterfacePtr->GetResolvedConnectString(SessionName, ConnectInformation) == true)
			{
				APlayerController* PlayerController = GetFirstLocalPlayerController();
				if (!IsValid(PlayerController))
				{
					break;
				}
				PlayerController->ClientTravel(ConnectInformation, ETravelType::TRAVEL_Absolute);
			}

			break;
		case EOnJoinSessionCompleteResult::SessionIsFull:
			break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist:
			break;
		case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
			break;
		case EOnJoinSessionCompleteResult::AlreadyInSession:
			break;
		case EOnJoinSessionCompleteResult::UnknownError:
			break;
		default:
			break;
	}
}