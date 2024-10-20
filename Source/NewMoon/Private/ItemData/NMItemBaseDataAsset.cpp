// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData/NMItemBaseDataAsset.h"

FPrimaryAssetId UNMItemBaseDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
