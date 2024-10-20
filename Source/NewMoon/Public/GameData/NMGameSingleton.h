// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NMHeroData.h"
#include "UObject/NoExportTypes.h"
#include "NMGameSingleton.generated.h"

class UCompositeDataTable;
DECLARE_LOG_CATEGORY_EXTERN(LogNMGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class NEWMOON_API UNMGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UNMGameSingleton();
	static UNMGameSingleton& Get();

// Hero Data
public:
	FNMHeroStat GetHeroStat(const FName& HeroName) const;
	FNMHeroData GetHeroData(const FName& HeroName) const;
	FNMHeroLevelData GetHeroLevelData(const int32 InLevel) const;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> HeroDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> HeroLevelDataTable;
	
};
