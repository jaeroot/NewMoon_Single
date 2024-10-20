// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NMPlayerCharacter.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "AI/NMAICharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameData/NMGameSingleton.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/NMInputData.h"
#include "Interface/NMInteractionInterface.h"
#include "Inventory/NMInventoryComponent.h"
#include "Item/NMItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/NMCollision.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "AI/NMAIBoss.h"
#include "Animation/NMPlayerAnimInstance.h"

ANMPlayerCharacter::ANMPlayerCharacter()
{
	// Set Child Skeletal Mesh
	ChildMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChildMesh"));
	ChildMesh->SetupAttachment(GetMesh());
	ChildMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetChildMesh()->bReceivesDecals = false;

	// Set Inventory
	InventoryComponent = CreateDefaultSubobject<UNMInventoryComponent>(TEXT("Inventory"));

	// Set Interaction
	bInteract = true;

	// Set Stat
	MaxHP = 1000.0f;
	HP = MaxHP;
	OldHP = HP;
	SavedOldHP = OldHP;
	AttackDamage = 10.0f;
	bIsDead = false;

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


	// Weapon
	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SwordMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Weapon/Mesh/Weapons/Weapons_Kit/SM_Sword.SM_Sword'"));
	if (SwordMeshRef.Succeeded())
	{
		Sword->SetStaticMesh(SwordMeshRef.Object);
	}
	Sword->SetupAttachment(GetChildMesh(), TEXT("weapon_r_swordandshield"));
	Sword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sword->bReceivesDecals = false;
	
	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ShieldMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Weapon/Mesh/Weapons/Weapons_Kit/SM_Shield.SM_Shield'"));
	if (ShieldMeshRef.Succeeded())
	{
		Shield->SetStaticMesh(ShieldMeshRef.Object);
	}
	Shield->SetupAttachment(GetChildMesh(), TEXT("weapon_l_shield"));
	Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Shield->bReceivesDecals = false;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(Sword);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		LinkAnimationClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Animations/LinkedLayers/ABP_SwordAndShieldAnimLayers.ABP_SwordAndShieldAnimLayers_C'"));
	if (LinkAnimationClassRef.Succeeded())
	{
		LinkAnimation = LinkAnimationClassRef.Class;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Animations/SwordAndShield/Attack/AM_SwordAndShield_Hit.AM_SwordAndShield_Hit'"));
	if (HitMontageRef.Succeeded())
	{
		HitMontage = HitMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Animations/SwordAndShield/Attack/AM_SwordAndShield_Attack.AM_SwordAndShield_Attack'"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		KickMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Animations/Actions/Kick/AM_Kick.AM_Kick'"));
	if (KickMontageRef.Succeeded())
	{
		KickMontage = KickMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		HitImpactRef(TEXT("/Script/Engine.ParticleSystem'/Game/Paragon/Kallari/FX/Rogue/P_Melee_SucessfulImpact_Rogue.P_Melee_SucessfulImpact_Rogue'"));
	if (HitImpactRef.Succeeded())
	{
		HitImpact = HitImpactRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		LineFXSystemRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/FX/NS_DashLines.NS_DashLines'"));
	if (LineFXSystemRef.Succeeded())
	{
		LineFXSystem = LineFXSystemRef.Object;
	}
	LineFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LineFXComponent"));
	LineFXComponent->SetupAttachment(CameraComponent);
	LineFXComponent->SetRelativeLocation(FVector(300.0f, 0.0f, 0.0f));
	LineFXComponent->SetAsset(LineFXSystem);
	LineFXComponent->bAutoActivate = false;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase>
		CameraShakeBaseRef(TEXT("/Script/Engine.Blueprint'/Game/FX/BP_DefaultCameraShake.BP_DefaultCameraShake_C'"));
	if (CameraShakeBaseRef.Succeeded())
	{
		CameraShakeBase = CameraShakeBaseRef.Class;
	}

	bCanKick = false;
	bCombo = false;
	bKickCombo = false;


	// Guard
	bCanGuard = false;
	bIsGuard = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GuardMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Animations/SwordAndShield/Attack/AM_SwordAndShield_Guard_Loop.AM_SwordAndShield_Guard_Loop'"));
	if (GuardMontageRef.Succeeded())
	{
		GuardMontage = GuardMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GuardHitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Animations/SwordAndShield/Attack/AM_SwordAndShield_Guard_Hit.AM_SwordAndShield_Guard_Hit'"));
	if (GuardHitMontageRef.Succeeded())
	{
		GuardHitMontage = GuardHitMontageRef.Object;
	}


	// FireBall
	bCanFireBall = false;

	static ConstructorHelpers::FClassFinder<AActor>
		FireBallClassRef(TEXT("/Script/Engine.Blueprint'/Game/FX/BP_PlayerFireBall.BP_PlayerFireBall_C'"));
	if (FireBallClassRef.Succeeded())
	{
		FireballClass = FireBallClassRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FireBallMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Animations/SwordAndShield/Attack/AM_SwordAndShield_Magic.AM_SwordAndShield_Magic'"));
	if (FireBallMontageRef.Succeeded())
	{
		FireBallMontage = FireBallMontageRef.Object;
	}

	bIsBossLevel = false;


	ResurrectLocation = FVector(-5920.0f, -266.44342f, 202.000671f);
	ResurrectRotation = FRotator(0.0f, 0.0f, 0.0f);
}

void ANMPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set Child Skeletal Mesh
	FNMHeroData HeroData = UNMGameSingleton::Get().GetHeroData(TEXT("UE5Manny"));
	if (HeroData.HeroMesh.IsPending())
	{
		HeroData.HeroMesh.LoadSynchronous();
	}
	GetChildMesh()->SetSkeletalMesh(HeroData.HeroMesh.Get());
	GetChildMesh()->SetAnimClass(HeroData.HeroAnim);

	// Set Parent Skeletal Mesh
	GetMesh()->SetVisibility(false);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
}

void ANMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set Enhanced Input
	if (IsLocallyControlled())
	{
		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add Combat IMC
			const UNMCombatInputData* CombatInputData = GetDefault<UNMCombatInputData>();
			SubSystem->AddMappingContext(CombatInputData->CombatInputMappingContext, 0);
		}

		PlayerController->SetShowMouseCursor(false);
		FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
	}


	GetMesh()->GetAnimInstance()->LinkAnimClassLayers(LinkAnimation);


	
}

void ANMPlayerCharacter::SetDead()
{
	bIsDead = true;
	if (UNMPlayerAnimInstance* NMAnimInstance = Cast<UNMPlayerAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		NMAnimInstance->bIsDead = bIsDead;
	}
	
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	SetActorEnableCollision(false);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]
		{
			ResurrectInit();
		}), 8.0f, false, -1
	);
}

