// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_IsNextAttack.generated.h"

class UAnimMontage;

UCLASS()
class GAMEPROJECT_API UAnimNotifyState_IsNextAttack : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UPlayerAnimInstance* AnimInstance;

	UPROPERTY()
	class APlayerCharacter* Player;

	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AnimMontage;

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
