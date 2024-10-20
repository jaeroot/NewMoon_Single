// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/NMWeapon.h"

#include "Interface/NMCharacterInterface.h"

ANMWeapon::ANMWeapon()
{
	
}

void ANMWeapon::Interaction(AActor* OtherActor)
{
	INMCharacterInterface* InteractPawn = Cast<INMCharacterInterface>(OtherActor);
	if (InteractPawn)
	{
		InteractPawn->InteractItem(this);
	}

}
