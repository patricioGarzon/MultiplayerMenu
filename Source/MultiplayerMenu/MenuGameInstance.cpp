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
    OnlineSubsystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
    if (OnlineSubsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("Online Subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());

        SessionInterface = OnlineSubsystem->GetSessionInterface();
        IdentityInterface = OnlineSubsystem->GetIdentityInterface();
        if (IdentityInterface.IsValid())
        {
            // Always valid for Steam unless there's an issue
            if (IdentityInterface->GetLoginStatus(0) == ELoginStatus::LoggedIn)
            {               
                // You can immediately grab nickname and SteamID here
                CachedNickname = IdentityInterface->GetPlayerNickname(0);

                TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);
                if (UserId.IsValid())
                {
                    CachedSteamId = UserId->ToString();
                    // Now create the menu since Steam info is ready
                    FTimerHandle MenuDelayHandle;
                    GetWorld()->GetTimerManager().SetTimer(MenuDelayHandle, this, &UMenuGameInstance::CreateMainMenu, 1.0f, false);
                  
                }       
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Identity Interface is not valid!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No OnlineSubsystem found!"));
        //Create the menu when the PC is ready 
        FTimerHandle MenuDelayHandle;
        GetWorld()->GetTimerManager().SetTimer(MenuDelayHandle, this, &UMenuGameInstance::CreateMainMenu, 1.0f, false);
    }
}

void UMenuGameInstance::OnMapLoaded(UWorld* LoadedWorld)
{
}

void UMenuGameInstance::CheckForLobbyMap()
{
  
}

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
                UTexture2D* avatar = GetSteamAvatar();
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

UTexture2D* UMenuGameInstance::GetSteamAvatar()
{
    //Validate Steam friends available
    if (!SteamFriends()) {
        UE_LOG(LogTemp, Error, TEXT("SteamFriends not available"));
        return nullptr;
    }
    
    CSteamID SteamID = SteamUser()->GetSteamID();

    int Avatar = SteamFriends()->GetLargeFriendAvatar(SteamID);

    if (Avatar <= 0) {
        UE_LOG(LogTemp, Warning, TEXT("Steam Avatar not available yet"));
        //GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Green, "avatar not initialized yedt");
        return nullptr;
    }

    uint32 Width, Height;
    if (!SteamUtils()->GetImageSize(Avatar, &Width, &Height)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to get avatar size"));
        return nullptr;
    }

    TArray<uint8> RawData;
    RawData.SetNumUninitialized(Width * Height * 4);

    bool bSuccess = SteamUtils()->GetImageRGBA(Avatar, RawData.GetData(), RawData.Num());
    if (!bSuccess)
    {
        GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Green, "Avatar id recieved but no image");
        //UE_LOG(LogTemp, Error, TEXT("SteamUtils()->GetImageRGBA failed even though avatar ID is valid!"));
        return nullptr;
    }
    if (RawData.Num() > 0)
    {
        uint8 FirstPixelR = RawData[0];
        uint8 FirstPixelG = RawData[1];
        uint8 FirstPixelB = RawData[2];
        uint8 FirstPixelA = RawData[3]; 
    }

    if (!SteamUtils()->GetImageRGBA(Avatar, RawData.GetData(), RawData.Num())) {
        UE_LOG(LogTemp, Error, TEXT("Failed to get avatar image data"));
        return nullptr;
    }

    UTexture2D* AvatarTexture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
    if (!AvatarTexture) return nullptr;

    AvatarTexture->SRGB = true;
    AvatarTexture->Filter = TF_Bilinear;
    AvatarTexture->LODGroup = TEXTUREGROUP_UI;

    void* TextureData = AvatarTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
    AvatarTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
    AvatarTexture->UpdateResource();

    return AvatarTexture;
}

void UMenuGameInstance::CacheSession(FString SessionName, FString SessionPassword, int MaxPlayers, bool JoinInProgress, bool ShouldAdvertise)
{
    ChSessionDetails.SessionName = SessionName;
    ChSessionDetails.SessionPassword = SessionPassword;
    ChSessionDetails.MaxPlayers = MaxPlayers;
    ChSessionDetails.JoinInProgress = true;
    ChSessionDetails.ShouldAdvertise = true;
}

void UMenuGameInstance::OpenMainMeu()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuLevel"));
    FTimerHandle MenuDelayHandle;
    GetWorld()->GetTimerManager().SetTimer(MenuDelayHandle, this, &UMenuGameInstance::CreateMainMenu, 0.5f, false);
}

void UMenuGameInstance::OnSteamLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
#if USE_STEAM_AUTH_TICKET
    if (SteamUser()) {
        const int32 BufferSize = 1024;
        uint8 AuthBuffer[BufferSize];
        uint32 TicketSize = 0;

        HAuthTicket AuthTicketHandle = SteamUser()->GetAuthSessionTicket(AuthBuffer, BufferSize, &TicketSize);

        if (AuthTicketHandle != k_HAuthTicketInvalid && TicketSize > 0) {
            // Convert to hex string for server or just print
            FString TicketHex;
            for (uint32 i = 0; i < TicketSize; ++i) {
                TicketHex += FString::Printf(TEXT("%02x"), AuthBuffer[i]);
            }

            UE_LOG(LogTemp, Log, TEXT("Steam Auth Ticket: %s"), *TicketHex);
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Steam Ticket Generated"));
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Failed to get Steam Auth Ticket"));
        }
    }
#endif
    //check if succesful conected to steam services
    if (bWasSuccessful) {
        UE_LOG(LogTemp, Log, TEXT("Login complete: %s"), *UserId.ToString());
        CacheSteamUserInfo();
        Cast<UMainMenuWidget>(MenuUI)->PopulateSteamDetails();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Login failed: %s"), *Error);
    }


}

void UMenuGameInstance::PrintSteamInfo()
{
    UE_LOG(LogTemp, Log, TEXT("Steam Nickname: %s"), *CachedNickname);
    UE_LOG(LogTemp, Log, TEXT("Steam ID: %s"), *CachedSteamId);
}

void UMenuGameInstance::CreateSession()
{

    if (OnlineSubsystem)
    {
        IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
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
            IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
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

void UMenuGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful) {
        if (OnlineSubsystem)
        {
            IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
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



void UMenuGameInstance::CacheSteamUserInfo()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
    if (Subsystem)
    {
        IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
            if (UserId.IsValid())
            {

                CachedNickname = Identity->GetPlayerNickname(0);
                CachedSteamId = UserId->ToString();

            }
        }
    }
   

}
