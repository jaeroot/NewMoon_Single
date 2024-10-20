// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HeroesWidget.h"

#include "Character/LobbyPawn.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHeroesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HeroName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_HeroName")));
	AttackProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_Attack")));
	DefenseProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_Defense")));
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HP")));
	StaminaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_Stamina")));

	UE4Button = Cast<UButton>(GetWidgetFromName(TEXT("Btn_UE4")));
	if (IsValid(UE4Button))
	{
		UE4Button->OnClicked.AddUniqueDynamic(this, &UHeroesWidget::UE4ButtonClicked);
	}
	
	UE5MannyButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_UE5Manny")));
	if (IsValid(UE5MannyButton))
	{
		UE5MannyButton->OnClicked.AddUniqueDynamic(this, &UHeroesWidget::UE5MannyButtonClicked);
	}
	
	UE5QuinnButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_UE5Quinn")));
	if (IsValid(UE5QuinnButton))
	{
		UE5QuinnButton->OnClicked.AddUniqueDynamic(this, &UHeroesWidget::UE5QuinnButtonClicked);
	}
	
	PaladinButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Paladin")));
	if (IsValid(PaladinButton))
	{
		PaladinButton->OnClicked.AddUniqueDynamic(this, &UHeroesWidget::PaladinButtonClicked);
	}
	
	MariaButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Maria")));
	if (IsValid(MariaButton))
	{
		MariaButton->OnClicked.AddUniqueDynamic(this, &UHeroesWidget::MariaButtonClicked);
	}
	
	EscButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_ESC")));
	if (IsValid(EscButton))
	{
		EscButton->OnClicked.AddUniqueDynamic(this, &UHeroesWidget::EscButtonClicked);
	}
}

void UHeroesWidget::SetHeroName(const FString& Name)
{
	HeroName->SetText(FText::FromString(Name));
}

void UHeroesWidget::SetAttackProgressBar(float Percent)
{
	AttackProgressBar->SetPercent(Percent);
}

void UHeroesWidget::SetDefenseProgressBar(float Percent)
{
	DefenseProgressBar->SetPercent(Percent);
}

void UHeroesWidget::SetHPProgressBar(float Percent)
{
	HPProgressBar->SetPercent(Percent);
}

void UHeroesWidget::SetStaminaProgressBar(float Percent)
{
	StaminaProgressBar->SetPercent(Percent);
}

void UHeroesWidget::UE4ButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->ChangeHero(EHeroType::UE4);
		PlayerPawn->EquipWeapon(EWeaponType::NoEquip);
	}
}

void UHeroesWidget::UE5MannyButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->ChangeHero(EHeroType::UE5Manny);
		PlayerPawn->EquipWeapon(EWeaponType::NoEquip);
	}
}

void UHeroesWidget::UE5QuinnButtonClicked()
{
}

void UHeroesWidget::PaladinButtonClicked()
{
}

void UHeroesWidget::MariaButtonClicked()
{
}

void UHeroesWidget::EscButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->ChangeLobbyType(ELobbyType::Main);
	}
}
