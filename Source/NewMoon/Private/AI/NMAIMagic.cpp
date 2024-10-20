// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NMAIMagic.h"

ANMAIMagic::ANMAIMagic()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		AttackMontageRef1(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Magic_01.AM_Magic_01'"));
	if (AttackMontageRef1.Succeeded())
	{
		AttackMontages.Add(AttackMontageRef1.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		AttackMontageRef2(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Magic_02.AM_Magic_02'"));
	if (AttackMontageRef2.Succeeded())
	{
		AttackMontages.Add(AttackMontageRef2.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		AttackMontageRef3(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Magic_03.AM_Magic_03'"));
	if (AttackMontageRef3.Succeeded())
	{
		AttackMontages.Add(AttackMontageRef3.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WeaponAttackMontageRef1(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Weapon_Magic_01.AM_Weapon_Magic_01'"));
	if (WeaponAttackMontageRef1.Succeeded())
	{
		WeaponAttackMontages.Add(WeaponAttackMontageRef1.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WeaponAttackMontageRef2(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Weapon_Magic_02.AM_Weapon_Magic_02'"));
	if (WeaponAttackMontageRef2.Succeeded())
	{
		WeaponAttackMontages.Add(WeaponAttackMontageRef2.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WeaponAttackMontageRef3(TEXT("/Script/Engine.AnimMontage'/Game/Goblin/Animation/AM_Weapon_Magic_03.AM_Weapon_Magic_03'"));
	if (WeaponAttackMontageRef3.Succeeded())
	{
		WeaponAttackMontages.Add(WeaponAttackMontageRef3.Object);
	}

	AttackRange = 500.0f;
}

void ANMAIMagic::Attack()
{
	Super::Attack();
	
	// int32 RandNum = FMath::RandRange(0, AttackMontages.Num() - 1);
	int32 RandNum = 0;

	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontages[RandNum]);
	WeaponMesh->GetAnimInstance()->Montage_Play(WeaponAttackMontages[RandNum]);
	if (IsValid(ShieldMesh->GetSkeletalMeshAsset()))
	{
		ShieldMesh->GetAnimInstance()->Montage_Play(WeaponAttackMontages[RandNum]);
	}
}
