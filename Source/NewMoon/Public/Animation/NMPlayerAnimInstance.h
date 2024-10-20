// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NMPlayerAnimInstance.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class NEWMOON_API UNMPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UNMPlayerAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<ACharacter> CharacterOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float Direction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float MoveThreshold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsInAir : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float JumpThreshold;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDead : 1;
	
protected:
	UFUNCTION(BlueprintCallable)
	void AnimNotify_Dead();

	UFUNCTION()
	void AnimNotify_LeftHandMagic();
	
	UFUNCTION()
	void AnimNotify_RightHandMagic();
};
