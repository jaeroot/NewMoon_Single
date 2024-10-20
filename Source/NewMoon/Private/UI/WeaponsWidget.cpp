// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponsWidget.h"

#include "Character/LobbyPawn.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UWeaponsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Weapon List
	NoEquipButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_NoEquip")));
	if (IsValid(NoEquipButton))
	{
		NoEquipButton->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::NoEquipButtonClicked);
	}
	
	SwordAndShieldButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_SwordAndShield")));
	if (IsValid(SwordAndShieldButton))
	{
		SwordAndShieldButton->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::SwordAndShieldButtonClicked);
	}
	
	TwoHandedSwordButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_TwoHandedSword")));
	if (IsValid(TwoHandedSwordButton))
	{
		TwoHandedSwordButton->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::TwoHandedSwordButtonClicked);
	}

	MaceButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Mace")));
	if (IsValid(MaceButton))
	{
		MaceButton->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::MaceButtonClicked);
	}

	
	// Select Weapon Name
	WeaponName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_WeaponName")));
	if (IsValid(WeaponName))
	{
		WeaponName->SetVisibility(ESlateVisibility::Collapsed);
	}


	// Weapon Skill Box
	WeaponSkillBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("WeaponSkillBox")));
	if (IsValid(WeaponSkillBox))
	{
		WeaponSkillBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	// Weapon Skills
	Attack1Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Attack1")));
	if (IsValid(Attack1Button))
	{
		Attack1Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::Attack1ButtonClicked);
	}
	
	Attack2Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Attack2")));
	if (IsValid(Attack2Button))
	{
		Attack2Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::Attack2ButtonClicked);
	}
	
	Attack3Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Attack3")));
	if (IsValid(Attack3Button))
	{
		Attack3Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::Attack3ButtonClicked);
	}
	
	CoreAttackButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_CoreAttack1")));
	if (IsValid(CoreAttackButton))
	{
		CoreAttackButton->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::CoreAttackButtonClicked);
	}
	
	SpecialSkill1Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_SpecialSkill1")));
	if (IsValid(SpecialSkill1Button))
	{
		SpecialSkill1Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::SpecialSkill1ButtonClicked);
	}
	
	SpecialSkill2Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_SpecialSkill2")));
	if (IsValid(SpecialSkill2Button))
	{
		SpecialSkill2Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::SpecialSkill2ButtonClicked);
	}
	
	NormalSkill1Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_NormalSkill1")));
	if (IsValid(NormalSkill1Button))
	{
		NormalSkill1Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::NormalSkill1ButtonClicked);
	}
	
	NormalSkill2Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_NormalSkill2")));
	if (IsValid(NormalSkill2Button))
	{
		NormalSkill2Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::NormalSkill2ButtonClicked);
	}
	
	NormalSkill3Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_NormalSkill3")));
	if (IsValid(NormalSkill3Button))
	{
		NormalSkill3Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::NormalSkill3ButtonClicked);
	}
	
	NormalSkill4Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_NormalSkill4")));
	if (IsValid(NormalSkill4Button))
	{
		NormalSkill4Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::NormalSkill4ButtonClicked);
	}
	
	NormalSkill5Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_NormalSkill5")));
	if (IsValid(NormalSkill5Button))
	{
		NormalSkill5Button->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::NormalSkill5ButtonClicked);
	}


	// Close Button
	EscButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_ESC")));
	if (IsValid(EscButton))
	{
		EscButton->OnClicked.AddUniqueDynamic(this, &UWeaponsWidget::EscButtonClicked);
	}
}

void UWeaponsWidget::SetWeaponName(const FString& Name)
{
	WeaponName->SetText(FText::FromString(Name));
}

void UWeaponsWidget::NoEquipButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->EquipWeapon(EWeaponType::NoEquip);
	}
	
	SetWeaponName(TEXT("장착해제"));
	WeaponName->SetVisibility(ESlateVisibility::Collapsed);
	WeaponSkillBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UWeaponsWidget::SwordAndShieldButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->EquipWeapon(EWeaponType::SwordAndShield);
	}
	
	SetWeaponName(TEXT("한손검"));
	WeaponName->SetVisibility(ESlateVisibility::Visible);
	WeaponSkillBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWeaponsWidget::TwoHandedSwordButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->EquipWeapon(EWeaponType::TwoHandedSword);
	}
	
	SetWeaponName(TEXT("두손검"));
	WeaponName->SetVisibility(ESlateVisibility::Visible);
	WeaponSkillBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWeaponsWidget::MaceButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->EquipWeapon(EWeaponType::Mace);
	}
	
	SetWeaponName(TEXT("도끼"));
	WeaponName->SetVisibility(ESlateVisibility::Visible);
	WeaponSkillBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWeaponsWidget::Attack1ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(0);
	}
}

void UWeaponsWidget::Attack2ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(1);
	}
}

void UWeaponsWidget::Attack3ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(2);
	}
}

void UWeaponsWidget::CoreAttackButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(3);
	}
}

void UWeaponsWidget::SpecialSkill1ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(4);
	}
}

void UWeaponsWidget::SpecialSkill2ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(5);
	}
}

void UWeaponsWidget::NormalSkill1ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(6);
	}
}

void UWeaponsWidget::NormalSkill2ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(7);
	}
}

void UWeaponsWidget::NormalSkill3ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(8);
	}
}

void UWeaponsWidget::NormalSkill4ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(9);
	}
}

void UWeaponsWidget::NormalSkill5ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->PlayAttackMontageSample(10);
	}
}

void UWeaponsWidget::EscButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->ChangeLobbyType(ELobbyType::Main);
	}
}
