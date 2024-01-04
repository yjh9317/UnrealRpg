// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInfo.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UInventoryComponent;

UCLASS()
class GAMEPROJECT_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

	virtual void Tick(float DeltaTime) override;
	void SetInventoryComponent(UInventoryComponent* Inventory) { InventoryComponent = Inventory; }

protected:

	/*
	 * Variable
	 */

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class UInventoryComponent* InventoryComponent = nullptr;

	// For Item Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	FItemData ItemData;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UItemWidget* ItemWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UItemWidget> ItemWidgetClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	AActor* ItemOwner;
	/*
	 * Function
	 */

	template<typename T>
	T Avg(T First, T Seceond);

	UFUNCTION(BlueprintPure)
	float TransformedSin();
	UFUNCTION(BlueprintPure)
	float TransformedCos();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;


public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	FItemData GetItemData() const { return ItemData; }

	void SetItemWidget(UItemWidget* _ItemWidget) { ItemWidget = _ItemWidget;} 
	FORCEINLINE UItemWidget* GetItemWidget() const {return ItemWidget;}

	virtual void Use();

	AActor* GetItemOwner() const {return ItemOwner;}
	void SetItemOwner(AActor* Actor) {ItemOwner = Actor;}
};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}