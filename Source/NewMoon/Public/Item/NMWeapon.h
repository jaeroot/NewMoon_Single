// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/NMInteractionInterface.h"
#include "Item/NMItemBase.h"
#include "NMWeapon.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API ANMWeapon : public ANMItemBase, public INMInteractionInterface
{
	GENERATED_BODY()

public:
	ANMWeapon();

	virtual void Interaction(AActor* OtherActor) override;
};
