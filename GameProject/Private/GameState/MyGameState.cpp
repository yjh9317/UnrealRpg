// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/MyGameState.h"
#include "EditorLevelLibrary.h"
#include "Items/Item.h"

void AMyGameState::SpawnItemfromActor(AItem* Item, AActor* Actor, bool GroundClamp)
{
	FVector ActorLocation = Actor->GetActorLocation();
	FVector ForwardVector = Actor->GetActorForwardVector();

	FVector SpawnLocation = ActorLocation + ForwardVector * 150.f;
	FVector SpawnEndLocation =
		SpawnLocation - FVector{SpawnLocation.X,SpawnLocation.Y,SpawnLocation.Z-1000.f}; 
	
	if (GroundClamp)
	{
		FHitResult HitResult;
		
		bool IsTrace = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			SpawnLocation,
			SpawnEndLocation,
			ECollisionChannel::ECC_Visibility);

		if(IsTrace)
		{
			SpawnLocation = HitResult.Location;
		}
	}
	
	// StaticClass는 컴파일 타임, GetClass()는 런타임 기준, StaticClass로 계속 하다가 시간 많이 버림 --
	AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(Item->GetClass(), SpawnLocation, FRotator::ZeroRotator);
}