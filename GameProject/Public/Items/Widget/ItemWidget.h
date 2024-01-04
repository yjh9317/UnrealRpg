// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemRefresh);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoved, AItem*, Item);

UCLASS()
class GAMEPROJECT_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	//virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	//virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	// virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// virtual void NativeOnDragLeave(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 인터넷에 있는 드래그 방법
	FReply CustomDectectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);
public:
	UItemWidget(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	class UCanvasPanel* CanvasPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	class USizeBox* BackgroundSizeBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	class UBorder* BackGroundBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	class UImage* ItemImage;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Widget")
	class UInventoryGridWidget* ParentGrid = nullptr;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item")
	class AItem* Item;


protected:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory|UI|Click")
	float RightClickMaxDuration = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|UI|Click")
	bool IsRightClicking = false;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|UI|Click")
	FPointerEvent ClickEvent;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|UI|Click")
	FTimerHandle RightClickTimerHandle;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item")
	FVector2D Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item")
	float TileSize;
	
	FItemRefresh RefreshDelegate;

	FOnRemoved RemoveItemsDelegate;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float ClickTime = 0.f;

	UPROPERTY()
	bool ReLeftClick = false;

protected:
	UFUNCTION(BlueprintCallable)
	void RefreshSize();

	UFUNCTION(BlueprintCallable)
	void RefreshSizeBox() const;

	UFUNCTION(BlueprintCallable)
	void RefreshItemImage();


protected:
	UFUNCTION()
	void RightClickTimerFunction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintCosmetic)
	bool RightClickLongEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintCosmetic)
	bool RightClickShortEffect();

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	bool LeftClickEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintCosmetic)
	void SetupUI();

	UFUNCTION(BlueprintCallable)
	void DisplayDescription(const FPointerEvent& InMouseEvent);

	UFUNCTION(BlueprintCallable)
	void DisplayBookText(const FPointerEvent& InMouseEvent);

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	virtual void StopDrag();

	UFUNCTION(BlueprintCallable)
	FSlateBrush GetItemIcon();
	
public:
	UFUNCTION(BlueprintCallable)
	void SetItem(AItem* _Item) {Item = _Item;}

	UFUNCTION(BlueprintCallable)
	void SetParentGrid(UInventoryGridWidget* InputParentGrid);

	FORCEINLINE UCanvasPanel* GetCanvasPanel() const { return CanvasPanel; }
	FORCEINLINE USizeBox* GetSizeBox() const { return BackgroundSizeBox; }
	FORCEINLINE UBorder* GetBorder() const { return BackGroundBorder; }
	FORCEINLINE UImage* GetImage() const { return ItemImage; }

	UFUNCTION()
	void Refresh();

	UFUNCTION()
	void ItemUse();
};
