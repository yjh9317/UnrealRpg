#include "Components/InventoryComponent.h"

#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryGridWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Items/Item.h"
#include "CHelper.h"
#include "IDetailTreeNode.h"
#include "SNodePanel.h"
#include "Items/Widget/ItemWidget.h"



UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	InventoryWidget->InventoryComponent = this;
	
	InventoryGridWidget = InventoryWidget->InventoryGridWidget;
	InventoryGridWidget->InventoryComponent = this;
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::AddItemAt(AItem* Item, int TopLeftIndex)
{
	//ItemList.Add(NewItem);
	int32 Width = InventoryGridWidget->Width;
	//int32 Height = InventoryGridWidget->Height;
	
	//assign item to the map
	const int ItemWidth = Item->GetItemData().Width;
	const int ItemHeight = Item->GetItemData().Height;
	const int32 Sx = TopLeftIndex % Width;
	const int32 Sy = TopLeftIndex / Width;

	TArray<AItem*>& ItemGrid = InventoryGridWidget->GetItemGridArray();

	UItemWidget* CurItemWidget = Item->GetItemWidget();
	
	for (int y = Sy; y < Sy + ItemHeight; ++y)
	{
		for (int x = Sx; x < Sx + ItemWidth; ++x)
		{
			const int32 ID = x + y * Width;
			ItemGrid[ID] = Item;
		}
	}
	if (CurItemWidget)
	{
		CurItemWidget->SetParentGrid(InventoryGridWidget);
		CurItemWidget->TileSize = InventoryGridWidget->TileSize;
	}
	
	if(InventoryGridWidget->RefreshDelegate.IsBound())
	{
		InventoryGridWidget->RefreshDelegate.Broadcast();
	}

	InventoryGridWidget->ItemList.Add(Item);
}

bool UInventoryComponent::TryAddItem(AItem* Item)
{
	InventoryChanged.Broadcast();
	const TArray<AItem*>& ItemGrid = InventoryGridWidget->GetItemGridArray();
	
	for(int i = 0;i<ItemGrid.Num();i++)
	{
		if(IsRoomAvailable(Item,i))
		{
			AddItemAt(Item,i);
			return true;
		}
	}
	//AddItemAt(Item);
	return false;
}

void UInventoryComponent::RemoveItem(AItem* Item)
{
	
}

bool UInventoryComponent::IsRoomAvailable(const AItem* Item, int TopLeftIndex) const
{
	int32 Width = InventoryGridWidget->Width;
	int32 Height = InventoryGridWidget->Height;
	
	const int32 MaxIndex = Width * Height;

	if (TopLeftIndex < 0 || TopLeftIndex > MaxIndex)
		return false;
	
	
	const int32 ItemWidth = Item->GetItemData().Width;
	const int32 ItemHeight = Item->GetItemData().Height;
	const int32 sx = TopLeftIndex % Width;
	const int32 sy = TopLeftIndex / Width;

	const TArray<AItem*>& ItemGrid = InventoryGridWidget->GetItemGridArray();
	
	for (int y = sy; y < sy + ItemHeight; ++y)
	{
		for (int x = sx; x < sx + ItemWidth; ++x)
		{
			if (x >= Width || x < 0)
			{
				return false;
			}
			if (y >= Height || y < 0)
			{
				return false;
			}

			const int32 ID = x + y * Width;
			if (ID < 0 || ID >= Width * Height)
				return false;

			if (ItemGrid[ID] != nullptr) //only look for empty stuff
			{
				UE_LOG(LogTemp, Log, TEXT("Cannot copy because %d %d %d is not empty"), x, y, ID);
				return false;
			}
		}
	}

	return true;
}

TMap<AItem*, int> UInventoryComponent::GetAllItems()
{
	const TArray<AItem*>& ItemGrid = InventoryGridWidget->GetItemGridArray();
	
	TMap<AItem*, int> AllItems;

	for (int i = 0; i < ItemGrid.Num(); i++)
	{
		AItem* CurItem = ItemGrid[i];
		if(CurItem)
		{
			if(!AllItems.Contains(CurItem))
			{
				AllItems.Add(CurItem,i);
				UE_LOG(LogTemp,Warning,TEXT("%d is Added"),i);
			}
		}
	}

	return AllItems;
}



