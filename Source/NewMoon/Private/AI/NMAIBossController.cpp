// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMAIBossController.h"

#include "AI/NMAIInfo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ANMAIBossController::ANMAIBossController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_BossAI.BB_BossAI'"));
	if (BBAssetRef.Succeeded())
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_BossAI.BT_BossAI'"));
	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ANMAIBossController::RunAI()
{
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(BBKEY_BASELOCATION, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ANMAIBossController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void ANMAIBossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
