// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NMInputData.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class NEWMOON_API UNMMenuInputData : public UObject
{
	GENERATED_BODY()

public:
	UNMMenuInputData();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> MenuInputMappingContext;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InventoryAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> SettingAction;
	
};

UCLASS()
class NEWMOON_API UNMFreeInputData : public UObject
{
	GENERATED_BODY()

public:
	UNMFreeInputData();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FreeInputMappingContext;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> FreeMoveAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> FreeLookAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;
	
};

UCLASS()
class NEWMOON_API UNMCombatInputData : public UObject
{
	GENERATED_BODY()

public:
	UNMCombatInputData();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> CombatInputMappingContext;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> DashAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> AttackAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> SpecialSkillAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> NormalSkillAction1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> NormalSkillAction2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> UltimateSkillAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LockAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> ResurrectAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> NoEquipAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InteractAction;
	
};
