// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/NMProjectile.h"

#include "EngineUtils.h"
#include "AI/NMAICharacter.h"
#include "Components/InterpToMovementComponent.h"

ANMProjectile::ANMProjectile()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (MeshRef.Succeeded())
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);

	InterpToMovementComponent = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("InterpToMovementComponent"));
	// InterpToMovementComponent->Deactivate();
	InterpToMovementComponent->Duration = 1.0f;

}

void ANMProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator::ZeroRotator);
	
	OriginLocation = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(OriginLocation).ToString());
		

	FInterpCurve<FVector> VectorCurve;

	FInterpCurvePoint<FVector> StartPoint;
	StartPoint.InVal = 0.0f;
	StartPoint.OutVal = OriginLocation;
	StartPoint.InterpMode = CIM_CurveAuto;
	// StartPoint.ArriveTangent = FVector(50.0f, 50.0f, 50.0f);
	// StartPoint.LeaveTangent = FVector(50.0f, 50.0f, 50.0f);

	FInterpCurvePoint<FVector> EndPoint;
	EndPoint.InVal = 1.0f;
	for (auto Actor : TActorRange<ANMAICharacter>(GetWorld()))
	{
		EndPoint.OutVal = Actor->GetActorLocation();
		break;
	}
	EndPoint.InterpMode = CIM_CurveAuto;
	// EndPoint.ArriveTangent = FVector(50.0f, 50.0f, 50.0f);
	// EndPoint.LeaveTangent = FVector(50.0f, 50.0f, 50.0f);
	
	FInterpCurvePoint<FVector> MiddlePoint;
	MiddlePoint.InVal = 0.5f;
	MiddlePoint.OutVal = (OriginLocation + EndPoint.OutVal) / 2;
	MiddlePoint.OutVal.Z = 500.0f;
	MiddlePoint.InterpMode = CIM_CurveAuto;
	// StartPoint.ArriveTangent = FVector(50.0f, 50.0f, 50.0f);
	// StartPoint.LeaveTangent = FVector(50.0f, 50.0f, 50.0f);

	VectorCurve.Points.Add(StartPoint);
	VectorCurve.Points.Add(MiddlePoint);
	VectorCurve.Points.Add(EndPoint);

	for (int i = 0; i < 11; ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(VectorCurve.Eval(0.1f * i)).ToString());
		InterpToMovementComponent->AddControlPointPosition(VectorCurve.Eval(0.1f * i), false);
	}

	InterpToMovementComponent->FinaliseControlPoints();
	InterpToMovementComponent->Activate();
}


