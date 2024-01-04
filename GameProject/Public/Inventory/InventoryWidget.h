// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// UInventoryWidget();
	// virtual bool Initialize() override;

protected:
	virtual void NativeOnInitialized() override;
	// virtual void NativePreConstruct() override;
	// virtual void NativeConstruct() override;
	// virtual void NativeDestruct() override;

	// virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	class UInventoryGridWidget* InventoryGridWidget;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UInventoryComponent* InventoryComponent;
};
