// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "ConsumableItem.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API AConsumableItem : public AItem
{
	GENERATED_BODY()

public:
	AConsumableItem();
	virtual void Use() override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess="true"))
	float HealthAmount;
};
