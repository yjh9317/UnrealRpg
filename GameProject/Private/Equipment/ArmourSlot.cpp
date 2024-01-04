// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/ArmourSlot.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"


UArmourSlot::UArmourSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//ArmourButton = CreateDefaultSubobject<UButton>(TEXT("ArmourButton"));
}

void UArmourSlot::ButtonPress()
{	
	//TArray<UUserWidget*> WidgetArray;
	//UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, WidgetArray, SlotInventoryClass, false);

	//for (int i = 0; i < WidgetArray.Num(); i++)
	//{
	//	WidgetArray[i]->RemoveFromParent();
	//}

	//UUserWidget* Widget = CreateWidget<UUserWidget>(GetOwningPlayer(), SlotInventoryClass);

	//AHUD* HUD = UGameplayStatics::GetPlayerController(this,0)->GetHUD();
	//ASlashHUD* SlashHUD = Cast<ASlashHUD>(HUD);

	////AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	////Cast<USlashOverlay>(GameMode);

	//USlashOverlay* SlashOverlay = SlashHUD->GetSlashOverlay();
	//UCanvasPanelSlot* Canvas = SlashOverlay->GetCanvasPanel()->AddChildToCanvas(Widget);
	//if (Canvas)
	//{
	//	Canvas->SetPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
	//	Canvas->SetAutoSize(true);
	//}
}

void UArmourSlot::NativeConstruct()
{
	Super::NativeConstruct();
	ArmourButton->OnClicked.AddDynamic(this, &UArmourSlot::ButtonPress);
}
