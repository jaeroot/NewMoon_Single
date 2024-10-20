// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameData/NMAssetManager.h"
#include "NMItemBaseDataAsset.generated.h"

class ANMItemBase;
/**
 * 
 */
UCLASS()
class NEWMOON_API UNMItemBaseDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UNMItemBaseDataAsset()
		: MaxCount(0), PurchasePrice(0), SellingPrice(0)
	{
		
	}
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 MaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 PurchasePrice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 SellingPrice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FSlateBrush ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	TSubclassOf<ANMItemBase> ItemClass;
	
};
