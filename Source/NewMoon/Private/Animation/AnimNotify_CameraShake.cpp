// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_CameraShake.h"

#include "Character/NMPlayerCharacter.h"

UAnimNotify_CameraShake::UAnimNotify_CameraShake()
{
}

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ANMPlayerCharacter* NMCharacter = Cast<ANMPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(NMCharacter))
	{
		NMCharacter->StartCameraShake();
	}
}
