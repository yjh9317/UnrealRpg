// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_IsNextAttack.h"
#include "Character/PlayerCharacter.h"
#include "Character/PlayerAnimInstance.h"

void UAnimNotifyState_IsNextAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	AnimInstance = Cast<UPlayerAnimInstance>(MeshComp->GetAnimInstance());

	if (Player)
	{
		PlayerController = Cast<APlayerController>(Player->GetController());
	}
}

void UAnimNotifyState_IsNextAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (Player)
	{
		if (AnimInstance->Montage_IsPlaying(AnimMontage))
		{
			if (PlayerController->IsInputKeyDown(EKeys::LeftMouseButton)
				&& !Player->IsNextAttack())
			{
				Player->SetNextAttack(true);
			}
		}
	}
}

void UAnimNotifyState_IsNextAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

}
