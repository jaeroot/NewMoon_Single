// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AnimNotify_AIAttackMagic.h"

#include "AI/NMAIBoss.h"
#include "AI/NMAICharacter.h"

UAnimNotify_AIAttackMagic::UAnimNotify_AIAttackMagic()
{
}

void UAnimNotify_AIAttackMagic::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ANMAICharacter* AICharacter = Cast<ANMAICharacter>(MeshComp->GetOwner());
	if (IsValid(AICharacter))
	{
		AICharacter->MagicAttack();
	}
}
