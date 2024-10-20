// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Attack.generated.h"

class ANMPlayerCharacter;
/**
 * 
 */
UCLASS()
class NEWMOON_API UAnimNotifyState_Attack : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_Attack();
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	void Attack();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	FName FirstSocketName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	FName SecondSocketName = NAME_None;

private:
	UPROPERTY()
	TObjectPtr<ANMPlayerCharacter> NMCharacter;
};
