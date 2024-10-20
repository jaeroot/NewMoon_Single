// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AnimNotify_AIAttackMelee.h"

#include "AI/NMAIBoss.h"
#include "AI/NMAICharacter.h"

UAnimNotify_AIAttackMelee::UAnimNotify_AIAttackMelee()
{
}

void UAnimNotify_AIAttackMelee::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ANMAICharacter* AICharacter = Cast<ANMAICharacter>(MeshComp->GetOwner());
	if (IsValid(AICharacter))
	{
		AICharacter->MeleeAttack();
	}
	
	ANMAIBoss* AIBossCharacter = Cast<ANMAIBoss>(MeshComp->GetOwner());
	if (IsValid(AIBossCharacter))
	{
		AIBossCharacter->MeleeAttack();
	}
}
