#include "Inventory/InventoryGridWidget.h"
#include "Components/InventoryComponent.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/UserWidget.h"

#include "Components/Widget.h"
#include "Items/Item.h"
#include "Items/ItemInfo.h"

#include "CHelper.h"
#include "Blueprint/DragDropOperation.h"
#include "GameState/MyGameState.h"
#include "Items/Widget/ItemWidget.h"
#include "Math/UnrealMathUtility.h"

UInventoryGridWidget::UInventoryGridWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UInventoryGridWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UCanvasPanelSlot* PanelSlot =UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	if(PanelSlot)
	PanelSlot->SetSize(FVector2D{Width*TileSize,Height*TileSize});
	CreateLineSegments();

	ItemGrid.SetNum(Width*Height);
	
	RefreshDelegate.AddDynamic(this,&UInventoryGridWidget::Refresh);
}

void UInventoryGridWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	GridBorder->OnMouseButtonDownEvent;
}


bool UInventoryGridWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	AItem* PayloadItem = GetPayload(InOperation);

	if(PayloadItem)
	{
		if(IsRoomAvailableForPayload(PayloadItem))
		{
			InventoryComponent->AddItemAt(PayloadItem, DragItemGridIndex);
		}
		else
		{
			//InventoryComponent->AddItemAt()
		}
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

bool UInventoryGridWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	FVector2D MousePosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	
	DragItemGridIndex = static_cast<int>((MousePosition.Y / TileSize)) * Width + (MousePosition.X / TileSize);

	//fmod(MousePosition.Y, TileSize);

	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

bool UInventoryGridWidget::Initialize()
{
	return Super::Initialize();
}

void UInventoryGridWidget::CreateLineSegments()
{
	{
		float Y = TileSize * Height;
		for (size_t i = 0; i <= Width; ++i)
		{
			float X = TileSize * i;
			Lines.Add(FInventoryLine({X, 0.f}, {X, Y}));
		}
	}

	{
		float X = TileSize * Width;
		for (size_t i = 0; i <= Height; ++i)
		{
			float Y = TileSize * i;
			Lines.Add(FInventoryLine({0.f, Y}, {X, Y}));
		}
	}
}

AItem* UInventoryGridWidget::GetPayload(const UDragDropOperation* InOperation) const
{
	if(nullptr != InOperation)
	{
		AItem* PayloadItem = Cast<AItem>(InOperation->Payload);

		if(PayloadItem)
		return PayloadItem;
	}
	return nullptr;
}

bool UInventoryGridWidget::IsRoomAvailableForPayload(const AItem* Item) const
{
	if(Item)
	{
		return InventoryComponent->IsRoomAvailable(Item,DragItemGridIndex);
	}
	return false;
}

// void UInventoryGridWidget::MyInitialize()
// {
// 	UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
// 	if (PanelSlot)
// 		PanelSlot->SetSize(FVector2D{ Width * TileSize,Height * TileSize });
// 	CreateLineSegments();
//
// 	ItemGrid.SetNum(Width * Height);
// 	Refresh();
// }

void UInventoryGridWidget::Refresh()
{
	GridCanvasPanel->ClearChildren();

	TMap<AItem*, int> AllItems = InventoryComponent->GetAllItems();
	TMap<AItem*, int>::TIterator iter(AllItems);

	TMap<AItem*, int> CurAllItems;
	
	for (iter; iter; ++iter)
	{
		if(!CurAllItems.Find(iter->Key) && UItemwidgetClass)
		{
			int value = iter->Value;
			UE_LOG(LogTemp,Warning,TEXT("%d is Added"),value);
			
			UItemWidget* ItemWidget = CreateWidget<UItemWidget>(GetOwningPlayer(),UItemwidgetClass);
			AItem* Item = iter->Key;
			FItemData ItemData = Item->GetItemData();
			
			Item->SetItemWidget(ItemWidget);
			ItemWidget->SetItem(Item);
			
			ItemWidget->Size.X = ItemData.Width * TileSize;
			ItemWidget->Size.Y = ItemData.Height * TileSize;
			ItemWidget->RemoveItemsDelegate.AddDynamic(this,&UInventoryGridWidget::OnItemRemoved);
	
			UPanelSlot* PanelSlot = GridCanvasPanel->AddChild(ItemWidget);
			UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(PanelSlot);

			if(CanvasPanelSlot)
			{
				CanvasPanelSlot->SetAutoSize(true);
	
				int ID = iter->Value;
				const int32 Sx = ID % Width;
				const int32 Sy = ID / Width;
				CanvasPanelSlot->SetPosition(FVector2D{TileSize * Sx,TileSize * Sy});
			}
			if(ItemWidget->RefreshDelegate.IsBound())
				ItemWidget->RefreshDelegate.Broadcast();

			CurAllItems.Add(Item, value);
		}
	}
}

void UInventoryGridWidget::OnItemRemoved(AItem* Item)
{
	if(nullptr == Item) return;

	for(int i =0;i<ItemGrid.Num();i++)
	{
		if(Item == ItemGrid[i])
		{
			ItemGrid[i] = nullptr;
		}
	}

	ItemList.Remove(Item);
}


