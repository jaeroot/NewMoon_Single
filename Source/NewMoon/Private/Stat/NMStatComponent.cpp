// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/NMStatComponent.h"

UNMStatComponent::UNMStatComponent()
{
	bWantsInitializeComponent = true;
}

void UNMStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UNMStatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

