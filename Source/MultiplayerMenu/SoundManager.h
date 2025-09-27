// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "USoundManagerData.h"
#include "SoundManager.generated.h"


class USoundMix;

UENUM(BlueprintType)
enum class EVolumeType : uint8
{
	Master UMETA(DisplayName = "Master"),
	Music  UMETA(DisplayName = "Music"),
	SFX    UMETA(DisplayName = "SFX")
};
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API USoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY()
	UAudioComponent* MusicComponent;

	void PlayMenuMusic();

	void StopMenuMusic(float FadeOutTime);

	UFUNCTION(BlueprintCallable)
	void SetVolume(EVolumeType Type, float NewVolume);

	UPROPERTY(EditAnywhere)
	USoundBase* MenuSound = nullptr;

	UPROPERTY()
	UUSoundManagerData* SoundsDA;

	UFUNCTION()
	void SetSoundData(UUSoundManagerData* Data);

protected:
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundMix* MasterSoundMix;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundClass* MasterClass;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundClass* MusicClass;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundClass* SFXClass;
};
