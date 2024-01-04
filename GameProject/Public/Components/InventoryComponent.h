// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;
class UInventoryGridWidget;
class AItem;
class UItemObject;
struct FInventoryTile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS()
class GAMEPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
protected:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<UInventoryGridWidget> InventoryGridWidgetClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UInventoryGridWidget* InventoryGridWidget;
	
	UPROPERTY()
	AActor* Owner;

public:
	FOnInventoryChanged InventoryChanged;

	
public:
	UFUNCTION(BlueprintCallable)
	void AddItemAt(AItem* Item, int TopLeftIndex);

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(AItem* Item);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(AItem* Item);

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(const AItem* Item,int TopLeftIndex) const;

	UFUNCTION(BlueprintCallable)
	TMap<AItem*,int> GetAllItems();
public:
	void SetOwner(AActor* Actor) { Owner = Actor; }
	AActor* GetPlayerOwner() const {return Owner;}
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	FORCEINLINE UInventoryGridWidget* GetInventoryGridWidget() const { return InventoryGridWidget; }
};
