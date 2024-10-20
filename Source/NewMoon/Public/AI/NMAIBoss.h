// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "NMAIBoss.generated.h"

UCLASS()
class NEWMOON_API ANMAIBoss : public ACharacter
{
	GENERATED_BODY()

public:
	ANMAIBoss();
	void InitBoss();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Attack();
	virtual void MeleeAttack();
	virtual void LeftMagicAttack();
	virtual void RightMagicAttack();

	FORCEINLINE bool GetISAttack() const { return bIsAttack; }
	FORCEINLINE void SetIsAttack(bool NewBool) { bIsAttack = NewBool; }
	
protected:
	void SetDead();
	uint8 bIsAttack : 1;

// HP
public:
	FORCEINLINE float GetHP() const { return HP; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHPRatio() const { return HP / MaxHP; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetOldHPRatio() const { return OldHP / MaxHP; }

	void CalculateOldHP();

	UFUNCTION()
	void OldHPLerp(float value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float MaxHP;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float HP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float OldHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float SavedOldHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float AttackDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<UCurveFloat> OldHPCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<UTimelineComponent> HPLerpTimeline;
	
	FOnTimelineFloat OldHPCurveCallback;

protected:
	TObjectPtr<UParticleSystem> MeleeParticle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeadMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackRange = 300.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	TSubclassOf<AActor> LeftMagicProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	TSubclassOf<AActor> RightMagicProjectileClass;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ClearStage();

protected:
	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> MeleeAttackMontages;
	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> MagicAttackMontages;
	UPROPERTY()
	TObjectPtr<UAnimMontage> JumpAttackMontage;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SpawnDecal(float Duration);

	UFUNCTION(BlueprintCallable)
	void JumpAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector JumpAttackLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecalRadius = 400.0f;
};
