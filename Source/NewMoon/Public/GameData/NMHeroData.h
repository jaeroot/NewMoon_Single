
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NMHeroData.generated.h"


USTRUCT(BlueprintType)
struct FNMHeroStat : public FTableRowBase
{
    GENERATED_BODY()

public:
    FNMHeroStat()
        : MaxHP(0.0f), MaxStamina(0.0f), AttackDamage(0.0f), Defense(0.0f), AttackSpeed(0.0f), MoveSpeed(0.0f)
    {
        
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float MaxStamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float AttackDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float Defense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float AttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float MoveSpeed;

    FNMHeroStat operator+(const FNMHeroStat& Other) const
    {
        FNMHeroStat Result;

        Result.MaxHP = this->MaxHP + Other.MaxHP;
        Result.MaxStamina = this->MaxStamina + Other.MaxStamina;
        Result.AttackDamage = this->AttackDamage + Other.AttackDamage;
        Result.Defense = this->Defense + Other.Defense;
        Result.AttackSpeed = this->AttackSpeed + Other.AttackSpeed;
        Result.MoveSpeed = this->MoveSpeed + Other.MoveSpeed;

        return Result;
    }
    
    FNMHeroStat operator-(const FNMHeroStat& Other) const
    {
        FNMHeroStat Result;

        Result.MaxHP = FMath::Clamp(this->MaxHP - Other.MaxHP, 0.0f, this->MaxHP);
        Result.MaxStamina = FMath::Clamp(this->MaxStamina - Other.MaxStamina, 0.0f, this->MaxStamina);
        Result.AttackDamage = FMath::Clamp(this->AttackDamage - Other.AttackDamage, 0.0f, this->AttackDamage);
        Result.Defense = FMath::Clamp(this->Defense - Other.Defense, 0.0f, this->Defense);
        Result.AttackSpeed = FMath::Clamp(this->AttackSpeed - Other.AttackSpeed, 0.0f, this->AttackSpeed);
        Result.MoveSpeed = FMath::Clamp(this->MoveSpeed - Other.MoveSpeed, 0.0f, this->MoveSpeed);

        return Result;
    }
    
};


USTRUCT(BlueprintType)
struct FNMHeroData : public FNMHeroStat
{
    GENERATED_BODY()

public:
    FNMHeroData()
        : HeroMesh(nullptr)
    {

    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroData)
    TSoftObjectPtr<USkeletalMesh> HeroMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroData)
    TSubclassOf<UAnimInstance> HeroAnim;
    
};


USTRUCT(BlueprintType)
struct FNMHeroLevelData : public FTableRowBase
{
    GENERATED_BODY()

public:
    FNMHeroLevelData()
        : ReqExp(0.0f)
    {

    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroLevelData)
    float ReqExp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroLevelData)
    FNMHeroStat MaxGrowthStat;
    
};
