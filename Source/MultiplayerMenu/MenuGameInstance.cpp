// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "MultiplayerMenuGameMode.h"
#include "GameFramework/Actor.h"
#include <Private/OnlineSubsystemSteamTypes.h>
#include "CustromPlayerController.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"
#include <Kismet/GameplayStatics.h>
#include "MySavedGame.h"
#include "SettingsSubsystem.h"
#include "USteamManagerSubsytem.h"
#include "SoundManager.h"



void UMenuGameInstance::Init()
{
    Super::Init();
    
    // Get OnlineSubsystem (e.g., Steam)
    USettingsSubsystem* SettingsSubsystem = GetSubsystem<USettingsSubsystem>();
    if (SettingsSubsystem)
    {
       GameSavedSettings = SettingsSubsystem->LoadGameSettings();
    }
    
    USoundManager* SM = GetSubsystem<USoundManager>();
    if (SM)
    {
        SM->SetSoundData(SoundData); 
    }
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMenuGameInstance::OnMapLoaded);

    //get the steam subsytem manager and load it 
    SteamManager = GetSubsystem<UUSteamManagerSubsytem>();
    FTimerHandle MenuDelayHandle;
    GetWorld()->GetTimerManager().SetTimer(MenuDelayHandle, this, &UMenuGameInstance::CreateMainMenu, 1.0f, false);
}

void UMenuGameInstance::OnMapLoaded(UWorld* LoadedWorld)
{
}

void UMenuGameInstance::CheckForLobbyMap()
{
  
}

//UI HANDLING SPAWN MAIN MENU LOAD MAIN MENU
void UMenuGameInstance::CreateMainMenu()
{
    if (MainMenuWidgetClass) {
        MenuUI = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass, "Menu HUD");
        if (MenuUI) {
            MenuUI->AddToViewport(100);
            if (MenuUI->MediaPlayer && MenuUI->MediaSource) {
                if (MenuUI->MediaPlayer->OpenSource(MenuUI->MediaSource)) {
                    MenuUI->MediaPlayer->Play();
                }
            }
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                UTexture2D* avatar = SteamManager->GetOrLoadAvatar();
                if (avatar && MenuUI) {
                    Cast<UMainMenuWidget>( MenuUI)->steamUserAvatar = avatar;
                    Cast<UMainMenuWidget>(MenuUI)->SteamAvatar->SetBrushFromTexture(avatar);
                }
                PC->SetInputMode(FInputModeGameAndUI());
                PC->bShowMouseCursor = true;
                if (USoundManager* SM = GetSubsystem<USoundManager>())
                {
                    SM->PlayMenuMusic();
                    
                }
            }        
        }
    }
}

void UMenuGameInstance::OpenMainMeu()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuLevel"));
    FTimerHandle MenuDelayHandle;
    GetWorld()->GetTimerManager().SetTimer(MenuDelayHandle, this, &UMenuGameInstance::CreateMainMenu, 0.5f, false);
}

// SINGLE PLAYER FUNCTIONS TO CREATE LOAD AND SAVE GAMES
void UMenuGameInstance::CreateGame()
{
    //Initial game created

    UMySavedGame* SavedGameInstance = Cast<UMySavedGame>(UGameplayStatics::CreateSaveGameObject(UMySavedGame::StaticClass()));
    
    SavedGameInstance->MapName = "Initial Town";
    SavedGameInstance->Progression = 0;
    SavedGameInstance->TimePlayed = 0;
    
    UGameplayStatics::SaveGameToSlot(SavedGameInstance, SavedGameInstance->MapName, 0);
}

TArray<FString> UMenuGameInstance::LoadSavedGames()
{
    TArray<FString> SaveSlotNames;

    //Get the directory for the fiels
    FString SaveDir = FPaths::ProjectSavedDir() + "SaveGames/";

    IFileManager& FileManager = IFileManager::Get();

    // Get all .sav files
    TArray<FString> FoundFiles;
    FileManager.FindFiles(FoundFiles, *SaveDir, TEXT("*.sav"));

    // Remove file extension and store just slot names
    for (const FString& File : FoundFiles)
    {
        FString SlotName = FPaths::GetBaseFilename(File);
        SaveSlotNames.Add(SlotName);
    }
    return SaveSlotNames;
}

void UMenuGameInstance::SaveGame(FString FileName)
{
    UMySavedGame* SavedGameInstance = Cast<UMySavedGame>(UGameplayStatics::CreateSaveGameObject(UMySavedGame::StaticClass()));
    //All variables to be changed in game
    SavedGameInstance->MapName = "Begining Town";
    SavedGameInstance->Progression = 0.1f;
    SavedGameInstance->TimePlayed = 0;// Get local variable for time;
    CreateSession();

    if (FileName.IsEmpty()) {
        FileName = "BeginingTown";
    }
    UGameplayStatics::SaveGameToSlot(SavedGameInstance, FileName, 0);
}

// FROM THIS DOWN IS MULTIPLAYER SESSION HANDLING 
void UMenuGameInstance::CreateSession()
{

    if (SteamManager->GetSteamOnlineSubs())
    {
        IOnlineSessionPtr Sessions = SteamManager->GetSteamOnlineSubs()->GetSessionInterface();
        if (Sessions.IsValid())
        {

            // Register the delegate with the session interface
            OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(
                FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMenuGameInstance::OnCreateSessionComplete)
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
        UGameplayStatics::OpenLevel(this, "LobbyMenuLevel");
    }
}

void UMenuGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
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
