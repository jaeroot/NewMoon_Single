// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NMAISpawnTrigger.generated.h"

class ANMAISpawner;
class UBoxComponent;

UCLASS()
class NEWMOON_API ANMAISpawnTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ANMAISpawnTrigger();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ANMAISpawner>> Spawners;

	UFUNCTION()
	void SpawnAI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	uint8 bSpawned : 1;
};
