// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NMAIBossController.generated.h"

/**
 * 
 */
UCLASS()
class NEWMOON_API ANMAIBossController : public AAIController
{
	GENERATED_BODY()

public:
	ANMAIBossController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY()
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset;
	
};
