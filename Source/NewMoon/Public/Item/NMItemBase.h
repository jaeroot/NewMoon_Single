// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NMItemBase.generated.h"

class UNMItemBaseDataAsset;
class USphereComponent;
class UCapsuleComponent;

UCLASS()
class NEWMOON_API ANMItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ANMItemBase();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
// Item Data
public:
	void SetItem(const FName& Name);
	
	FORCEINLINE int32 GetCurrentCount() const { return CurrentCount; }
	FORCEINLINE UNMItemBaseDataAsset* GetItemData() const { return ItemData; }

	FORCEINLINE void SetItemCount(int32 NewItemCount) { CurrentCount = NewItemCount; }

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentCount, EditAnywhere, Category = Item)
	int32 CurrentCount = 1;

	UFUNCTION()
	void OnRep_CurrentCount();


// Item Mesh
protected:
	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<UCapsuleComponent> Capsule;
	
	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<USphereComponent> InteractCollision;

// ItemData
protected:
	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<UNMItemBaseDataAsset> ItemData;
	
};
