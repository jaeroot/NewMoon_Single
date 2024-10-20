// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMAISpawnTrigger.h"

#include "AI/NMAISpawner.h"
#include "Components/BoxComponent.h"
#include "Physics/NMCollision.h"

ANMAISpawnTrigger::ANMAISpawnTrigger()
{
	bSpawned = false;
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName(NM_CPROFILE_ITEM);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ANMAISpawnTrigger::SpawnAI);
	
#if WITH_EDITORONLY_DATA
	TriggerBox->bVisualizeComponent = true;
#endif
}

void ANMAISpawnTrigger::SpawnAI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& Spawner : Spawners)
	{
		Spawner->Spawn();
	}

	bSpawned = true;
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
