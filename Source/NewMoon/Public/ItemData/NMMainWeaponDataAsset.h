// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/NMHeroData.h"
#include "ItemData/NMItemBaseDataAsset.h"
#include "NMMainWeaponDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FAdditionalWeapon
{
	GENERATED_BODY()

	FAdditionalWeapon()
	{
		
	}

	UPROPERTY(EditAnywhere, Category = Weapon)
	FName AdditionalWeaponSocket;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<UStaticMesh> AdditionalMesh;
};

UCLASS()
class NEWMOON_API UNMMainWeaponDataAsset : public UNMItemBaseDataAsset
{
	GENERATED_BODY()

public:
	UNMMainWeaponDataAsset()
	{
		ItemType = UNMAssetManager::MainWeaponItemType;
	}

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	FNMHeroStat WeaponStat;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FName WeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TArray<FAdditionalWeapon> AdditionalWeapons;
	
};
