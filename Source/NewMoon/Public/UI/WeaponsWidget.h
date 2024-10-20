// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponsWidget.generated.h"

class UVerticalBox;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class NEWMOON_API UWeaponsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION()
	void SetWeaponName(const FString& Name);
	
	UFUNCTION()
	void NoEquipButtonClicked();
	UFUNCTION()
	void SwordAndShieldButtonClicked();
	UFUNCTION()
	void TwoHandedSwordButtonClicked();
	UFUNCTION()
	void MaceButtonClicked();
	
	UFUNCTION()
	void Attack1ButtonClicked();
	UFUNCTION()
	void Attack2ButtonClicked();
	UFUNCTION()
	void Attack3ButtonClicked();
	UFUNCTION()
	void CoreAttackButtonClicked();
	UFUNCTION()
	void SpecialSkill1ButtonClicked();
	UFUNCTION()
	void SpecialSkill2ButtonClicked();
	UFUNCTION()
	void NormalSkill1ButtonClicked();
	UFUNCTION()
	void NormalSkill2ButtonClicked();
	UFUNCTION()
	void NormalSkill3ButtonClicked();
	UFUNCTION()
	void NormalSkill4ButtonClicked();
	UFUNCTION()
	void NormalSkill5ButtonClicked();
	
	UFUNCTION()
	void EscButtonClicked();

protected:
	// Weapon List
	TObjectPtr<UButton> NoEquipButton;
	TObjectPtr<UButton> SwordAndShieldButton;
	TObjectPtr<UButton> TwoHandedSwordButton;
	TObjectPtr<UButton> MaceButton;

	// Selected Weapon Name
	TObjectPtr<UTextBlock> WeaponName;

	// Weapon Skill Box
	TObjectPtr<UVerticalBox> WeaponSkillBox;

	// Weapon Skills
	TObjectPtr<UButton> Attack1Button;
	TObjectPtr<UButton> Attack2Button;
	TObjectPtr<UButton> Attack3Button;
	TObjectPtr<UButton> CoreAttackButton;
	TObjectPtr<UButton> SpecialSkill1Button;
	TObjectPtr<UButton> SpecialSkill2Button;
	TObjectPtr<UButton> NormalSkill1Button;
	TObjectPtr<UButton> NormalSkill2Button;
	TObjectPtr<UButton> NormalSkill3Button;
	TObjectPtr<UButton> NormalSkill4Button;
	TObjectPtr<UButton> NormalSkill5Button;

	// Close Button
	TObjectPtr<UButton> EscButton;
};
