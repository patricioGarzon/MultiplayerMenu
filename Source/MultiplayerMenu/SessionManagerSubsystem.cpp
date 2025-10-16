// Fill out your copyright notice in the Description page of Project Settings.
#include "SessionManagerSubsystem.h"

#include "CustromPlayerController.h"
#include "MenuGameInstance.h"
#include "USteamManagerSubsytem.h"

#include "OnlineSubsystem.h"                 // Access the online subsystem
#include "OnlineSessionSettings.h"           // FOnlineSessionSettings, session queries
#include "Interfaces/OnlineSessionInterface.h" // IOnlineSession, FOnlineSessionSearch
#include "OnlineSubsystemSteam.h"
#include <Online/OnlineSessionNames.h>
#include <Kismet/GameplayStatics.h>
#include "SoundManager.h"

void USessionManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (GetGameInstance()) {
        SteamManager = GetGameInstance()->GetSubsystem<UUSteamManagerSubsytem>();
    }
   
}

void USessionManagerSubsystem::Deinitialize()
{
}


void USessionManagerSubsystem::FindSessions()
{
    SteamManager = GetGameInstance()->GetSubsystem<UUSteamManagerSubsytem>();
    if (SteamManager && SteamManager->GetSteamOnlineSubs()) {
        IOnlineSessionPtr Sessions = SteamManager->GetSteamOnlineSubs()->GetSessionInterface();
        if (Sessions.IsValid()) {
            //here search for sessions
            SessionSearch = MakeShareable(new FOnlineSessionSearch());
            
            SessionSearch->MaxSearchResults = 10; // Set max results
            SessionSearch->bIsLanQuery = bIsLan; // Search over the internet (Steam)
            SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // Only find sessions with presence enabled
            // Add other search parameters if needed (e.g., game mode, custom properties)

             // Bind completion delegate
            Sessions->AddOnFindSessionsCompleteDelegate_Handle(
                FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionManagerSubsystem::OnFindSessionsComplete)
            );

            // Start searching
            Sessions->FindSessions(0, SessionSearch.ToSharedRef());
        }
    }
}

void USessionManagerSubsystem::AttemptJoinSession(FOnlineSessionSearchResult& inSession)
{
    IOnlineSessionPtr Sessions = SteamManager->GetSteamOnlineSubs()->GetSessionInterface();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    if (Sessions.IsValid()) {
        //Sessions->JoinSession()
        FString SessionNameStr;
        if (inSession.Session.SessionSettings.Get(FName("SESSION_NAME"), SessionNameStr))
        {
            // Convert to FName when needed
            FName SessionName(*SessionNameStr);
            Sessions->JoinSession(*PC->GetLocalPlayer()->GetPreferredUniqueNetId(), SessionName, inSession);
        }
        else {
            //Show text ensure session is correct
        }    
    }
}

void USessionManagerSubsystem::CreateSession() {
    if (SteamManager && SteamManager->GetSteamOnlineSubs())
    {
        IOnlineSessionPtr Sessions = SteamManager->GetSteamOnlineSubs()->GetSessionInterface();
        if (Sessions.IsValid())
        {

            // Register the delegate with the session interface
            OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(
                FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionManagerSubsystem::OnCreateSessionComplete)
            );
            //
            FOnlineSessionSettings SessionSettings;
            SessionSettings.NumPublicConnections = ChSessionDetails.MaxPlayers; // Set max players
            SessionSettings.bAllowJoinInProgress = ChSessionDetails.JoinInProgress;
            SessionSettings.bShouldAdvertise = ChSessionDetails.ShouldAdvertise;
            SessionSettings.bUsesPresence = true;
            SessionSettings.bUseLobbiesIfAvailable = true;

            if (!ChSessionDetails.SessionPassword.IsEmpty()) {
                SessionSettings.Set(FName(ChSessionDetails.SessionPassword), FString(ChSessionDetails.SessionPassword), EOnlineDataAdvertisementType::ViaOnlineService);
            }
            CSteamID SteamID = SteamUser()->GetSteamID();
            IOnlineIdentityPtr Identity = SteamManager->GetSteamOnlineSubs()->GetIdentityInterface();
            if (Identity.IsValid())
            {
                TSharedPtr<const FUniqueNetId> UniqueId = Identity->GetUniquePlayerId(0);
                if (UniqueId.IsValid())
                {
                    // ✅ Now pass a reference to CreateSession
                    bool bCreated = Sessions->CreateSession(*UniqueId, FName(*ChSessionDetails.SessionName), SessionSettings);
                    if (!bCreated)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CreateSession failed to start"));
                    }
                    else {
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Session Created"));
                    }
                }
                else
                {
                    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, "Unique ID not created");
                }
            }
        }
    }
    else {
        //Prepare to create local, offline game
        UGameplayStatics::OpenLevel(this, "InitialTown");
        USoundManager* SoundMG = GetGameInstance()->GetSubsystem<USoundManager>();
        if (SoundMG) {
            SoundMG->StopPlayingSound();
        }
    }
}

void USessionManagerSubsystem::CreateDummySessions()
{
    for (int i = 0; i < 5; ++i)
    {
        FOnlineSessionSearchResult DummyResult;
        DummyResult.Session.SessionSettings.NumPublicConnections = 4;
        DummyResult.Session.OwningUserName = FString::Printf(TEXT("Host : %d"), i + 1);
        // Add to SessionSearch->SearchResults manually
        DummyResult.Session.SessionSettings.SessionIdOverride = "SSDFS:" + i;
        SessionSearch->SearchResults.Add(DummyResult);
    }
}

void USessionManagerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
    if (bWasSuccessful) {
        if (SteamManager->GetSteamOnlineSubs())
        {
            IOnlineSessionPtr Sessions = SteamManager->GetSteamOnlineSubs()->GetSessionInterface();
            if (Sessions.IsValid())
            {
                if (GetWorld() && GetWorld()->GetNetMode() != NM_Client)
                {
                    // Host = local controller on listen server
                    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
                    if (ACustromPlayerController* HostPC = Cast<ACustromPlayerController>(PC))
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, "Player calling server travel");
                        HostPC->Server_TravelToLobby();
                    }
                    // Perform ServerTravel with seamless travel disabled
                    //GetWorld()->ServerTravel("/Game/Menu/LobbyMenuLevel?listen", false); // false disables seamless travel
                }
            }
        }
    }
    else {
        GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, "Session not created");
    }
}

void USessionManagerSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    if(bWasSuccessful){
        
        CreateDummySessions();
        UE_LOG(LogTemp, Log, TEXT("Found %d sessions."), SessionSearch->SearchResults.Num());
        OnSessionsFound.Broadcast(SessionSearch->SearchResults);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No sessions found or search failed."));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Found 0sessions, or error"));
    }
}
