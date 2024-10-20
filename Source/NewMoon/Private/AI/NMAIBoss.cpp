// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMAIBoss.h"

#include "AI/NMAIBossController.h"
#include "AI/NMAIInfo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/NMPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Physics/NMCollision.h"

ANMAIBoss::ANMAIBoss()
{
	AIControllerClass = ANMAIBossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCapsuleHalfHeight(92.0f);
	GetCapsuleComponent()->SetCapsuleRadius(35.0f);
	GetCapsuleComponent()->SetCollisionProfileName(NM_CPROFILE_ENEMY);
	
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -92.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->bReceivesDecals = false;

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Mutant/ABP_Mutant.ABP_Mutant_C'"));
	if (AnimClassRef.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimClassRef.Class);
	}

	// Set Stat
	MaxHP = 500.0f;
	AttackDamage = 20.0f;

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Mutant/Animations/AM_Creature_Death.AM_Creature_Death'"));
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}



	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MeleeAttackMontagesRef1(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Mutant/Animations/AM_Melee_01.AM_Melee_01'"));
	if (MeleeAttackMontagesRef1.Succeeded())
	{
		MeleeAttackMontages.Add(MeleeAttackMontagesRef1.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MeleeAttackMontagesRef2(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Mutant/Animations/AM_Melee_02.AM_Melee_02'"));
	if (MeleeAttackMontagesRef2.Succeeded())
	{
		MeleeAttackMontages.Add(MeleeAttackMontagesRef2.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MagicAttackMontagesRef1(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Mutant/Animations/AM_Magic_01.AM_Magic_01'"));
	if (MagicAttackMontagesRef1.Succeeded())
	{
		MagicAttackMontages.Add(MagicAttackMontagesRef1.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MagicAttackMontagesRef2(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Mutant/Animations/AM_Magic_02.AM_Magic_02'"));
	if (MagicAttackMontagesRef2.Succeeded())
	{
		MagicAttackMontages.Add(MagicAttackMontagesRef2.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		JumpAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Mutant/Animations/AM_JumpAttack.AM_JumpAttack'"));
	if (JumpAttackMontageRef.Succeeded())
	{
		JumpAttackMontage = JumpAttackMontageRef.Object;
	}
}

void ANMAIBoss::InitBoss()
{
	HP = MaxHP;
	OldHP = HP;
	SavedOldHP = OldHP;
}

float ANMAIBoss::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                            class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HP = FMath::Clamp(HP - Damage, 0.0f, MaxHP);
	SavedOldHP = OldHP;
	CalculateOldHP();

	if (HP <= KINDA_SMALL_NUMBER)
	{
		ClearStage();
		
		SetDead();
	}

	return Damage;
}

void ANMAIBoss::BeginPlay()
{
	Super::BeginPlay();

	HP = MaxHP;
	OldHP = HP;
	SavedOldHP = OldHP;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ANMAIBoss::Attack()
{
	ANMAIBossController* BossController = Cast<ANMAIBossController>(GetController());
	if (IsValid(BossController))
	{
		float TargetDistance = BossController->GetBlackboardComponent()->GetValueAsFloat(BBKEY_TRACEDISTANCE);

		if (TargetDistance <= 400.0f)
		{
			int32 RandIdx = FMath::RandRange(0, 1);
			GetMesh()->GetAnimInstance()->Montage_Play(MeleeAttackMontages[RandIdx]);
		}
		else if (TargetDistance <= 800.0f)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(JumpAttackMontage);
		}
		else
		{
			int32 RandIdx = FMath::RandRange(0, 1);
			GetMesh()->GetAnimInstance()->Montage_Play(MagicAttackMontages[RandIdx]);
		}
	}
}

void ANMAIBoss::MeleeAttack()
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
		FCollisionShape::MakeSphere(100.0f),
		Params
	);

	if (bResult)
	{
		FDamageEvent DamageEvent;
		for (auto HitResult : HitResults)
		{
			HitResult.GetActor()->TakeDamage(AttackDamage * 3, DamageEvent, GetController(), this);
			
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				MeleeParticle,
				HitResult.ImpactPoint);

		}
	}
}

void ANMAIBoss::LeftMagicAttack()
{
	if (LeftMagicProjectileClass == nullptr)
	{
		return;
	}

	FTransform Transform = GetActorTransform();
	Transform.SetLocation(GetMesh()->GetSocketLocation(TEXT("hand_l")) + GetActorForwardVector() * 50);
	ANMAIBossController* NMAIController = Cast<ANMAIBossController>(GetController());
	if (IsValid(NMAIController))
	{
		AActor* Target = Cast<AActor>(NMAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_Target));
		if (IsValid(Target))
		{
			FVector Dir = Target->GetActorLocation() - Transform.GetLocation();
			Transform.SetRotation(FQuat(Dir.GetSafeNormal().Rotation()));
		}
	}
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(
		LeftMagicProjectileClass,
		Transform,
		Params
	);
}

void ANMAIBoss::RightMagicAttack()
{
	if (RightMagicProjectileClass == nullptr)
	{
		return;
	}

	FTransform Transform = GetActorTransform();
	Transform.SetLocation(GetMesh()->GetSocketLocation(TEXT("hand_r")) + GetActorForwardVector() * 50);
	ANMAIBossController* NMAIController = Cast<ANMAIBossController>(GetController());
	if (IsValid(NMAIController))
	{
		AActor* Target = Cast<AActor>(NMAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_Target));
		if (IsValid(Target))
		{
			FVector Dir = Target->GetActorLocation() - Transform.GetLocation();
			Transform.SetRotation(FQuat(Dir.GetSafeNormal().Rotation()));
		}
	}
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(
		RightMagicProjectileClass,
		Transform,
		Params
	);
}

void ANMAIBoss::SetDead()
{
	ANMAIBossController* NMAIController = Cast<ANMAIBossController>(GetController());
	if (NMAIController)
	{
		NMAIController->StopAI();
	}

	SetActorEnableCollision(false);

	ClearStage();
}

void ANMAIBoss::CalculateOldHP()
{
	if (HPLerpTimeline->IsPlaying())
	{
		HPLerpTimeline->Stop();
	}

	HPLerpTimeline->PlayFromStart();
}

void ANMAIBoss::OldHPLerp(float value)
{
	OldHP = FMath::Lerp(SavedOldHP, HP, value);
}

void ANMAIBoss::JumpAttack()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		JumpAttackLocation,
		FQuat::Identity,
		NM_CCHANNEL_ENEMYATTACK,
		FCollisionShape::MakeSphere(800.0f),
		CollisionQueryParams
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			ANMPlayerCharacter* PlayerCharacter = Cast<ANMPlayerCharacter>(OverlapResult.GetActor());
			
			FDamageEvent DamageEvent;
			if (IsValid(PlayerCharacter))
			{
				PlayerCharacter->TakeDamage(AttackDamage * 5, DamageEvent, GetController(), this);
				PlayerCharacter->HitKnockBack();

				
				// DrawDebugSphere(GetWorld(), JumpAttackLocation, 800.0f, 16, FColor::Green, false, 0.2f);
			}
		}
	}
	
	// DrawDebugSphere(GetWorld(), JumpAttackLocation, 800.0f, 16, FColor::Red, false, 0.2f);
}
