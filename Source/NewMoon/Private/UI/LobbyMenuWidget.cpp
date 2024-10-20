// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyMenuWidget.h"


#include "Character/LobbyPawn.h"
#include "Components/Button.h"

void ULobbyMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Play")));
	if (IsValid(PlayButton))
	{
		PlayButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenuWidget::PlayButtonClicked);
	}
	
	HeroesButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Heroes")));
	if (IsValid(HeroesButton))
	{
		HeroesButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenuWidget::HeroesButtonClicked);
	}
	
	WeaponsButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Weapons")));
	if (IsValid(WeaponsButton))
	{
		WeaponsButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenuWidget::WeaponsButtonClicked);
	}
	
	ChallengesButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Challenges")));
	if (IsValid(ChallengesButton))
	{
		ChallengesButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenuWidget::ChallengesButtonClicked);
	}
	
	OptionsButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Options")));
	if (IsValid(OptionsButton))
	{
		OptionsButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenuWidget::OptionsButtonClicked);
	}
	
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_EXIT")));
	if (IsValid(ExitButton))
	{
		ExitButton->OnClicked.AddUniqueDynamic(this, &ULobbyMenuWidget::ExitButtonClicked);
	}
	
}

void ULobbyMenuWidget::PlayButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Demonstration_Village"));
}

void ULobbyMenuWidget::HeroesButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->ChangeLobbyType(ELobbyType::Heroes);
	}
}

void ULobbyMenuWidget::WeaponsButtonClicked()
{
	auto PlayerPawn = Cast<ALobbyPawn>(GetOwningPlayerPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->ChangeLobbyType(ELobbyType::Weapons);
	}
}

void ULobbyMenuWidget::ChallengesButtonClicked()
{
	
}

void ULobbyMenuWidget::OptionsButtonClicked()
{
	
}

void ULobbyMenuWidget::ExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
