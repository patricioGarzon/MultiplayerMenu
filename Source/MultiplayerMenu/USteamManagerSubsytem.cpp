// Fill out your copyright notice in the Description page of Project Settings.

#include "USteamManagerSubsytem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemSteam.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include <Private/OnlineSubsystemSteamTypes.h>
#include "steam_api.h"

#include <isteamfriends.h>
#include <isteamuser.h>
#include <steam_api.h>
#include <Kismet/GameplayStatics.h>

void UUSteamManagerSubsytem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("SteamManagerSubsystem initialized"));

    OnlineSubsystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
    //We are initializing the subsystem and checking connection with steam
    if (OnlineSubsystem)
    {
        CacheSteamUserInfo();

        UE_LOG(LogTemp, Log, TEXT("Online Subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());

        SessionInterface = OnlineSubsystem->GetSessionInterface();
        IdentityInterface = OnlineSubsystem->GetIdentityInterface();
        if (IdentityInterface.IsValid())
        {
            IdentityInterface->AddOnLoginCompleteDelegate_Handle(
                0,
                FOnLoginCompleteDelegate::CreateUObject(this, &UUSteamManagerSubsytem::OnSteamLoginCompleted));

            // Always valid for Steam unless there's an issue
            if (IdentityInterface->GetLoginStatus(0) != ELoginStatus::LoggedIn)
            {
                //Login in trigger log in
                FOnlineAccountCredentials Credentials;
                IdentityInterface->Login(0, Credentials);
            }
            else {
                // You can immediately grab nickname and SteamID here

                    // pass the function manually //// TESTING
                TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);
                if (UserId.IsValid())
                {
                    OnSteamLoginCompleted(0, true, *UserId, TEXT(""));
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
    }
    GetOrLoadAvatar();
}

void UUSteamManagerSubsytem::Deinitialize()
{
    CachedPlayerAvatar = nullptr;
    Super::Deinitialize();
}

UTexture2D* UUSteamManagerSubsytem::GetOrLoadAvatar()
{
    //check if we already cached a player avater texture if not load it
    if (CachedPlayerAvatar) return CachedPlayerAvatar; 

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

    CachedPlayerAvatar = AvatarTexture;
    return AvatarTexture;
}

void UUSteamManagerSubsytem::CacheSteamSession(FString SessionName, FString SessionPassword, int MaxPlayers, bool JoinInProgress, bool ShouldAdvertise)
{
   /* ChSessionDetails.SessionName = SessionName;
    ChSessionDetails.SessionPassword = SessionPassword;
    ChSessionDetails.MaxPlayers = MaxPlayers;
    ChSessionDetails.JoinInProgress = true;
    ChSessionDetails.ShouldAdvertise = true;*/
}

void UUSteamManagerSubsytem::OnSteamLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
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
        FString Msg = FString::Printf(TEXT("Logged in as: %s)"), *UserId.ToString());        
        UE_LOG(LogTemp, Log, TEXT("Login complete: %s"), *UserId.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Log,TEXT("Login failed: $s"),*Error);

    }
}

void UUSteamManagerSubsytem::CacheSteamUserInfo(){
    if (OnlineSubsystem)
    {
        IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
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