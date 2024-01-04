// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Widget/ItemWidget.h"
#include "Items/Item.h"
#include "Inventory/InventoryGridWidget.h"

#include "Components/SizeBox.h"
#include "Components/Image.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Kismet/KismetInputLibrary.h"
#include "Blueprint/DragDropOperation.h"


UItemWidget::UItemWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UItemWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	RefreshDelegate.AddDynamic(this,&UItemWidget::Refresh);
}


void UItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	BackGroundBorder->SetBrushColor(FLinearColor{0.5,0.5,0.5,0.2});
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	BackGroundBorder->SetBrushColor(FLinearColor{0,0,0,0.5});
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	UDragDropOperation* DragWidget = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());

	DragWidget->DefaultDragVisual = this;
	DragWidget->Payload = Item;
	DragWidget->Pivot = EDragPivot::MouseDown;
	this->SetVisibility(ESlateVisibility::HitTestInvisible);
	OutOperation = DragWidget;
	RemoveItemsDelegate.Broadcast(Item);
	RemoveFromParent();
}

FReply UItemWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// if (!IsRightClicking)
	// 	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	// IsRightClicking = false;
	// GetWorld()->GetTimerManager().ClearTimer(RightClickTimerHandle);
	// if (RightClickShortEffect())
	// 	return FReply::Handled();
	
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	// FEventReply MouseDownEvent = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::LeftMouseButton);
	// return MouseDownEvent.NativeReply;

	// ClickEvent = InMouseEvent;
	// if (UKismetInputLibrary::PointerEvent_GetEffectingButton(InMouseEvent) != FKey("RightMouseButton"))
	// {
	// 	LeftClickEffect();
	// 	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	// }
	// IsRightClicking = true;
	// GetWorld()->GetTimerManager().SetTimer(RightClickTimerHandle, this, &UItemWidget::RightClickTimerFunction,
	// 									   RightClickMaxDuration,
	// 									   false, RightClickMaxDuration);

	if(UKismetInputLibrary::PointerEvent_GetEffectingButton(InMouseEvent) == FKey("LeftMouseButton"))
	{
		FEventReply MouseDownEvent = UWidgetBlueprintLibrary::DetectDragIfPressed(
			InMouseEvent, this, EKeys::LeftMouseButton);
		return MouseDownEvent.NativeReply;
	}
	if(UKismetInputLibrary::PointerEvent_GetEffectingButton(InMouseEvent) == FKey("RightMouseButton"))
	{
		ItemUse();		
	}
	
	return FReply::Handled();
}

FReply UItemWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	return FReply::Handled();
}

FReply UItemWidget::CustomDectectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if ( InMouseEvent.GetEffectingButton() == DragKey /*|| PointerEvent.IsTouchEvent()*/ )
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();
		
		if ( WidgetDetectingDrag )
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();
			if ( SlateWidgetDetectingDrag.IsValid() )
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				return Reply.NativeReply;
			}
		}
	}

	return FReply::Unhandled();
}

void UItemWidget::RefreshSize()
{
	if (ParentGrid && Item)
	{
		TileSize = ParentGrid->TileSize;
		const int Width = Item->GetItemData().Width;
		const int Height = Item->GetItemData().Height;

		Size.X = TileSize * Width;
		Size.Y = TileSize * Height;
	}
}

void UItemWidget::RefreshSizeBox() const
{
	if (BackgroundSizeBox)
	{
		BackgroundSizeBox->SetWidthOverride(Size.X);
		BackgroundSizeBox->SetHeightOverride(Size.Y);
	}
}

void UItemWidget::RefreshItemImage()
{
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(Item->GetItemData().Texture);
		ItemImage->SetDesiredSizeOverride(FVector2D{Size.X,Size.Y});
	}
}


void UItemWidget::SetParentGrid(UInventoryGridWidget* InputParentGrid)
{
	ParentGrid = InputParentGrid;
}

void UItemWidget::Refresh()
{
	RefreshSize();
	RefreshSizeBox();
	RefreshItemImage();
}

void UItemWidget::ItemUse()
{
	Item->Use();
	RemoveFromParent();
}


void UItemWidget::RightClickTimerFunction()
{
}

bool UItemWidget::LeftClickEffect()
{
	return true;
}

void UItemWidget::DisplayDescription(const FPointerEvent& InMouseEvent)
{
}

void UItemWidget::DisplayBookText(const FPointerEvent& InMouseEvent)
{
}

void UItemWidget::StopDrag()
{
}

FSlateBrush UItemWidget::GetItemIcon()
{
	return UWidgetBlueprintLibrary::MakeBrushFromMaterial(
		Item->GetItemData().Icon,
		Item->GetItemData().Weight * TileSize,
		Item->GetItemData().Height * TileSize);
}