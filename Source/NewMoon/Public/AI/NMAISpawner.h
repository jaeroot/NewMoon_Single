// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NMAISpawner.generated.h"

class ANMAICharacter;

UCLASS()
class NEWMOON_API ANMAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ANMAISpawner();

	void Spawn();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ANMAICharacter> AIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ANMAICharacter> AIActor;
};
