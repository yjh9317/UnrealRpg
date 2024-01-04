// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

class USlashOverlay;

UCLASS()
class GAMEPROJECT_API ASlashHUD : public AHUD
{
	GENERATED_BODY()
		
protected:
	virtual void BeginPlay() override; 

private:
	UPROPERTY(EditDefaultsOnly, Category = Slash)
	TSubclassOf<USlashOverlay> SlashOverlayClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USlashOverlay* SlashOverlay;


	UPROPERTY(EditDefaultsOnly, Category = EquippmentWindow)
	TSubclassOf<UUserWidget> EquipmentWindowClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UUserWidget* EquipmentWindow;


public:
	FORCEINLINE USlashOverlay* GetSlashOverlay() const { return SlashOverlay; }
};
