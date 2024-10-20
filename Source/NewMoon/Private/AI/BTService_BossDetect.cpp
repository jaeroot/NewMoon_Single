// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_BossDetect.h"

#include "AIController.h"
#include "AI/NMAIInfo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "Physics/NMCollision.h"

UBTService_BossDetect::UBTService_BossDetect()
{
	NodeName = TEXT("BossDetect");
	Interval = 1.0f;
	TraceRadius = 10000.0f;
}

void UBTService_BossDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(AIPawn))
	{
		return;
	}

	FVector Center = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_BASELOCATION);
	UWorld* World = AIPawn->GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	// Detect
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, AIPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		NM_CCHANNEL_ENEMYATTACK,
		FCollisionShape::MakeBox(FVector(TraceRadius, TraceRadius, 1000.0f)),
		CollisionQueryParams
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_Target, Pawn);
				// DrawDebugBox(World, Center, FVector(TraceRadius, TraceRadius, 1000.0f), FColor::Green, false, 0.2f);
				
				FVector CurrentLocation = AIPawn->GetActorLocation();
				FVector TargetLocation = Pawn->GetActorLocation();
				float Distance = FVector::Distance(TargetLocation, CurrentLocation);
				OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_TRACEDISTANCE, Distance);
				
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_Target, nullptr);
	// DrawDebugBox(World, Center, FVector(TraceRadius, TraceRadius, 1000.0f), FColor::Red, false, 0.2f);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_TRACEDISTANCE, 0.0f);
}
