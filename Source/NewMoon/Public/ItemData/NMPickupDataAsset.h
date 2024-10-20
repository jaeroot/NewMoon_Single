// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData/NMItemBaseDataAsset.h"
#include "NMPickupDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API UNMPickupDataAsset : public UNMItemBaseDataAsset
{
	GENERATED_BODY()

public:
	UNMPickupDataAsset()
	{
		ItemType = UNMAssetManager::PickupItemType;
	}
	
};
