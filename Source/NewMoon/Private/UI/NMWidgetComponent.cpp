// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NMWidgetComponent.h"

#include "UI/NMUserWidget.h"

void UNMWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UNMUserWidget* NMUserWidget = Cast<UNMUserWidget>(GetWidget());
	if (NMUserWidget)
	{
		NMUserWidget->SetOwningActor(GetOwner());
	}
}