void ANMPlayerCharacter::ResurrectInit()
{
	bIsDead = false;
	HP = MaxHP;
	OldHP = HP;
	SavedOldHP = OldHP;
	if (UNMPlayerAnimInstance* NMAnimInstance = Cast<UNMPlayerAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		NMAnimInstance->bIsDead = bIsDead;
	}
	
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	SetActorEnableCollision(true);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	if (bIsBossLevel)
	{
		// SetActorLocation(FVector(-42899.999991f, 15769.999981f, -22497.999983f));
		// SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		//
		// for (auto AIBoss : TActorRange<ANMAIBoss>(GetWorld()))
		// {
		// 	AIBoss->InitBoss();
		// }

		PlayerController->RestartLevel();
		
		return;
	}

	SetActorLocation(ResurrectLocation);
	SetActorRotation(ResurrectRotation);
}

void ANMPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bInteract && HasAuthority())
	{
		TraceForward();
	}
}

float ANMPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bIsGuard)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(GuardHitMontage);
		
		return Damage;
	}
	
	if (ANMAICharacter* AICharacter = Cast<ANMAICharacter>(DamageCauser))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Damaged By NMAICharacter"));
		if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		{
			PlayHitMontage(FMath::RandRange(1, 2));
		}
		else if (AICharacter->bIsBoss)
		{
			PlayHitMontage(FMath::RandRange(1, 2));
		}
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("Damaged By %s"), *DamageCauser->GetName());
		PlayHitMontage(FMath::RandRange(1, 2));
	}

	
	HP = FMath::Clamp(HP - Damage, 0.0f, MaxHP);
	SavedOldHP = OldHP;
	CalculateOldHP();

	if (HP <= KINDA_SMALL_NUMBER)
	{
		SetDead();
	}

	return Damage;
}

void ANMPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	// Bind Menu Input Actions
	const UNMMenuInputData* MenuInputData = GetDefault<UNMMenuInputData>();
	EnhancedInputComponent->BindAction(MenuInputData->InventoryAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::ToggleInventoryMenu);
	EnhancedInputComponent->BindAction(MenuInputData->SettingAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::ToggleSettingMenu);
	

	// Bind Combat Input Actions
	const UNMCombatInputData* CombatInputData = GetDefault<UNMCombatInputData>();
	EnhancedInputComponent->BindAction(CombatInputData->SprintAction, ETriggerEvent::Started, this, &ANMPlayerCharacter::SprintStart);
	EnhancedInputComponent->BindAction(CombatInputData->SprintAction, ETriggerEvent::Completed, this, &ANMPlayerCharacter::SprintStop);
	EnhancedInputComponent->BindAction(CombatInputData->DashAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::Dash);
	EnhancedInputComponent->BindAction(CombatInputData->AttackAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::AttackAction);
	EnhancedInputComponent->BindAction(CombatInputData->SpecialSkillAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::SpecialSkill);
	EnhancedInputComponent->BindAction(CombatInputData->NormalSkillAction1, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::NormalSkill1);
	EnhancedInputComponent->BindAction(CombatInputData->NormalSkillAction2, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::NormalSkill2);
	EnhancedInputComponent->BindAction(CombatInputData->UltimateSkillAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::UltimateSkill);
	EnhancedInputComponent->BindAction(CombatInputData->LockAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::ToggleLockOn);
	EnhancedInputComponent->BindAction(CombatInputData->ResurrectAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::Resurrect);
	EnhancedInputComponent->BindAction(CombatInputData->NoEquipAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::NoEquip);
	EnhancedInputComponent->BindAction(CombatInputData->InteractAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacter::Interaction);
}

void ANMPlayerCharacter::ToggleInventoryMenu()
{
	UE_LOG(LogNMCharacter, Log, TEXT("ToggleInventoryMenu"));
}

void ANMPlayerCharacter::ToggleSettingMenu()
{
	UE_LOG(LogNMCharacter, Log, TEXT("ToggleSettingMenu"));
}

void ANMPlayerCharacter::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = 900.0f;
}

void ANMPlayerCharacter::SprintStop()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ANMPlayerCharacter::Dash()
{
	// if (HasAuthority())
	// {
	// 	FTransform Transform = GetTransform();
	// 	Transform.SetLocation(GetActorLocation() + GetActorForwardVector() * 100);
	// 	ANMItemBase* TestItem = GetWorld()->SpawnActorDeferred<ANMItemBase>(ANMItemBase::StaticClass(), Transform);
	// 	if (TestItem)
	// 	{
	// 		TestItem->SetItem(TEXT("2001"));
	// 	}
	// 	TestItem->FinishSpawning(TestItem->GetTransform());
	// }
	UE_LOG(LogNMCharacter, Log, TEXT("Dash"));
}

void ANMPlayerCharacter::AttackAction()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
	{
		bCombo = true;
		bKickCombo = false;
	}
	else if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		bCombo = false;
		bKickCombo = false;
		AttackNum = 1;
		KickNum = 0;
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	}
}

void ANMPlayerCharacter::SpecialSkill()
{
	if (!bCanKick)
	{
		return;
	}
	
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(KickMontage))
	{
		bCombo = false;
		bKickCombo = true;
	}
	else if (AttackNum >= 1 && AttackNum < 3)
	{
		bCombo = false;
		bKickCombo = true;
		KickNum = AttackNum - 1;
	}
}

