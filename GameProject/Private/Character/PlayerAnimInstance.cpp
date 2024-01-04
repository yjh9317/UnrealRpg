#include "Character/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	GroundSpeed = 100.f;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Player)
	{
		PlayerMovement = Player->GetCharacterMovement();
	}

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovement->Velocity);
		bIsInAir = PlayerMovement->IsFalling();
		CharacterState = Player->GetCharacterState();
		ActionState = Player->GetActionState();
		DeathPose = Player->GetDeathPose();
	}
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	if (Player)
	{
		if (Player->IsNextAttack())
		{
			const  TArray<FName>& AttackSectionNames = Player->GetAttackSection();
			int CurrentAttackIndex = Player->GetCurrentAttackIndex();

			if (AttackSectionNames.Num() > CurrentAttackIndex + 1
				&& Player->HasEnoughStamina())
			{
				Player->UseAttackStaminar();
				Player->SetCurrentAttackIndex(CurrentAttackIndex + 1);
				Player->PlayAnimMontage(Player->AttackMontage, 1, AttackSectionNames[CurrentAttackIndex + 1]);
			}
		}
		else
		{
			Player->AttackEnd();
			Player->SetCurrentAttackIndex(0);
		}
		Player->SetNextAttack(false);
	}
}

void UPlayerAnimInstance::AnimNotify_Arm()
{
	if (Player)
	{
		Player->AttachWeaponToHand();
	}
}

void UPlayerAnimInstance::AnimNotify_DisArm()
{
	if (Player)
	{
		Player->AttachWeaponToBack();
	}
}

void UPlayerAnimInstance::AnimNotify_FinishEquipping()
{
	if (Player)
	{
		Player->FinishEquipping();
	}
}

void UPlayerAnimInstance::AnimNotify_EnableCollision()
{
	if (Player)
	{
		Player->SetWeaponCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UPlayerAnimInstance::AnimNotify_DisableCollision()
{
	if (Player)
	{
		Player->SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UPlayerAnimInstance::AnimNotify_HitReactEnd()
{
	if (Player)
	{
		Player->HitReactEnd();
	}
}

void UPlayerAnimInstance::AnimNotify_AttackCheck()
{
	if (Player)
	{
		int CurIndex = Player->GetCurrentAttackIndex();

		if (CurIndex > 0)
			Player->SetCurrentAttackIndex(0);
	}
}
