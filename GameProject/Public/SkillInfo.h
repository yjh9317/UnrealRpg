#pragma once

#include "Engine/Texture2D.h"
#include "SkillInfo.generated.h"

UENUM(BlueprintType)
enum ESkillType
{
	EST_Melee UMETA(DisplayName = "Melee"),
	EST_Scope UMETA(DisplayName = "Scope")
};

USTRUCT(BlueprintType)
struct FSkillData
{
public:
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 RequireLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentSkillLevel;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MaxSkillLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SkillCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Scale;
};

