// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "../Interfaces/PickupInterface.h"
#include "PlayerCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class USceneCaptureComponent2D;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
class UAnimMontage;
class AItem;
class UInventoryWidget;
class UInventoryGridWidget;

UCLASS()
class GAMEPROJECT_API APlayerCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

private:
	/*
	* Camera Variables
	*/

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	float CameraDist;

	/*
	* Item
	*/

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, AItem*> Equipment;

	/*
	* Input Variables
	*/

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CameraLockAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CharacterWindowAction;

	/*
	* Player States
	*/

	UPROPERTY(VisibleAnywhere)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;


	//Overlay
	UPROPERTY()
	class USlashOverlay* SlashOverlay;

	// Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

	// ComboAttack Varaible
	int CurrentAttackIndex;
	bool bNextAttack;

	// Camera Lock
	bool bCameraLock;
	AActor* HitTarget;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void EKeyPress();
	
	virtual void Attack() override;

	void EquipWeapon(AWeapon* Weapon);
	
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanDisArm();
	bool CanArm();
	void Disarm();
	void Arm();
	void Dodge();
	void PlayEquipMontage(FName SectionName);
	virtual void Die_Implementation() override;
	bool HasEnoughStamina();
	bool IsOccupied();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	
	void ComboAttack();

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(class ASoul* Soul) override;
	virtual void AddGold(class ATreasure* Treasure) override;

	void SetHPPercent(float HP);
	
	// State
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

	// Attack
	FORCEINLINE int GetCurrentAttackIndex() const { return CurrentAttackIndex; }
	FORCEINLINE bool IsNextAttack() const { return bNextAttack; }
	void SetNextAttack(bool _b) { bNextAttack = _b; }
	void SetCurrentAttackIndex(int _Index) { CurrentAttackIndex = _Index; }

	// GetSet
	UInventoryComponent* GetInventory() const { return InventoryComponent; }

private:
	void UseAttackStaminar();
	bool IsUnoccupied();
	void InitializeSlashOverlay();
	void SetHUDHealth();

	// Inventory Func
	void InitializeInventoryWidget();
	void OpenInventory();

	// Move
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	// Camera Lock
	void CameraLockToEnemy();

	friend class UPlayerAnimInstance;
};
