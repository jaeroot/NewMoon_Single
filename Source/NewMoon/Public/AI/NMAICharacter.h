// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "NMAICharacter.generated.h"

class UNMWidgetComponent;

UCLASS()
class NEWMOON_API ANMAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANMAICharacter();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Attack();
	virtual void MeleeAttack();
	virtual void MagicAttack();

	FORCEINLINE bool GetISAttack() const { return bIsAttack; }
	FORCEINLINE void SetIsAttack(bool NewBool) { bIsAttack = NewBool; }
	
protected:
	void SetDead();
	uint8 bIsAttack : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> ShieldMesh;

	
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

	FORCEINLINE bool GetIsBoss() const { return bIsBoss; }
	
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsBoss : 1;


protected:
	TObjectPtr<UParticleSystem> MeleeParticle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<UNMWidgetComponent> InfoWidget;

public:
	FORCEINLINE FName GetAIName() const { return AIName; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AIName;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeadMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> WeaponDeadMontage;

public:
	void HitKnockBack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackRange = 150.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	TSubclassOf<AActor> MagicProjectileClass;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ClearStage();
};
