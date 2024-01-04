// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interfaces/HitInterface.h"
#include "Character/CharacterTypes.h"
#include "Enemy.generated.h"


class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class GAMEPROJECT_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/* AActor */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/* IHitInterface*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	AWeapon* GetWeapon() const { return EquippedWeapon; }
	virtual void Attack() override;

protected:
	/* <AActor> */
	virtual void BeginPlay() override;

	/* <ABaseCharacter> */
	virtual void Die_Implementation() override;
	void SpawnSoul();
	
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount);
	virtual int32 PlayDeathMontage() override;
	virtual void AttackEnd() override;

	//UPROPERTY(BlueprintReadOnly)
	//TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState =EEnemyState::EES_Patrolling;
	
public:
	UPROPERTY()
	AAIController* EnemyController;

private:
	/* AI Behavior*/
	void InitializeEnemy();
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, float Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();
	

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;


	UPROPERTY(EditAnywhere, Category = Combat, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = Combat, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = Combat, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	double AcceptanceRadius = 50.f;
	
	UPROPERTY(EditInstanceOnly, Category="AI Navigation", BlueprintReadOnly, meta = (AllowPrivateAccess ="true"))
	AActor* PatrolTarget;
	
	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<AActor*> PatrolTargets;
	
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;
	
	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float PatrolWaitMin = 3.f;
	
	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float PatrolWaitMax = 5.f;
	
	UPROPERTY(EditAnywhere, Category= Combat)
	float PatrollingSpeed = 125.f;
	
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category= Combat)
	float AttackMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category= Combat)
	float AttackMax = 1.f;
	
	UPROPERTY(EditAnywhere, Category= Combat)
	float ChasingSpeed = 300.f;
	
	UPROPERTY(EditAnywhere,Category= "Combat")
	float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ASoul> SoulClass;

	friend class UPaladinAnimInstance;
};
