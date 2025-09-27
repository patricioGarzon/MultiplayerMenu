// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"
#include "Components/AudioComponent.h"
#include <Kismet/GameplayStatics.h>

void USoundManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    MusicComponent = nullptr;
}

// ---------------- Music ----------------
void USoundManager::PlayMenuMusic()
{
    if (!SoundsDA) return;

    if (!MusicComponent)
    {
        if (SoundsDA->MenuTracks.Num() > 0) {
            MenuSound = SoundsDA->MenuTracks[0];
            MusicComponent = UGameplayStatics::CreateSound2D(
                this,
                MenuSound,
                1.f,
                1.f,
                0.f,
                nullptr,
                true);
        }
    }

    if (!MusicComponent->IsPlaying())
    {
        MusicComponent->Play();
    }
}

void USoundManager::StopMenuMusic(float FadeOutTime)
{
}

void USoundManager::SetVolume(EVolumeType Type, float NewVolume)
{
    USoundClass* TargetClass = nullptr;

    switch (Type)
    {
    case EVolumeType::Master: TargetClass = MasterClass; break;
    case EVolumeType::Music:  TargetClass = MusicClass; break;
    case EVolumeType::SFX:    TargetClass = SFXClass; break;
    }

    if (TargetClass && MasterSoundMix)
    {
        UGameplayStatics::SetSoundMixClassOverride(
            this,
            MasterSoundMix,
            TargetClass,
            FMath::Clamp(NewVolume, 0.f, 1.f),
            1.f,    // pitch
            0.25f,  // fade time
            true
        );
        UGameplayStatics::PushSoundMixModifier(this, MasterSoundMix);
    }
}

void USoundManager::SetSoundData(UUSoundManagerData* Data)
{
    //set the data asset 
    SoundsDA = Data;
}
