// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PaladinAnimInstance.generated.h"

/**
 *
 */

class AEnemy;
class UCharacterMovementComponent;

UCLASS()
class GAMEPROJECT_API UPaladinAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	// About Player Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AEnemy* Enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCharacterMovementComponent* Movement;

	class AWeapon* Weapon;
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_EnableBoxCollision();

	UFUNCTION()
	void AnimNotify_DisableBoxCollision();

	//UFUNCTION()
	//void AnimNotify_UpdateTranslationTarget();
	//UFUNCTION()
	//void AnimNotify_UpdateRotationTarget();
};
