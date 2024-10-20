// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Pawn.h"
#include "LobbyPawn.generated.h"

class UNMPlayerAnimInstance;

UENUM()
enum class ELobbyType : uint8
{
	Main,
	Heroes,
	Weapons,
	Menu
};

UENUM()
enum class EHeroType
{
	UE4,
	UE5Manny,
	UE5Quinn,
	Paladin,
	Maria
};

UENUM()
enum class EWeaponType
{
	NoEquip,
	SwordAndShield,
	TwoHandedSword,
	Mace
};

UCLASS()
class NEWMOON_API ALobbyPawn : public APawn
{
	GENERATED_BODY()

public:
	ALobbyPawn();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	void ChangeLobbyType(const ELobbyType& ELT);

	void ChangeHero(const EHeroType& EHT);
	void EquipWeapon(const EWeaponType& EWT);

	// void PlayAttack();
	// void PlayAttack2();

protected:
	void SetViewTarget(ELobbyType ELT, float BlendTime);
	void SetWidget(ELobbyType ELT);


protected:
	// Mesh		20, 70, 0,		0, 0, -35
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMesh> Mesh1;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMesh> Mesh2;
	
	// Animation
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNMPlayerAnimInstance> AnimInstance;

	// Linked Anim Layers
	UPROPERTY(VisibleAnywhere)
	TMap<EWeaponType, TSubclassOf<UAnimInstance>> LinkedAnimLayers;

	//weapon
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SwordMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ShieldMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MaceMesh;
	
	UPROPERTY(EditAnywhere)
	TMap<ELobbyType, TObjectPtr<AActor>> TransitionCameras;
	
	UPROPERTY(VisibleAnywhere)
	TMap<ELobbyType, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY(VisibleAnywhere)
	TMap<ELobbyType, TObjectPtr<UUserWidget>> Widgets;
	

protected:
	ELobbyType LobbyType = ELobbyType::Main;

	
// Sample
public:
	void PlayAttackMontageSample(int MontageNum);

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UAnimMontage>> Montage;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UAnimMontage>> Montage2;
	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UAnimMontage>> Montage3;

	EWeaponType WeaponType = EWeaponType::NoEquip;
};
