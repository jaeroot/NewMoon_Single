// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AIAttackMagic.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API UAnimNotify_AIAttackMagic : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_AIAttackMagic();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
