// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData/NMItemBaseDataAsset.h"
#include "NMSubWeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API UNMSubWeaponDataAsset : public UNMItemBaseDataAsset
{
	GENERATED_BODY()

public:
	UNMSubWeaponDataAsset()
	{
		ItemType = UNMAssetManager::SubWeaponItemType;
	}
	
};
