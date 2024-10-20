// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NMPlayerCharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
DECLARE_LOG_CATEGORY_EXTERN(LogNMCharacter, Log, All);

UCLASS()
class NEWMOON_API ANMPlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ANMPlayerCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Input Actions
protected:
	// Base Free Move Input Action Functions
	void FreeMove(const FInputActionValue& Value);
	void FreeLook(const FInputActionValue& Value);
	void JumpAction();

	
protected:
	// Camera
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	// Animation
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimInstance> AnimInstance;

	// // Linked Anim Layers
	// UPROPERTY(VisibleAnywhere)
	// TArray<TObjectPtr<UClass>> LinkedAnimLayers;
	
};
