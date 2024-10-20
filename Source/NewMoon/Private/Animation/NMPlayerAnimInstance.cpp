// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NMPlayerAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "AI/NMAIBoss.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UNMPlayerAnimInstance::UNMPlayerAnimInstance()
{
	MoveThreshold = 5.0f;
	JumpThreshold = 100.0f;
	
	bIsIdle = true;
	bIsInAir = false;
	bIsJumping = false;
	bIsDead = false;
}

void UNMPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CharacterOwner = Cast<ACharacter>(GetOwningActor());
	if (IsValid(CharacterOwner))
	{
		MovementComponent = CharacterOwner->GetCharacterMovement();
	}
}

void UNMPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(MovementComponent))
	{
		Velocity = MovementComponent->Velocity;
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, CharacterOwner->GetActorRotation());
		Speed = Velocity.Size2D();
		bIsIdle = Speed < MoveThreshold;
		bIsInAir = MovementComponent->IsFalling();
		bIsJumping = bIsInAir & (Velocity.Z > JumpThreshold);
	}
	
}

void UNMPlayerAnimInstance::AnimNotify_Dead()
{
	GetOwningActor()->Destroy();
}

void UNMPlayerAnimInstance::AnimNotify_LeftHandMagic()
{
	ANMAIBoss* AIBossCharacter = Cast<ANMAIBoss>(GetOwningActor());
	if (IsValid(AIBossCharacter))
	{
		AIBossCharacter->LeftMagicAttack();
	}
}

void UNMPlayerAnimInstance::AnimNotify_RightHandMagic()
{
	ANMAIBoss* AIBossCharacter = Cast<ANMAIBoss>(GetOwningActor());
	if (IsValid(AIBossCharacter))
	{
		AIBossCharacter->RightMagicAttack();
	}
}
