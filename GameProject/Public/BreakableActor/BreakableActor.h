// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class UCapsuleComponent;
class ATreasure;

UCLASS()
class GAMEPROJECT_API ABreakableActor : public AActor , public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
protected:
	virtual void BeginPlay() override;

	
public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;


private:
	UPROPERTY(EditAnywhere , Category="Breakable Properties")
	TArray<TSubclassOf<ATreasure>> TreasureClasses;
	
	bool bBroken = false;


};
