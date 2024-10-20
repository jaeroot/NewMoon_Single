// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AIAttackEnd.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API UAnimNotify_AIAttackEnd : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_AIAttackEnd();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
