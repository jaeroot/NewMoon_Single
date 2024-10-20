// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/NMAICharacter.h"
#include "NMAIMagic.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API ANMAIMagic : public ANMAICharacter
{
	GENERATED_BODY()

public:
	ANMAIMagic();

public:
	virtual void Attack() override;

protected:
	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;
	
	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> WeaponAttackMontages;
};
