// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackCombo.h"

#include "Character/NMPlayerCharacter.h"


UAnimNotify_AttackCombo::UAnimNotify_AttackCombo()
{
}

void UAnimNotify_AttackCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ANMPlayerCharacter* NMCharacter = Cast<ANMPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(NMCharacter))
	{
		NMCharacter->AttackCombo();
	}
}
