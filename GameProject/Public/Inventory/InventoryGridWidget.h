// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/UserWidget.h"
#include "Character/PlayerCharacter.h"
#include "InventoryGridWidget.generated.h"

class APlayerCharacter;
class UItemWidget;
USTRUCT(BlueprintType)
struct FInventoryLine
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Begin {
		0.f, 0.f
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector2D End {
		0.f, 0.f
	};
	FInventoryLine() = default;

	FInventoryLine(const FVector2D& InputBegin, const FVector2D& InputEnd)
	{
		Begin = InputBegin;
		End = InputEnd;
	}
};

USTRUCT(BlueprintType)
struct FInventoryTile
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 X;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Y;

	FInventoryTile() = default;

	FInventoryTile(const int32& InputX, const int32& InputY)
	{
		X = InputX;
		Y = InputY;
	}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRefreshDelegate);


class UCanvasPanel;

UCLASS()
class GAMEPROJECT_API UInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryGridWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeOnInitialized() override;
	//virtual void NativePreConstruct() override;
	//virtual void NativeConstruct() override;
	//virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual bool Initialize() override;
private:

	/*
	* Binding Variable
	*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget),VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UBorder* GridBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget),VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UCanvasPanel* GridCanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget),VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UCanvasPanel* BackCanvasPanel;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FInventoryLine> Lines;
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Data")
	TObjectPtr<AActor> ActorOwner = GetOwningPlayerPawn();

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Data")
	TArray<AItem*> ItemGrid;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Data")
	TArray<AItem*> ItemList;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category= "Inventory|Item")
	TSubclassOf<UItemWidget> UItemwidgetClass;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TileSize = 40;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Width = 20;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Height = 20;

	FRefreshDelegate RefreshDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int DragItemGridIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsInGrid = false;
private:
	void CreateLineSegments();

	
public:
	// UFUNCTION(BlueprintCallable)
	// void MyInitialize();
	AItem* GetPayload(const UDragDropOperation* InOperation) const;
	bool IsRoomAvailableForPayload(const AItem* Item) const;
	
	TArray<AItem*>& GetItemGridArray() {return ItemGrid;}

	UFUNCTION(BlueprintCallable)
	void Refresh();

	UFUNCTION()
	void OnItemRemoved(AItem* Item);

	UFUNCTION()
	APlayerCharacter* GetPlayerOwner() const {return Cast<APlayerCharacter>(ActorOwner);}
	
	friend class UInventoryComponent;
};
