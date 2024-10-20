// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NMProjectile.generated.h"

class UInterpToMovementComponent;

UCLASS()
class NEWMOON_API ANMProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ANMProjectile();

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInterpToMovementComponent> InterpToMovementComponent;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	FVector OriginLocation;
};
