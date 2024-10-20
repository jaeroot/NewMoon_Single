// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData/NMItemBaseDataAsset.h"
#include "NMInventoryComponent.generated.h"

class UNMItemBaseDataAsset;

USTRUCT(BlueprintType)
struct FNMInventoryItem
{
	GENERATED_BODY()

public:
	FNMInventoryItem()
		: ItemCount(0)
	{
		
	}

	FORCEINLINE void Clear() { ItemData = nullptr;	ItemCount = 0; }

	FORCEINLINE UNMItemBaseDataAsset* GetItemData() const { return ItemData; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	FORCEINLINE FName GetItemName() const { return ItemData->ItemName; }

	FORCEINLINE void SetItemData(UNMItemBaseDataAsset* NewItemData) { ItemData = NewItemData; }
	int32 AddItem(int32 NewItemCount)
	{
		if (NewItemCount <= ItemData->MaxCount - ItemCount)
		{
			ItemCount += NewItemCount;

			return 0;
		}

	
		int32 EmptyCount = ItemData->MaxCount - ItemCount;
		ItemCount += EmptyCount;
		NewItemCount -= EmptyCount;

	
		return NewItemCount;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	TObjectPtr<UNMItemBaseDataAsset> ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 ItemCount;
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWMOON_API UNMInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNMInventoryComponent();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE const TArray<FNMInventoryItem>& GetInventory() const { return Inventory; }

	/**
	 * 
	 * @param NewItemData Item Data
	 * @param NewItemCount Item Count
	 * @return Remaining Item Count
	 * If return 0, Successfully add All Item
	 */
	int32 AddInventoryItem(UNMItemBaseDataAsset* NewItemData, int32 NewItemCount);
	void RemoveInventoryITem(int32 SlotNum);

protected:
	UPROPERTY(Replicated = OnRep_Inventory, VisibleAnywhere, Category = Inventory)
	TArray<FNMInventoryItem> Inventory;

	const int32 InventoryMaxCount = 20;

	UFUNCTION()
	void OnRep_Inventory();
	
};
