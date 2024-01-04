// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Slot.generated.h"


UENUM(BlueprintType)
enum class ESlotType : uint8
{
	SLOT_NONE UMETA(DisplayName = "None"),
	SLOT_ITEM UMETA(DisplayName = "Item"),
	SLOT_SKILL UMETA(DisplayName = "Skill")
};

class UImage;
class UTextBlock;

UCLASS()
class GAMEPROJECT_API USlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMax = 31, UIMin = -1))
		int SlotNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Count;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESlotType> SlotType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Img;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text;

public:
	virtual bool Initialize() override;
};
