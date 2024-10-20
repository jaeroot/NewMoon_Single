// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMAICharacter.h"

#include "UI/NMUserWidget.h"
#include "AI/NMAIController.h"
#include "Character/NMPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Physics/NMCollision.h"
#include "UI/NMWidgetComponent.h"

ANMAICharacter::ANMAICharacter()
{
	bIsAttack = false;
	
	AIControllerClass = ANMAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCapsuleHalfHeight(65.0f);
	GetCapsuleComponent()->SetCollisionProfileName(NM_CPROFILE_ENEMY);
	
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -65.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh());

	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	ShieldMesh->SetupAttachment(GetMesh());

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Goblin/ABP_Goblin.ABP_Goblin_C'"));
	if (AnimClassRef.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		WeaponAnimClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Goblin/ABP_Goblin_Weapon.ABP_Goblin_Weapon_C'"));
	if (WeaponAnimClassRef.Succeeded())
	{
		WeaponMesh->SetAnimClass(WeaponAnimClassRef.Class);
	}

	// Set Stat
	MaxHP = 50.0f;
	AttackDamage = 10.0f;
	bIsBoss = false;

	HPLerpTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HPLerpTimeline"));
	OldHPCurveCallback.BindUFunction(this, FName("OldHPLerp"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		OldHPCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/UI/InGame/LayeredHPBar/Curve_OldHP_Float.Curve_OldHP_Float'"));
	if (OldHPCurveRef.Succeeded())
	{
		OldHPCurve = OldHPCurveRef.Object;

		HPLerpTimeline->SetLooping(false);
		HPLerpTimeline->AddInterpFloat(OldHPCurve, OldHPCurveCallback);
		HPLerpTimeline->SetTimelineLength(0.5f);
	}

	MeleeParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("MeleeParticle"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		// MeleeParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Paragon/Sparrow/FX/Abilities/Primary/P_Sparrow_Primary_Ballistic_HitPlayer.P_Sparrow_Primary_Ballistic_HitPlayer'"));
		MeleeParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Paragon/Sparrow/FX/Abilities/Primary/P_Sparrow_HitHero.P_Sparrow_HitHero'"));
	if (MeleeParticleRef.Succeeded())
	{
		MeleeParticle = MeleeParticleRef.Object;
	}

	InfoWidget = CreateDefaultSubobject<UNMWidgetComponent>(TEXT("InfoWidget"));
	InfoWidget->SetupAttachment(GetMesh());
	InfoWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UNMUserWidget>
		InfoWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/InGame/LayeredHPBar/WBP_MonsterHPBar.WBP_MonsterHPBar_C'"));
	if (InfoWidgetClassRef.Succeeded())
	{
		InfoWidget->SetWidgetClass(InfoWidgetClassRef.Class);
	}
	InfoWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InfoWidget->SetDrawSize(FVector2D(150.0f, 26.0f));
	InfoWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AIName = TEXT("몬스터");


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WeaponDeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Weapon_Dead.AM_Weapon_Dead'"));
	if (WeaponDeadMontageRef.Succeeded())
	{
		WeaponDeadMontage = WeaponDeadMontageRef.Object;
	}
}

float ANMAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HP = FMath::Clamp(HP - Damage, 0.0f, MaxHP);
	SavedOldHP = OldHP;
	CalculateOldHP();

	if (HP <= KINDA_SMALL_NUMBER)
	{
		if (bIsBoss)
		{
			ClearStage();
		}
		SetDead();
	}

	return Damage;
}

void ANMAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HP = MaxHP;
	OldHP = HP;
	SavedOldHP = OldHP;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ANMAICharacter::Attack()
{

}

void ANMAICharacter::MeleeAttack()
{
	TArray<FHitResult> HitResults;
	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector EndLocation = StartLocation + GetActorForwardVector() * AttackRange;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		NM_CCHANNEL_ENEMYATTACK,
		FCollisionShape::MakeSphere(30.0f),
		Params
	);

	if (bResult)
	{
		FDamageEvent DamageEvent;
		for (auto HitResult : HitResults)
		{
			HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
			
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				MeleeParticle,
				HitResult.ImpactPoint);

		}
	}

// #if ENABLE_DRAW_DEBUG
// 	FVector CapsuleOrigin = StartLocation + (EndLocation - StartLocation) * 0.5f;
// 	float CapsuleHalfHeight = AttackRange / 2;
// 	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//
// 	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 30.0f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
//
// #endif
}

void ANMAICharacter::MagicAttack()
{
	if (MagicProjectileClass == nullptr)
	{
		return;
	}

	FTransform Transform = GetActorTransform();
	Transform.SetLocation(WeaponMesh->GetSocketLocation(TEXT("Staff_Tip")));
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(
		MagicProjectileClass,
		Transform,
		Params
	);
}

void ANMAICharacter::SetDead()
{
	ANMAIController* NMAIController = Cast<ANMAIController>(GetController());
	if (NMAIController)
	{
		NMAIController->StopAI();
	}

	SetActorEnableCollision(false);

	GetMesh()->GetAnimInstance()->Montage_Play(DeadMontage);
	if (IsValid(WeaponMesh))
	{
		WeaponMesh->GetAnimInstance()->Montage_Play(WeaponDeadMontage);
	}

	if (IsValid(ShieldMesh->GetSkeletalMeshAsset()))
	{
		ShieldMesh->GetAnimInstance()->Montage_Play(WeaponDeadMontage);
	}
}

void ANMAICharacter::CalculateOldHP()
{
	if (HPLerpTimeline->IsPlaying())
	{
		HPLerpTimeline->Stop();
	}

	HPLerpTimeline->PlayFromStart();
}

void ANMAICharacter::OldHPLerp(float value)
{
	OldHP = FMath::Lerp(SavedOldHP, HP, value);
}

void ANMAICharacter::HitKnockBack()
{
	TSharedPtr<FRootMotionSource_MoveToForce> MoveToForce = MakeShareable(new FRootMotionSource_MoveToForce());
	MoveToForce->InstanceName = TEXT("KnockBack");
	MoveToForce->AccumulateMode = ERootMotionAccumulateMode::Override;
	MoveToForce->Priority = 1;

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (GetActorForwardVector() * -20.0f);
	
	MoveToForce->StartLocation = StartLocation;
	MoveToForce->TargetLocation = EndLocation;
	MoveToForce->Duration = 0.05f;

	uint16 KnockBackID = GetCharacterMovement()->ApplyRootMotionSource(MoveToForce);

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([this, KnockBackID]
	{
		GetCharacterMovement()->RemoveRootMotionSourceByID(KnockBackID);
	}), 0.05f, false, -1);
}