void ANMPlayerCharacter::NormalSkill1()
{
	if (bCanGuard)
	{
		if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		{
			bIsGuard = true;
			GetMesh()->GetAnimInstance()->Montage_Play(GuardMontage);

			FOnMontageBlendingOutStarted BlendingOutDelegate;
			BlendingOutDelegate.BindUObject(this, &ANMPlayerCharacter::GuardEnd);
			GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendingOutDelegate, GuardMontage);
		}
	}
}

void ANMPlayerCharacter::NormalSkill2()
{
	if (bCanFireBall)
	{
		if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(FireBallMontage);
		}
	}
}

void ANMPlayerCharacter::UltimateSkill()
{
	UE_LOG(LogNMCharacter, Log, TEXT("UltimateSkill"));
}

void ANMPlayerCharacter::ToggleLockOn()
{
	UE_LOG(LogNMCharacter, Log, TEXT("ToggleLockOn"));
}

void ANMPlayerCharacter::Resurrect()
{
	UE_LOG(LogNMCharacter, Log, TEXT("Resurrect"));
}

void ANMPlayerCharacter::NoEquip()
{
	UE_LOG(LogNMCharacter, Log, TEXT("NoEquip"));
}

void ANMPlayerCharacter::Interaction()
{
	ServerInteraction();
}

void ANMPlayerCharacter::Attack()
{
	UMeshComponent* Weapon = Sword;

	FVector StartLocation;
	FVector EndLocation;

	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
	{
		if (IsValid(Weapon))
		{
			StartLocation = Weapon->GetSocketLocation(TEXT("Weapon_Tip"));
			EndLocation = Weapon->GetSocketLocation(TEXT("Weapon_Base"));
		}
		else
		{
			StartLocation = GetMesh()->GetSocketLocation(TEXT("Weapon_Tip"));
			EndLocation = GetMesh()->GetSocketLocation(TEXT("Weapon_Base"));
		}
	}
	else if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(KickMontage))
	{
		StartLocation = GetMesh()->GetSocketLocation(TEXT("foot_r_base"));
		EndLocation = GetMesh()->GetSocketLocation(TEXT("calf_r_socket"));
	}
	
	
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray<FHitResult> HitResults;
	bool bHitResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		NM_CCHANNEL_PLAYERATTACK,
		FCollisionShape::MakeSphere(20.0f),
		Params
	);

	bool bDrawDebug = false;
	if (bHitResult)
	{
		if (HitResultSet.IsEmpty())
		{
			GetMesh()->GetAnimInstance()->Montage_SetPlayRate(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage(), 0.03f);
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(
				Handle,
				FTimerDelegate::CreateLambda([&]
				{
					GetMesh()->GetAnimInstance()->Montage_SetPlayRate(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage(), 1.5f);
				}),
				0.07f,
				false,
				-1
			);
		}
		
		for (auto HitResult : HitResults)
		{
			if (HitResultSet.Contains(HitResult.GetActor()))
			{
				continue;
			}

			float CurrentDamage = AttackDamage * AttackNum;

			// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("Weapon Hit : %s"), *HitResult.GetActor()->GetName()));
			HitResult.GetActor()->TakeDamage(AttackDamage * (AttackNum + KickNum), FDamageEvent(), GetController(), this);
			if (ANMAICharacter* AICharacter = Cast<ANMAICharacter>(HitResult.GetActor()))
			{
				if (!AICharacter->GetIsBoss())
				{
					AICharacter->HitKnockBack();
				}
			}
			
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				HitImpact,
				HitResult.ImpactPoint);
			
			HitResultSet.Add(HitResult.GetActor());
			bDrawDebug = true;
		}
	}


// #if ENABLE_DRAW_DEBUG
// 	FColor Color = bDrawDebug ? FColor::Green : FColor::Red;
// 	DrawDebugCapsule(
// 		GetWorld(),
// 		(StartLocation + EndLocation) / 2,
// 		FVector::Distance(StartLocation, EndLocation) / 2,
// 		20.0f,
// 		FRotationMatrix::MakeFromZ(EndLocation - StartLocation).ToQuat(),
// 		Color,
// 		false,
// 		2.0f
// 	);
// #endif
}

void ANMPlayerCharacter::AttackEnd()
{
	HitResultSet.Empty();
}

