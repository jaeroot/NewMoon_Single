// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API UAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_CameraShake();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
