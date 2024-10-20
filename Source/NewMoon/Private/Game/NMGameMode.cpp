// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NMGameMode.h"

#include "Character/NMPlayerCharacter.h"

ANMGameMode::ANMGameMode()
{
	DefaultPawnClass = ANMPlayerCharacter::StaticClass();
}
