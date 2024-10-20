// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroesWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class NEWMOON_API UHeroesWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void SetHeroName(const FString& Name);
	
	UFUNCTION()
	void SetAttackProgressBar(float Percent);
	UFUNCTION()
	void SetDefenseProgressBar(float Percent);
	UFUNCTION()
	void SetHPProgressBar(float Percent);
	UFUNCTION()
	void SetStaminaProgressBar(float Percent);
	
	UFUNCTION()
	void UE4ButtonClicked();
	UFUNCTION()
	void UE5MannyButtonClicked();
	UFUNCTION()
	void UE5QuinnButtonClicked();
	UFUNCTION()
	void PaladinButtonClicked();
	UFUNCTION()
	void MariaButtonClicked();
	
	UFUNCTION()
	void EscButtonClicked();

protected:
	TObjectPtr<UTextBlock> HeroName;
	TObjectPtr<UProgressBar> AttackProgressBar;
	TObjectPtr<UProgressBar> DefenseProgressBar;
	TObjectPtr<UProgressBar> HPProgressBar;
	TObjectPtr<UProgressBar> StaminaProgressBar;

	TObjectPtr<UButton> UE4Button;
	TObjectPtr<UButton> UE5MannyButton;
	TObjectPtr<UButton> UE5QuinnButton;
	TObjectPtr<UButton> PaladinButton;
	TObjectPtr<UButton> MariaButton;
	
	TObjectPtr<UButton> EscButton;
};
