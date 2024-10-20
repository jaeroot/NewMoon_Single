// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMAIController.h"

#include "AI/NMAIInfo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ANMAIController::ANMAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_NormalAI.BB_NormalAI'"));
	if (BBAssetRef.Succeeded())
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_NormalAI.BT_NormalAI'"));
	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ANMAIController::RunAI()
{
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(BBKEY_BASELOCATION, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ANMAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void ANMAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
