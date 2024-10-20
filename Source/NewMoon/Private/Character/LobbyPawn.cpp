// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LobbyPawn.h"

#include "Animation/NMPlayerAnimInstance.h"
#include "Blueprint/UserWidget.h"

ALobbyPawn::ALobbyPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin"));
	if (MeshAsset.Succeeded())
	{
		Mesh1 = MeshAsset.Object;
		SkeletalMeshComponent->SetSkeletalMesh(Mesh1);
	}
	SetRootComponent(SkeletalMeshComponent);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset2(TEXT("/Game/Characters/Mannequins_UE5/Meshes/SKM_Manny.SKM_Manny"));
	if (MeshAsset2.Succeeded())
	{
		Mesh2 = MeshAsset2.Object;
	}
	
	// Set Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Animations/ABP_NMPlayer.ABP_NMPlayer_C'"));
	if (AnimClass.Succeeded())
	{
		SkeletalMeshComponent->SetAnimClass(AnimClass.Class);
	}

	// Set Linked Anim Layers
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		NoEquipAnimClass(TEXT("/Game/Characters/Animations/LinkedLayers/ABP_NoEquipAnimLayers.ABP_NoEquipAnimLayers_C"));
	if (NoEquipAnimClass.Succeeded())
	{
		LinkedAnimLayers.Add(EWeaponType::NoEquip, NoEquipAnimClass.Class);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		SwordAndShieldAnimClass(TEXT("/Game/Characters/Animations/LinkedLayers/ABP_SwordAndShieldAnimLayers.ABP_SwordAndShieldAnimLayers_C"));
	if (SwordAndShieldAnimClass.Succeeded())
	{
		LinkedAnimLayers.Add(EWeaponType::SwordAndShield, SwordAndShieldAnimClass.Class);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		TwoHandedSwordAnimClass(TEXT("/Game/Characters/Animations/LinkedLayers/ABP_TwoHandedSwordAnimLayers.ABP_TwoHandedSwordAnimLayers_C"));
	if (TwoHandedSwordAnimClass.Succeeded())
	{
		LinkedAnimLayers.Add(EWeaponType::TwoHandedSword, TwoHandedSwordAnimClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MaceAnimClass(TEXT("/Game/Characters/Animations/LinkedLayers/ABP_MaceAnimLayers.ABP_MaceAnimLayers_C"));
	if (MaceAnimClass.Succeeded())
	{
		LinkedAnimLayers.Add(EWeaponType::Mace, MaceAnimClass.Class);
	}

	// Set Sword and Shield
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SwordAsset(TEXT("/Game/Weapon/Mesh/Weapons/Weapons_Kit/SM_Sword.SM_Sword"));
	if (SwordAsset.Succeeded())
	{
		SwordMesh->SetStaticMesh(SwordAsset.Object);
	}
	SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordMesh->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_r_swordandshield"));
	SwordMesh->SetVisibility(false);
	
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ShieldAsset(TEXT("/Game/Weapon/Mesh/Weapons/Weapons_Kit/SM_Shield.SM_Shield"));
	if (ShieldAsset.Succeeded())
	{
		ShieldMesh->SetStaticMesh(ShieldAsset.Object);
	}
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldMesh->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_l_shield"));
	ShieldMesh->SetVisibility(false);

	MaceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mace"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MaceAsset(TEXT("/Game/Weapon/Mesh/Weapons/Weapons_Kit/SM_Mace.SM_Mace"));
	if (MaceAsset.Succeeded())
	{
		MaceMesh->SetStaticMesh(MaceAsset.Object);
	}
	MaceMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MaceMesh->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_r_mace"));
	MaceMesh->SetVisibility(false);

	
	static ConstructorHelpers::FClassFinder<UUserWidget>
		MainWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Lobby.WBP_Lobby_C'"));
	if (MainWidgetClass.Succeeded())
	{
		WidgetClasses.Add(ELobbyType::Main, MainWidgetClass.Class);
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget>
		HeroesWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Lobby/WBP_Heroes.WBP_Heroes_C'"));
	if (HeroesWidgetClass.Succeeded())
	{
		WidgetClasses.Add(ELobbyType::Heroes, HeroesWidgetClass.Class);
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget>
		WeaponsWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Lobby/WBP_Weapons.WBP_Weapons_C'"));
	if (WeaponsWidgetClass.Succeeded())
	{
		WidgetClasses.Add(ELobbyType::Weapons, WeaponsWidgetClass.Class);
	}
}

void ALobbyPawn::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputModeUIOnly;
	auto PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(InputModeUIOnly);
		PlayerController->SetShowMouseCursor(true);
	}

	SetViewTarget(LobbyType, 0.0f);
	SetWidget(LobbyType);

	AnimInstance = Cast<UNMPlayerAnimInstance>(SkeletalMeshComponent->GetAnimInstance());

	EquipWeapon(EWeaponType::NoEquip);
}

void ALobbyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyPawn::ChangeLobbyType(const ELobbyType& ELT)
{
	Widgets[LobbyType]->RemoveFromParent();
	
	LobbyType = ELT;
	
	SetViewTarget(LobbyType, 1.0f);
	SetWidget(LobbyType);
}

void ALobbyPawn::ChangeHero(const EHeroType& EHT)
{
	switch (EHT) {
	case EHeroType::UE4:
		SkeletalMeshComponent->SetSkeletalMesh(Mesh1);
		break;
	case EHeroType::UE5Manny:
		SkeletalMeshComponent->SetSkeletalMesh(Mesh2);
		break;
	case EHeroType::UE5Quinn:
		break;
	case EHeroType::Paladin:
		break;
	case EHeroType::Maria:
		break;
	default:
		break;
	}
}

void ALobbyPawn::EquipWeapon(const EWeaponType& EWT)
{
	SkeletalMeshComponent->LinkAnimClassLayers(LinkedAnimLayers[EWT]);
	
	switch (EWT) {
	case EWeaponType::NoEquip:
		SwordMesh->SetVisibility(false);
		ShieldMesh->SetVisibility(false);
		MaceMesh->SetVisibility(false);
		break;
	case EWeaponType::SwordAndShield:
		SwordMesh->SetVisibility(true);
		ShieldMesh->SetVisibility(true);
		MaceMesh->SetVisibility(false);
		break;
	case EWeaponType::TwoHandedSword:
		SwordMesh->SetVisibility(true);
		ShieldMesh->SetVisibility(false);
		MaceMesh->SetVisibility(false);
		break;
	case EWeaponType::Mace:
		SwordMesh->SetVisibility(false);
		ShieldMesh->SetVisibility(false);
		MaceMesh->SetVisibility(true);
		break;
	default:
		break;
	}

	WeaponType = EWT;
}

// void ALobbyPawn::PlayAttack()
// {
// 	AnimInstance->PlayAttackMontage();
// }
//
// void ALobbyPawn::PlayAttack2()
// {
// 	AnimInstance->PlayAttackMontage2();
// }

void ALobbyPawn::SetViewTarget(ELobbyType ELT, float BlendTime)
{
	auto PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->SetViewTargetWithBlend(TransitionCameras[ELT], BlendTime, VTBlend_Cubic);
	}
}

void ALobbyPawn::SetWidget(ELobbyType ELT)
{
	if (!Widgets.Contains(ELT))
	{
		Widgets.Add(ELT, CreateWidget<UUserWidget>(GetWorld(), WidgetClasses[ELT]));
	}

	if (IsValid(Widgets[ELT]))
	{
		Widgets[ELT]->AddToViewport();
	}
}

void ALobbyPawn::PlayAttackMontageSample(int MontageNum)
{
	switch (WeaponType) {
	case EWeaponType::NoEquip:
		break;
	case EWeaponType::SwordAndShield:
		AnimInstance->Montage_Play(Montage[MontageNum]);
		break;
	case EWeaponType::TwoHandedSword:
		AnimInstance->Montage_Play(Montage2[MontageNum]);
		break;
	case EWeaponType::Mace:
		AnimInstance->Montage_Play(Montage3[MontageNum]);
		break;
	default:
		break;
	}
}
