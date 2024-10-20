// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "AI/NMAICharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ANMAICharacter* AICharacter = Cast<ANMAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(AICharacter))
	{
		return EBTNodeResult::Failed;
	}
	AICharacter->SetIsAttack(true);
	AICharacter->Attack();
	

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ANMAICharacter* AICharacter = Cast<ANMAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(AICharacter))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (!AICharacter->GetISAttack())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}	
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
