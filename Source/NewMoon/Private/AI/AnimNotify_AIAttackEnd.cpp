// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AnimNotify_AIAttackEnd.h"

#include "AI/NMAIBoss.h"
#include "AI/NMAICharacter.h"

UAnimNotify_AIAttackEnd::UAnimNotify_AIAttackEnd()
{
}

void UAnimNotify_AIAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ANMAICharacter* AICharacter = Cast<ANMAICharacter>(MeshComp->GetOwner());
	if (IsValid(AICharacter))
	{
		AICharacter->SetIsAttack(false);
	}
	
	ANMAIBoss* AIBossCharacter = Cast<ANMAIBoss>(MeshComp->GetOwner());
	if (IsValid(AIBossCharacter))
	{
		AIBossCharacter->SetIsAttack(false);
	}
}
