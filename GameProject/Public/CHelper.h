// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// 이 함수가 호출된 함수의 이름과 라인을 매크로로 String화 시킴 - 출처 : 이득우의 언리얼 C++ 게임 개발의 정석
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// 위의 CALLINFO와 함께 아무 내용이 없는 로그를 찍는 매크로 - 출처 : 이득우의 언리얼 C++ 게임 개발의 정석
#define LOG_S(Verbosity) UE_LOG(HACKED_LOG, Verbosity, TEXT("%s"), *LOG_CALLINFO)

// CALLINFO와 함께 텍스트로 로그를 찍는 매크로
#define LOG(Verbosity, Format, ...) UE_LOG(HACKED_LOG, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

// Screen에다가 LOG를 직접 찍는 매크로
#define	LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

// 위의 함수에 시간을 넣는 매크로. Tick에서 DeltaTime을 인자로 넣으면 보기 편함.
#define	LOG_SCREEN_T(Time, Format, ...) GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

// LOG에 Verbosity를 인자에서 빼고 매크로화 시킨것. 
#define LOG_WARNING(Format, ...) UE_LOG(HACKED_LOG, Warning, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define LOG_ERROR(Format, ...) UE_LOG(HACKED_LOG, Error, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

// assert()와 거의 비슷한 용도로 사용가능한 매크로. ...에는 retval이 들어감.
#define CHECK(Expr, ...) {if(!(Expr)) {LOG(Error, "ASSERTION : %s", "'"#Expr"'"); return __VA_ARGS__;}}


class GAMEPROJECT_API CHelper
{
public:
	CHelper();
	~CHelper();


public:
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		*OutObject = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		*OutClass = asset.Class;
	}

	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = NULL)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{
			(*OutComponent)->SetupAttachment(InParent);

			return;
		}

		InActor->SetRootComponent(*OutComponent);
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	template<typename T>
	static T* FindActor(UWorld* InWorld, int32 InIndex = 0)
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), actors);

		if (actors.Num() < 1)
			return NULL;

		return Cast<T>(actors[InIndex]);
	}

	template<typename T>
	static void FindActors(UWorld* InWorld, TArray<T*>& OutActors)
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), actors);

		for (AActor* actor : actors)
		{
			T* obj = Cast<T>(actor);

			if (!!obj)
				OutActors.Add(obj);
		}
	}

	static FLinearColor GetRandomColor()
	{
		FLinearColor color;
		color.R = UKismetMathLibrary::RandomFloatInRange(0, 1);
		color.G = UKismetMathLibrary::RandomFloatInRange(0, 1);
		color.B = UKismetMathLibrary::RandomFloatInRange(0, 1);
		color.A = 1.0f;

		return color;
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static T* GetComponent(AActor* InActor, FString InComponentName)
	{
		TArray<T*> components;
		InActor->GetComponents<T>(components);

		for (T* component : components)
		{
			if (component->GetName() == InComponentName)
				return component;
		}

		return NULL;
	}

	static FString GetStringFromEnum(FString InTypeName, int32 InIndex)
	{
		UEnum* ptr = FindObject<UEnum>(ANY_PACKAGE, *InTypeName, true);
		return ptr->GetNameStringByIndex(InIndex);
	}

	static void PlayEffect(UWorld* InWorld, UFXSystemAsset* InEffect, FTransform& InTransform, USkeletalMeshComponent* InMesh = NULL, FName InSocketName = NAME_None)
	{
		UParticleSystem* particle = Cast<UParticleSystem>(InEffect);
		UNiagaraSystem* niagara = Cast<UNiagaraSystem>(InEffect);

		FVector location = InTransform.GetLocation();
		FRotator rotation = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();
		if (InMesh)
		{
			if (particle)
			{
				UGameplayStatics::SpawnEmitterAttached(particle, InMesh, InSocketName, location, rotation, scale);

				return;
			}

			if (niagara)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, InMesh, InSocketName, location, rotation, scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);

				return;
			}
		}

		if (particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, InTransform);

			return;
		}

		if (niagara)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, location, rotation, scale);
	}

	static void Check(int Tmp)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), Tmp);
	}
};
