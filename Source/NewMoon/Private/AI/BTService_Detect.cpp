// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"

#include "AIController.h"
#include "AI/NMAIInfo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "Physics/NMCollision.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
	TraceRadius = 700.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(AIPawn))
	{
		return;
	}

	FVector Center = AIPawn->GetActorLocation();
	UWorld* World = AIPawn->GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	// Current Location
	FVector CurrentLocation = Center;
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_CURRENTLOCATION, CurrentLocation);
	
	FVector BaseLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_BASELOCATION);
	float Distance = FVector::Distance(BaseLocation, CurrentLocation);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_TRACEDISTANCE, Distance);

	// Detect
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, AIPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		NM_CCHANNEL_ENEMYATTACK,
		FCollisionShape::MakeSphere(TraceRadius),
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
				// DrawDebugSphere(World, Center, TraceRadius, 16, FColor::Green, false, 0.2f);

				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_Target, nullptr);
	// DrawDebugSphere(World, Center, TraceRadius, 16, FColor::Red, false, 0.2f);
	
}
