// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerMenuCharacter.h"
#include "MultiplayerMenuProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "MultiplayerMenuWeaponComponent.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMultiplayerMenuCharacter

AMultiplayerMenuCharacter::AMultiplayerMenuCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the camera boom (spring arm)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 300.0f;             // Distance to the character
	CameraBoom->bUsePawnControlRotation = true;       // Rotate the arm based on the controller

	// Create the follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach to boom
	FollowCamera->bUsePawnControlRotation = false;     // Camera does NOT rotate relative to arm

}

//////////////////////////////////////////////////////////////////////////// Input

void AMultiplayerMenuCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMultiplayerMenuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMultiplayerMenuCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMultiplayerMenuCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AMultiplayerMenuCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMultiplayerMenuCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		//rotate the mesh
		float Pitch = FMath::ClampAngle(LookAxisVector.Y, -90.0f, 90.0f);

	}
}

void AMultiplayerMenuCharacter::EquipWeaponBySlot(EWeaponSlot Slot)
{
	UMultiplayerMenuWeaponComponent* Temp = nullptr;
	switch (Slot)
	{
	case EWeaponSlot::Primary:
		Temp = PrimaryWeapon;
		break;
	case EWeaponSlot::Secondary:
		Temp = SecondaryWeapon;
		break;
	case EWeaponSlot::Throwable:
		Temp = ThrowableWeapon;
		break;
	}
	if (Temp)
	{
		EquipWeapon(Temp);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon assigned for this slot!"));
	}
}

void AMultiplayerMenuCharacter::EquipWeapon(UMultiplayerMenuWeaponComponent* WeaponToEquip)
{

}
