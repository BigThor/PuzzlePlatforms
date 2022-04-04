// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/GameMenu.h"

const static FName SERVER_NAME_SETTING_KEY = TEXT("ServerName");

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

void UPuzzlePlatformsGameInstance::Host(FName ServerName)
{
	if (!SessionInterfacePtr.IsValid())
	{
		return;
	}

	FNamedOnlineSession* PreviousSession = SessionInterfacePtr->GetNamedSession(ServerName);
	if (PreviousSession != nullptr)
	{
		SessionInterfacePtr->DestroySession(ServerName);
	}
	else
	{
		CreateSession(ServerName);
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
	FString ServerName;
	SelectedResult.Session.SessionSettings.Get(SERVER_NAME_SETTING_KEY, ServerName);
	//UE_LOG(LogTemp, Warning, TEXT("Trying to connect to Server %s"), *ServerName);
	SessionInterfacePtr->JoinSession(0, FName(ServerName) , SelectedResult);
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
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterfacePtr->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Find Sessions started"));
	}
}

void UPuzzlePlatformsGameInstance::CreateSession(FName ServerName)
{
	if (!SessionInterfacePtr.IsValid())
	{
		return;
	}

	FOnlineSessionSettings SessionSettings;
	if (IOnlineSubsystem::Get() != nullptr && IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
	}
	SessionSettings.NumPublicConnections = 3;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.Set(SERVER_NAME_SETTING_KEY, ServerName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterfacePtr->CreateSession(0, ServerName, SessionSettings);
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
		CreateSession(SessionName);
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

	if (SessionSearch->SearchResults.Num() > -1)
	{
		TArray<FServerData> ServerDataResults;
		/*SessionNameResults.Add("Test server 1");
		SessionNameResults.Add("Test server 2");
		SessionNameResults.Add("Test server 3");*/
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			FServerData Data;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTING_KEY, ServerName))
			{
				UE_LOG(LogTemp, Warning, TEXT("Data found in settings"));
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = SearchResult.GetSessionIdStr();
			}
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;

			ServerDataResults.Add(Data);
		}

		if (!IsValid(MainMenu))
		{
			UE_LOG(LogTemp, Warning, TEXT("Main Menu Widget does not exist"));
			return;
		}
		MainMenu->SetServersList(ServerDataResults);
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