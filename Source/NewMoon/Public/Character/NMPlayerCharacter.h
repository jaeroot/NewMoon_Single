// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NMPlayerCharacterBase.h"
#include "Components/TimelineComponent.h"
#include "Interface/NMCharacterInterface.h"
#include "NMPlayerCharacter.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class INMInteractionInterface;
class UNMInventoryComponent;
/**
 * 
 */
UCLASS()
class NEWMOON_API ANMPlayerCharacter : public ANMPlayerCharacterBase, public INMCharacterInterface
{
	GENERATED_BODY()

public:
	ANMPlayerCharacter();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	virtual void BeginPlay() override;

	void SetDead();
	void ResurrectInit();

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Input Actions
protected:
	// Menu Input Action Functions
	void ToggleInventoryMenu();
	void ToggleSettingMenu();
	
	// Combat Input Action Functions
	void SprintStart();
	void SprintStop();
	void Dash();
	void AttackAction();
	void SpecialSkill();
	void NormalSkill1();
	void NormalSkill2();
	void UltimateSkill();
	void ToggleLockOn();
	void Resurrect();
	void NoEquip();
	void Interaction();

public:
	void Attack();
	void AttackEnd();

	void AttackCombo();
	
private:
	TSet<TObjectPtr<AActor>> HitResultSet;

// Child Mesh
public:
	FORCEINLINE USkeletalMeshComponent* GetChildMesh() const { return ChildMesh; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ChildMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNMInventoryComponent> InventoryComponent;

// Interaction
protected:
	UPROPERTY(VisibleAnywhere)
	uint8 bInteract : 1;

	INMInteractionInterface* FocusedInteractionActor;

	void TraceForward();

	UFUNCTION(Server, Reliable)
	void ServerInteraction();

	virtual void InteractItem(class ANMItemBase* Item) override;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float MaxHP;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float HP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float OldHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float SavedOldHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float AttackDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<UCurveFloat> OldHPCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<UTimelineComponent> HPLerpTimeline;
	
	FOnTimelineFloat OldHPCurveCallback;

	uint8 bIsDead : 1;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UStaticMeshComponent> Sword;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UStaticMeshComponent> Shield;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UParticleSystemComponent> ParticleSystem;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> LinkAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> HitMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> KickMontage;

protected:
	TObjectPtr<UParticleSystem> HitImpact;

	TObjectPtr<UNiagaraSystem> LineFXSystem;
	TObjectPtr<UNiagaraComponent> LineFXComponent;

	UPROPERTY()
	TSubclassOf<UCameraShakeBase> CameraShakeBase;

public:
	void PlayHitMontage(int32 MontageNum);
	
	void ActivateLineFX();

	void StartCameraShake();

protected:
	
	int32 AttackNum = 0;
	int32 KickNum = 0;

	uint8 bCombo : 1;
	uint8 bKickCombo : 1;
	
protected:
	uint8 bIsGuard : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> GuardMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> GuardHitMontage;

	UFUNCTION()
	void GuardEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCanKick : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCanGuard : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCanFireBall : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ResurrectLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator ResurrectRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsBossLevel : 1;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> FireballClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> FireBallMontage;

public:
	void FireBallAttack();

	void HitKnockBack();

public:
	
};
