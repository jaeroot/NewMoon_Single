// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NMPlayerCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/NMInputData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/NMCollision.h"

DEFINE_LOG_CATEGORY(LogNMCharacter);

ANMPlayerCharacterBase::ANMPlayerCharacterBase()
{
	// Set Capsule Component
	GetCapsuleComponent()->SetCollisionProfileName(NM_CPROFILE_PLAYER);
	GetCapsuleComponent()->SetCapsuleHalfHeight(95.0f);
	GetCapsuleComponent()->SetCapsuleRadius(35.0f);

	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMeshRef(TEXT("/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin"));
	if (SkeletalMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(SkeletalMeshRef.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -95.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->bReceivesDecals = false;

	// Set SpringArm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	SpringArmComponent->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 300.0f;

	// Set Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetRelativeLocation(FVector(0.0f, 75.0f, 0.0f));

	// Set Control Rotation
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	SpringArmComponent->bUsePawnControlRotation = true;
	CameraComponent->bUsePawnControlRotation = false;

	// Set Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	// Set Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimInstanceClassRef(TEXT("/Game/Characters/Animations/ABP_NMPlayer.ABP_NMPlayer_C"));
	if (AnimInstanceClassRef.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimInstanceClassRef.Class);
	}
}

void ANMPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Set Animation
	AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());

	// Set Enhanced Input
	if (IsLocallyControlled())
	{
		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add Menu IMC
			const UNMMenuInputData* MenuInputData = GetDefault<UNMMenuInputData>();
			SubSystem->AddMappingContext(MenuInputData->MenuInputMappingContext, 2);
			
			// Add Base Free Move Menu IMC
			const UNMFreeInputData* FreeInputData = GetDefault<UNMFreeInputData>();
			SubSystem->AddMappingContext(FreeInputData->FreeInputMappingContext, 1);
		}
	}
}

void ANMPlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind Base Free Move Input Actions
	const UNMFreeInputData* FreeInputData = GetDefault<UNMFreeInputData>();
	EnhancedInputComponent->BindAction(FreeInputData->FreeMoveAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacterBase::FreeMove);
	EnhancedInputComponent->BindAction(FreeInputData->FreeLookAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacterBase::FreeLook);
	EnhancedInputComponent->BindAction(FreeInputData->JumpAction, ETriggerEvent::Triggered, this, &ANMPlayerCharacterBase::JumpAction);

}

void ANMPlayerCharacterBase::FreeMove(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator ForwardRotation = FRotator(0.0, Rotation.Yaw, 0.0);
	const FRotator RightRotation = FRotator(0.0, Rotation.Yaw, Rotation.Roll);
	
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ForwardRotation);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(RightRotation);

	AddMovementInput(ForwardVector, MoveVector.X);
	AddMovementInput(RightVector, MoveVector.Y);
}

void ANMPlayerCharacterBase::FreeLook(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void ANMPlayerCharacterBase::JumpAction()
{
	if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		ACharacter::Jump();
	}
	
}
