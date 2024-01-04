// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Items/ItemInfo.h"
#include "ArmourSlot.generated.h"

UCLASS()
class GAMEPROJECT_API UArmourSlot : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	FItemData EquipmentData;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class USizeBox* ArmourSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UBorder* ArmourBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UButton* ArmourButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> SlotInventoryClass;



public:
	UArmourSlot(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void ButtonPress();

	virtual void NativeConstruct() override;
};
