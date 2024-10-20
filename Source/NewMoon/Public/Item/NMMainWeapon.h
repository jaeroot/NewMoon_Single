// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NMWeapon.h"
#include "NMMainWeapon.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API ANMMainWeapon : public ANMWeapon
{
	GENERATED_BODY()

public:
	ANMMainWeapon();

public:
	virtual void Attack();
	virtual void SpecialSkill();
	virtual void NormalSkill1();
	virtual void NormalSkill2();
	virtual void UltimateSkill();

protected:
	// Skills
	
};
