// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_Attack.h"

#include "Character/NMPlayerCharacter.h"


UAnimNotifyState_Attack::UAnimNotifyState_Attack()
{
}

void UAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	NMCharacter = Cast<ANMPlayerCharacter>(MeshComp->GetOwner());

	Attack();
}

void UAnimNotifyState_Attack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	Attack();
}

void UAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	Attack();

	if (IsValid(NMCharacter))
	{
		NMCharacter->AttackEnd();
	}
}

void UAnimNotifyState_Attack::Attack()
{
	if (IsValid(NMCharacter))
	{
		NMCharacter->Attack();
	}
}
