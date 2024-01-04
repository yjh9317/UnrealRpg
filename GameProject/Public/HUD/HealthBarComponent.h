// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */

class UHealthBar;

UCLASS()
class GAMEPROJECT_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	UHealthBar* HealthBarWidget;
public:
	void SetHealthPercent(float Percent);
};
