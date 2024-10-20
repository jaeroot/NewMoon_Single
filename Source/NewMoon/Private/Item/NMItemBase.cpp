// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/NMItemBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameData/NMGameSingleton.h"
#include "Net/UnrealNetwork.h"
#include "Physics/NMCollision.h"


ANMItemBase::ANMItemBase()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->SetCollisionProfileName(NM_CPROFILE_ITEMPHYSICS);
	Capsule->SetSimulatePhysics(true);
	Capsule->SetCapsuleHalfHeight(50.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->bReceivesDecals = false;
	Mesh->SetIsReplicated(true);

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetCollisionProfileName(NM_CPROFILE_ITEM);
	InteractCollision->SetSphereRadius(100.0f);

	bReplicates = true;
}

void ANMItemBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANMItemBase, CurrentCount);
}

void ANMItemBase::SetItem(const FName& Name)
{
	// // Get Item Data from GameSingleton
	// ItemData = UNMGameSingleton::Get().GetItemData(Name);
	//
	// // Set Capsule
	// CapsuleRadius = ItemData.CapsuleRadius;
	// Capsule->SetCapsuleRadius(CapsuleRadius);
	// CapsuleHeight = ItemData.CapsuleHeight;
	// Capsule->SetCapsuleHalfHeight(CapsuleHeight);
	//
	// // Set Mesh
	// if (ItemData.Mesh.IsPending())
	// {
	// 	ItemData.Mesh.LoadSynchronous();
	// }
	// Mesh->SetStaticMesh(ItemData.Mesh.Get());
	// Mesh->SetRelativeTransform(ItemData.MeshTransform);
}

void ANMItemBase::OnRep_CurrentCount()
{
	
}
