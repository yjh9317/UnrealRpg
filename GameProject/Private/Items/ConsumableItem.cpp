// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ConsumableItem.h"

#include "Character/PlayerCharacter.h"
#include "Components/AttributeComponent.h"

AConsumableItem::AConsumableItem()
	:HealthAmount(20.f)
{
}

void AConsumableItem::Use()
{
	// HP Example
	APlayerCharacter* Player = Cast<APlayerCharacter>(ItemOwner);
	if(Player)
	{
		UAttributeComponent* Attributes = Player->GetAttribute();
		Attributes->RestoreHealth(HealthAmount);
		float HP = Attributes->GetHealthPercent();
		Player->SetHPPercent(HP);
	}
	Super::Use();
}
