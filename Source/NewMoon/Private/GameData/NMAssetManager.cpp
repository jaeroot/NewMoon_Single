// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/NMAssetManager.h"

DEFINE_LOG_CATEGORY(LogNMAssetManager);

const FPrimaryAssetType UNMAssetManager::PickupItemType = TEXT("Pickup");
const FPrimaryAssetType UNMAssetManager::AutoPickupItemType = TEXT("AutoPickup");
const FPrimaryAssetType UNMAssetManager::MainWeaponItemType = TEXT("MainWeapon");
const FPrimaryAssetType UNMAssetManager::SubWeaponItemType = TEXT("SubWeapon");

UNMAssetManager& UNMAssetManager::Get()
{
	UNMAssetManager* AssetManager = CastChecked<UNMAssetManager>(GEngine->AssetManager);
	if (::IsValid(AssetManager))
	{
		return *AssetManager;
	}

	UE_LOG(LogNMAssetManager, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UNMAssetManager>();
}
