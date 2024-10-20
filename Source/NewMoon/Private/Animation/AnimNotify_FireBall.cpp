// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_FireBall.h"

#include "Character/NMPlayerCharacter.h"

UAnimNotify_FireBall::UAnimNotify_FireBall()
{
}

void UAnimNotify_FireBall::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ANMPlayerCharacter* NMCharacter = Cast<ANMPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(NMCharacter))
	{
		NMCharacter->FireBallAttack();
	}
}
