// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/NMGameSingleton.h"

#include "Engine/CompositeDataTable.h"

DEFINE_LOG_CATEGORY(LogNMGameSingleton);

UNMGameSingleton::UNMGameSingleton()
{
	// Hero
	static ConstructorHelpers::FObjectFinder<UDataTable>
		HeroDataRef(TEXT("/Game/GameData/DT_NMHeroData.DT_NMHeroData"));
	if (HeroDataRef.Succeeded())
	{
		HeroDataTable = HeroDataRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable>
		HeroLevelDataRef(TEXT("/Game/GameData/DT_NMHeroLevelData.DT_NMHeroLevelData"));
	if (HeroLevelDataRef.Succeeded())
	{
		HeroLevelDataTable = HeroLevelDataRef.Object;
	}

}

UNMGameSingleton& UNMGameSingleton::Get()
{
	UNMGameSingleton* GameSingleton = CastChecked<UNMGameSingleton>(GEngine->GameSingleton);
	if (IsValid(GameSingleton))
	{
		return *GameSingleton;
	}

	UE_LOG(LogNMGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UNMGameSingleton>();
}

FNMHeroStat UNMGameSingleton::GetHeroStat(const FName& HeroName) const
{
	FNMHeroStat* HeroStat = HeroDataTable->FindRow<FNMHeroStat>(HeroName, TEXT(""));

	return HeroStat ? *HeroStat : FNMHeroStat();
}

FNMHeroData UNMGameSingleton::GetHeroData(const FName& HeroName) const
{
	FNMHeroData* HeroData = HeroDataTable->FindRow<FNMHeroData>(HeroName, TEXT(""));

	return HeroData ? *HeroData : FNMHeroData();
}

FNMHeroLevelData UNMGameSingleton::GetHeroLevelData(const int32 InLevel) const
{
	FNMHeroLevelData* HeroLevelData =
		HeroLevelDataTable->FindRow<FNMHeroLevelData>(*FString::Printf(TEXT("%d"), InLevel), TEXT(""));

	return HeroLevelData ? *HeroLevelData : FNMHeroLevelData();
}
