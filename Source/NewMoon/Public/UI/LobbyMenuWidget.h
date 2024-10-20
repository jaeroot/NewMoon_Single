// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class NEWMOON_API ULobbyMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:

protected:
	UFUNCTION()
	void PlayButtonClicked();
	
	UFUNCTION()
	void HeroesButtonClicked();
	
	UFUNCTION()
	void WeaponsButtonClicked();
	
	UFUNCTION()
	void ChallengesButtonClicked();
	
	UFUNCTION()
	void OptionsButtonClicked();
	
	UFUNCTION()
	void ExitButtonClicked();
	

protected:
	TObjectPtr<UButton> PlayButton;
	TObjectPtr<UButton> HeroesButton;
	TObjectPtr<UButton> WeaponsButton;
	TObjectPtr<UButton> ChallengesButton;
	TObjectPtr<UButton> OptionsButton;
	TObjectPtr<UButton> ExitButton;
};
