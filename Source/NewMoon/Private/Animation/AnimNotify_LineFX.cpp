// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_LineFX.h"

#include "Character/NMPlayerCharacter.h"

UAnimNotify_LineFX::UAnimNotify_LineFX()
{
}

void UAnimNotify_LineFX::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ANMPlayerCharacter* NMCharacter = Cast<ANMPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(NMCharacter))
	{
		NMCharacter->ActivateLineFX();
	}
}
