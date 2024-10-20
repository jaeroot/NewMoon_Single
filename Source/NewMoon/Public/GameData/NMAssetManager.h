// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "NMAssetManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogNMAssetManager, Error, All);

UCLASS()
class NEWMOON_API UNMAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UNMAssetManager() {}
	static UNMAssetManager& Get();
	
public:
	static const FPrimaryAssetType PickupItemType;
	static const FPrimaryAssetType AutoPickupItemType;
	static const FPrimaryAssetType MainWeaponItemType;
	static const FPrimaryAssetType SubWeaponItemType;
	
};
