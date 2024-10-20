// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Chase.h"

#include "AIController.h"
#include "AI/NMAICharacter.h"

UBTTask_Chase::UBTTask_Chase()
{
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANMAICharacter* AICharacter = Cast<ANMAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(AICharacter))
	{
		return EBTNodeResult::Failed;
	}
	
	AcceptableRadius = AICharacter->AttackRange;
	
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return Result;
}
