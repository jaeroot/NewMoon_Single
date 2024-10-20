// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/NMInventoryComponent.h"

#include "ItemData/NMItemBaseDataAsset.h"
#include "Net/UnrealNetwork.h"

UNMInventoryComponent::UNMInventoryComponent()
{
	Inventory.Init(FNMInventoryItem(), InventoryMaxCount);

	SetIsReplicatedByDefault(true);
}

void UNMInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UNMInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UNMInventoryComponent, Inventory, COND_OwnerOnly);
}

int32 UNMInventoryComponent::AddInventoryItem(UNMItemBaseDataAsset* NewItemData, int32 NewItemCount)
{
	// UE_LOG(LogTemp, Warning, TEXT("Owner: %s, Server: %s"), *GetOwner()->GetName(), *GetWorld()->GetFirstPlayerController()->GetPawn()->GetName());
	
	for (auto& Item : Inventory)
	{
		// ItemData doesn't exist and inventory slot is empty
		if (Item.ItemData == nullptr)
		{
			Item.ItemData = NewItemData;
		}

		// ItemData exist in Inventory
		if (Item.GetItemName() == NewItemData->ItemName)
		{
			NewItemCount = Item.AddItem(NewItemCount);

			// Add Item Succeed
			if (NewItemCount == 0)
			{
				break;
			}
		}
	}

	return NewItemCount;
}

void UNMInventoryComponent::RemoveInventoryITem(int32 SlotNum)
{
	Inventory[SlotNum].Clear();

	// Spawn Item
	
}

void UNMInventoryComponent::OnRep_Inventory()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Inventory"));
}
