// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemys/PaladinAnimInstance.h"
#include "Enemys/Enemy.h"
#include "Items/Weapons/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"

void UPaladinAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	
	Enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (Enemy)
	{
		Movement = Enemy->GetCharacterMovement();
	}
}

void UPaladinAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UPaladinAnimInstance::AnimNotify_AttackEnd()
{
	if (Enemy)
	{
		Enemy->AttackEnd();
	}
}

void UPaladinAnimInstance::AnimNotify_EnableBoxCollision()
{
	if (Enemy)
	{
		Enemy->SetWeaponCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UPaladinAnimInstance::AnimNotify_DisableBoxCollision()
{
	if (Enemy)
	{
		Enemy->SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//void UPaladinAnimInstance::AnimNotify_UpdateTranslationTarget()
//{
//	if (Enemy)
//	{
//		Enemy->GetTranslationWarpTarget();
//	}
//}
//
//void UPaladinAnimInstance::AnimNotify_UpdateRotationTarget()
//{
//	if (Enemy)
//	{
//		Enemy->GetRotationWarpTarget();
//	}
//}