void ANMPlayerCharacter::AttackCombo()
{
	if (bCombo)
	{
		bCombo = false;
		AttackNum++;

		FName NextSection = *FString::Printf(TEXT("Attack%d"), AttackNum);
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(NextSection, AttackMontage);
	}
	else if (bKickCombo)
	{
		bKickCombo = false;
		KickNum++;

		FName NextSection = *FString::Printf(TEXT("Kick%d"), KickNum);
		GetMesh()->GetAnimInstance()->Montage_Play(KickMontage);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(NextSection, KickMontage);
	}
}

void ANMPlayerCharacter::ServerInteraction_Implementation()
{
	if (FocusedInteractionActor != nullptr)
	{
		FocusedInteractionActor->Interaction(this);
	}
}

void ANMPlayerCharacter::TraceForward()
{	
	FHitResult HitResult;
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + CameraComponent->GetForwardVector() * 500;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bHitResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		NM_CCHANNEL_INTERACTION,
		Params
	);

	if (bHitResult)
	{
		INMInteractionInterface* HitActor = Cast<INMInteractionInterface>(HitResult.GetActor());
		if (HitActor != nullptr)
		{
			FocusedInteractionActor = HitActor;
		}
		else
		{
			FocusedInteractionActor = nullptr;
		}
	}

// #if ENABLE_DRAW_DEBUG
// 	FColor Color = bHitResult ? FColor::Green : FColor::Red;
// 	DrawDebugLine(
// 		GetWorld(),
// 		Start,
// 		End,
// 		Color,
// 		false,
// 		2.0f
// 	);
// #endif
	
}

void ANMPlayerCharacter::InteractItem(class ANMItemBase* Item)
{
	if (HasAuthority())
	{
		if (IsValid(Item))
		{
			int32 Result = InventoryComponent->AddInventoryItem(Item->GetItemData(), Item->GetCurrentCount());

			if (Result == 0)
			{
				Item->Destroy();
			}
			else
			{
				Item->SetItemCount(Result);
			}
		}
	}
}

void ANMPlayerCharacter::CalculateOldHP()
{
	if (HPLerpTimeline->IsPlaying())
	{
		HPLerpTimeline->Stop();
	}

	HPLerpTimeline->PlayFromStart();
}

void ANMPlayerCharacter::OldHPLerp(float value)
{
	OldHP = FMath::Lerp(SavedOldHP, HP, value);
}

void ANMPlayerCharacter::PlayHitMontage(int32 MontageNum)
{
	FName MontageSection = *FString::Printf(TEXT("Hit%d"), MontageNum);
	GetMesh()->GetAnimInstance()->Montage_Play(HitMontage, 1.5f);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(MontageSection, HitMontage);
}

void ANMPlayerCharacter::ActivateLineFX()
{
	LineFXComponent->Activate();
	
	FTimerHandle Handle;
	TWeakObjectPtr<UNiagaraComponent> WeakLineFX(LineFXComponent);
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]
		{
			if (WeakLineFX.IsValid())
			{
				WeakLineFX->Deactivate();
			}
		}), 1.0f, false, -1
	);
}

void ANMPlayerCharacter::StartCameraShake()
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (IsValid(PlayerController))
	{
		PlayerController->ClientStartCameraShake(CameraShakeBase);
	}
}

void ANMPlayerCharacter::GuardEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsGuard = false;
}

void ANMPlayerCharacter::FireBallAttack()
{
	if (FireballClass == nullptr)
	{
		return;
	}

	FTransform Transform = GetActorTransform();
	Transform.SetLocation(GetMesh()->GetSocketLocation(TEXT("hand_l")) + GetActorForwardVector() * 20);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(
		FireballClass,
		Transform,
		Params
	);
}

void ANMPlayerCharacter::HitKnockBack()
{
	TSharedPtr<FRootMotionSource_MoveToForce> MoveToForce = MakeShareable(new FRootMotionSource_MoveToForce());
	MoveToForce->InstanceName = TEXT("KnockBack");
	MoveToForce->AccumulateMode = ERootMotionAccumulateMode::Override;
	MoveToForce->Priority = 1;

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (GetActorForwardVector() * -50.0f);
	
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

