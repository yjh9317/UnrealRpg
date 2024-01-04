// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

#include "Inventory/InventoryWidget.h"
#include "Components/InventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CHelper.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
	: CameraDist{300},
	bCameraLock{false}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,  ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = CameraDist;
	CameraBoom->bUsePawnControlRotation = true;
	
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Player"));
	

	if(const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext,0);
		}
	}

	InitializeSlashOverlay();
	InitializeInventoryWidget();
	
	AttackMontageSections.AddUnique(FName("Attack1"));
	AttackMontageSections.AddUnique(FName("Attack2"));
	AttackMontageSections.AddUnique(FName("Attack3"));
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Attributes)
	{
		Attributes->RegenStamina(DeltaTime);
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}

	if (HitTarget)
	{
		FRotator Rotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitTarget->GetActorLocation());
		//ViewCamera->SetWorldRotation(Rotate);
		GetController()->SetControlRotation(Rotate);
	}

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction,ETriggerEvent::Triggered , this, &APlayerCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this , &ACharacter::Jump);
		EnhancedInput->BindAction(EquipAction, ETriggerEvent::Started, this, &APlayerCharacter::EKeyPress);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::ComboAttack);
		EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dodge);
		EnhancedInput->BindAction(InventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::OpenInventory);
		EnhancedInput->BindAction(CameraLockAction, ETriggerEvent::Started, this, &APlayerCharacter::CameraLockToEnemy);
	}
}

void APlayerCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void APlayerCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void APlayerCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;

	if (Item->GetItemData().ItemType == EItemType::EIT_Gold ||
		Item->GetItemData().ItemType == EItemType::EIT_Soul ||
		Item->GetItemData().ItemType == EItemType::EIT_Equipment)
		return;

	if (InventoryComponent){
		if (InventoryComponent->TryAddItem(Item))
		{
			InventoryComponent->InventoryChanged.Broadcast();
			Item->SetItemOwner(this);
			Item->Destroy();
		}
	}
}

void APlayerCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		SlashOverlay->SetSouls(Attributes->GetSouls());
	}
}

void APlayerCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		SlashOverlay->SetGold(Attributes->GetGold());
	}
}

void APlayerCharacter::SetHPPercent(float HP)
{
	SlashOverlay->SetHealthBarPercent(FMath::Clamp(HP,0,100));
}

void APlayerCharacter::EKeyPress()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if(CanDisArm())
		{
			PlayEquipMontage(FName("UnEquip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if(CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void APlayerCharacter::Attack()
{
	Super::Attack();
	if (CanAttack() && HasEnoughStamina())
	{
		Attributes->UseStamina(Attributes->GetAttackCost());
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void APlayerCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"),this,this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	EquippedWeapon = Weapon;
	//OverlappingItem = nullptr;
}

void APlayerCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool APlayerCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool APlayerCharacter::CanDisArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool APlayerCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
	CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void APlayerCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void APlayerCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void APlayerCharacter::Dodge()
{
	
	//if (IsOccupied() || !HasEnoughStamina()) return;
	
	//PlayDodgeMontage();
	//ActionState = EActionState::EAS_Dodge;
	//
	//if (Attributes && SlashOverlay)
	//{
	//	Attributes->UseStamina(Attributes->GetDodgeCost());
	//	SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	//}
}





void APlayerCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName,EquipMontage);
	}
}

void APlayerCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

bool APlayerCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetAttackCost();
}

bool APlayerCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

void APlayerCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void APlayerCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void APlayerCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::ComboAttack()
{
	Super::Attack();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (ActionState == EActionState::EAS_Attacking
		&& HasEnoughStamina())
	{
		if (!AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			UseAttackStaminar();
			PlayMontageSection(AttackMontage, AttackMontageSections[CurrentAttackIndex]);
		}
	}
	else if (CanAttack() && HasEnoughStamina())
	{
		UseAttackStaminar();
		PlayMontageSection(AttackMontage, AttackMontageSections[CurrentAttackIndex]);
		ActionState = EActionState::EAS_Attacking;
	}
}



void APlayerCharacter::UseAttackStaminar()
{
	Attributes->UseStamina(Attributes->GetAttackCost());
	SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
}

bool APlayerCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void APlayerCharacter::InitializeSlashOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay && Attributes)
			{
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(1.f);
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSouls(0);
			}
		}
	}
}

void APlayerCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void APlayerCharacter::InitializeInventoryWidget()
{
	if (InventoryComponent)
	{
		//InventoryComponent->InventoryInitialize();
		InventoryComponent->SetOwner(this);
	}
}

void APlayerCharacter::OpenInventory()
{
	UInventoryWidget* Widget = InventoryComponent->GetInventoryWidget();

	if (Widget && Widget->IsInViewport())
	{
		Widget->RemoveFromParent();
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
		PlayerController->SetShowMouseCursor(false);
	}
	else if (Widget && !Widget->IsInViewport())
	{
		Widget->AddToViewport();
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, Widget,EMouseLockMode::DoNotLock,false);
		PlayerController->SetShowMouseCursor(true);
		//TakeWidget : UWidget To SWidget
	}
}



void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if(ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void APlayerCharacter::CameraLockToEnemy()
{
	bCameraLock = !bCameraLock;
	if (bCameraLock)
	{
		const FVector PlayerLocation = GetActorLocation();
		FVector CameraForwardVector;

		CameraForwardVector = CameraBoom->GetComponentRotation().Vector();
		CameraForwardVector *= 1000;
		CameraForwardVector += PlayerLocation;
	
		TArray < TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // 충돌 검사 대상으로 Pawn 타입을 추가
		FCollisionQueryParams CollisionParams;
		TArray<AActor*> ActorsToIgnore;
		CollisionParams.bTraceComplex = false;
		CollisionParams.AddIgnoredActors(ActorsToIgnore);

		FHitResult HitResult;

		UKismetSystemLibrary::SphereTraceSingleForObjects(this,
			PlayerLocation,
			CameraForwardVector,
			125,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true);

		HitTarget = HitResult.GetActor();

	}
	else if (!bCameraLock)
	{
		HitTarget = nullptr;

		FRotator Rotate = UKismetMathLibrary::FindLookAtRotation(ViewCamera->GetComponentLocation(), GetActorLocation());
		ViewCamera->SetWorldRotation(Rotate);
	}
}
