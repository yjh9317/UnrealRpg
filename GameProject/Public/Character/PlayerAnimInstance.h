// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "PlayerAnimInstance.generated.h"


class APlayerCharacter;
class UCharacterMovementComponent;

UCLASS()
class GAMEPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();

	// virtual void NativeBeginPlay() override;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	// About Player Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerCharacter* Player;  

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCharacterMovementComponent* PlayerMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | States")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathPose> DeathPose;
	/*
	 * Animation Notify Functions
	 */
	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_Arm();
	UFUNCTION()
	void AnimNotify_DisArm();
	UFUNCTION()
	void AnimNotify_FinishEquipping();
	UFUNCTION()
	void AnimNotify_EnableCollision();
	UFUNCTION()
	void AnimNotify_DisableCollision();
	UFUNCTION()
	void AnimNotify_HitReactEnd();
	UFUNCTION()
	void AnimNotify_AttackCheck();

};
