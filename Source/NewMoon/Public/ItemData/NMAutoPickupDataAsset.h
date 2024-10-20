// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/NMHeroData.h"
#include "ItemData/NMItemBaseDataAsset.h"
#include "NMAutoPickupDataAsset.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	EPT_Temporary		UMETA(DisplayName = "Temporary Stat"),
	EPT_Permanent		UMETA(DisplayName = "Permanent Stat"),
	EPT_Replenish		UMETA(DisplayName = "Replenish"),

	EPT_MAX				UMETA(Hidden)
};

UCLASS()
class NEWMOON_API UNMAutoPickupDataAsset : public UNMItemBaseDataAsset
{
	GENERATED_BODY()

public:
	UNMAutoPickupDataAsset()
		: PickupType(EPickupType::EPT_Temporary), Duration(0.0f), ReplenishHP(0.0f), ReplenishStamina(0.0f)
	{
		ItemType = UNMAssetManager::AutoPickupItemType;
	}

public:
	UPROPERTY(EditAnywhere, Category = Pickup)
	EPickupType PickupType;

	UPROPERTY(EditAnywhere, Category = BuffStat, Meta = (EditCondition = "PickupType == EPickupType::EPT_Temporary"))
	float Duration;
	
	UPROPERTY(EditAnywhere, Category = BuffStat, Meta = (EditCondition = "PickupType == EPickupType::EPT_Temporary"))
	FNMHeroStat BuffStat;
	
	UPROPERTY(EditAnywhere, Category = AdditionalStat, Meta = (EditCondition = "PickupType == EPickupType::EPT_Permanent"))
	FNMHeroStat AdditionalStat;
	
	UPROPERTY(EditAnywhere, Category = ReplenishStat, Meta = (EditCondition = "PickupType == EPickupType::EPT_Replenish"))
	float ReplenishHP;
	
	UPROPERTY(EditAnywhere, Category = ReplenishStat, Meta = (EditCondition = "PickupType == EPickupType::EPT_Replenish"))
	float ReplenishStamina;
	
};
