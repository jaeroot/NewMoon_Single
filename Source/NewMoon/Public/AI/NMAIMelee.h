// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/NMAICharacter.h"
#include "NMAIMelee.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API ANMAIMelee : public ANMAICharacter
{
	GENERATED_BODY()

public:
	ANMAIMelee();

public:
	virtual void Attack() override;

	
protected:
	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;
	
	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> WeaponAttackMontages;
	
};
