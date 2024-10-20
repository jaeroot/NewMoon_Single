// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMAISpawner.h"

#include "AI/NMAICharacter.h"
#include "Components/CapsuleComponent.h"

ANMAISpawner::ANMAISpawner()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	
#if WITH_EDITORONLY_DATA
	Root->bVisualizeComponent = true;
#endif

}

void ANMAISpawner::Spawn()
{
	if (AIClass == nullptr)
	{
		return;
	}
	// UClass* Class,
	// 		FTransform const& Transform,
	// 		AActor* Owner = nullptr,
	// 		APawn* Instigator = nullptr,
	// 		ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined,
	// 		ESpawnActorScaleMethod TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot

	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	ANMAICharacter* AICharacter = AIClass->GetDefaultObject<ANMAICharacter>();
	if (IsValid(AICharacter))
	{
		Location.Z += AICharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}
	Transform.SetLocation(Location);
	
	AIActor = GetWorld()->SpawnActorDeferred<ANMAICharacter>(
		AIClass,
		Transform
	);
	if (IsValid(AIActor))
	{
		// Do Something

		AIActor->FinishSpawning(AIActor->GetTransform());
	}
}

void ANMAISpawner::BeginPlay()
{
	Super::BeginPlay();

}

