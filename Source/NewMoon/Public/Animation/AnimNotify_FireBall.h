// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FireBall.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API UAnimNotify_FireBall : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_FireBall();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
