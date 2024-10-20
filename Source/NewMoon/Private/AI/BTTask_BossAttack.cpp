// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_BossAttack.h"

#include "AIController.h"
#include "AI/NMAIBoss.h"

UBTTask_BossAttack::UBTTask_BossAttack()
{
	NodeName = TEXT("BossAttack");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ANMAIBoss* AICharacter = Cast<ANMAIBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(AICharacter))
	{
		return EBTNodeResult::Failed;
	}
	AICharacter->SetIsAttack(true);
	AICharacter->Attack();
	

	return EBTNodeResult::InProgress;
}

void UBTTask_BossAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ANMAIBoss* AICharacter = Cast<ANMAIBoss>(OwnerComp.GetAIOwner()->GetPawn());
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

void UBTTask_BossAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
