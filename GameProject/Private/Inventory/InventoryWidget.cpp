// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryGridWidget.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameState/MyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/DragDropOperation.h"
#include "Items/Item.h"

void UInventoryWidget::NativeOnInitialized()
{
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const FGeometry& Geometry = InventoryGridWidget->GetCachedGeometry();

	// 화면 좌상단을 (0,0) 을 기준으로 한 마우스 위치
	FVector2D mousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	//GridWidget의 위치와 크기
	const FVector2f& LocalPosition = Geometry.Position;
	const FVector2D& LocalSize = Geometry.GetLocalSize();

	// GridWidget 밖에 마우스 놓으면 소환하는식..
	if(!(LocalPosition.X <= mousePosition.X && mousePosition.X <= LocalPosition.X + LocalSize.X) &&
		LocalPosition.Y <= mousePosition.Y && mousePosition.Y <= LocalPosition.Y + LocalSize.Y)
	{
		if (AItem* PayloadItem = Cast<AItem>(InOperation->Payload))
		{
			AMyGameState* GameState = Cast<AMyGameState>(UGameplayStatics::GetGameState(GetWorld()));
			GameState->SpawnItemfromActor(PayloadItem, InventoryComponent->GetOwner(), true);
		}
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

bool UInventoryWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDragOver(InGeometry,InDragDropEvent,InOperation);
}
