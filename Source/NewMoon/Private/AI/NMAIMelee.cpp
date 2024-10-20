// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMAIMelee.h"

ANMAIMelee::ANMAIMelee()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		AttackMontageRef1(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Attack.AM_Attack'"));
	if (AttackMontageRef1.Succeeded())
	{
		AttackMontages.Add(AttackMontageRef1.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		AttackMontageRef2(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Attack_Slash.AM_Attack_Slash'"));
	if (AttackMontageRef2.Succeeded())
	{
		AttackMontages.Add(AttackMontageRef2.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WeaponAttackMontageRef1(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Weapon_Attack.AM_Weapon_Attack'"));
	if (WeaponAttackMontageRef1.Succeeded())
	{
		WeaponAttackMontages.Add(WeaponAttackMontageRef1.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WeaponAttackMontageRef2(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Weapon_Attack_Slash.AM_Weapon_Attack_Slash'"));
	if (WeaponAttackMontageRef2.Succeeded())
	{
		WeaponAttackMontages.Add(WeaponAttackMontageRef2.Object);
	}

	AttackRange = 150.0f;
}

void ANMAIMelee::Attack()
{
	Super::Attack();

	int32 RandNum = FMath::RandRange(0, AttackMontages.Num() - 1);

	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontages[RandNum], 0.75f);
	WeaponMesh->GetAnimInstance()->Montage_Play(WeaponAttackMontages[RandNum], 0.75f);
	if (IsValid(ShieldMesh->GetSkeletalMeshAsset()))
	{
		ShieldMesh->GetAnimInstance()->Montage_Play(WeaponAttackMontages[RandNum], 0.75f);
	}
}
