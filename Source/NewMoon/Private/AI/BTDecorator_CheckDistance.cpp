// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckDistance.h"

#include "AIController.h"
#include "AI/NMAIInfo.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
	NodeName = TEXT("IsInTraceDistance");
	TraceMaxDistance = 1000.0f;
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	float CurrentDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_TRACEDISTANCE);

	if (CurrentDistance <= TraceMaxDistance)
	{
		return true;
	}

	return false;
}
