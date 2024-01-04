#pragma once


#include "ItemInfo.generated.h"

/*
*  About Item 
*/
class AItem;

UENUM(BlueprintType)
enum EItemType
{
	EIT_Equipment UMETA(DisplayName = "Equipment"),
	EIT_Consumableble UMETA(DisplayName = "Consumableble"),
	EIT_Etc UMETA(DisplayName = "Etc"),
	EIT_Soul UMETA(DisplayName ="Soul"),
	EIT_Gold UMETA(DisplayName = "Gold"),

	EIT_End UMETA(DisplayName = "End")
};

UENUM()
enum class EWeaponPart : uint8
{
	EWP_RightHand UMETA(DisplayName = "RightHand")
};

UENUM()
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

//UENUM(BlueprintType)
//enum class EItemSize : uint8
//{
//	Tiny UMETA(DisplayName = "Tiny"),
//	Small UMETA(DisplayName = "Small"),
//	Medium UMETA(DisplayName = "Medium"),
//	Large UMETA(DisplayName = "Large"),
//	Giant UMETA(DisplayName = "Giant")
//};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Epic UMETA(DisplayName = "Epic"),
};
enum class EEquippmentPart : uint8
{
	EEP_HEAD UMETA(DisplayName = "Head"),
	EEP_SHOULDER UMETA(DisplayName = "Sholuder"),
	EEP_CHEST UMETA(DisplayName = "Chest"),
	EEP_HANDS UMETA(DisplayName = "Hands"),
	EEP_LEGS UMETA(DisplayName = "Legs"),
	EEP_FEET UMETA(DisplayName = "Feet"),
};

class UMaterialInterface;

USTRUCT(BlueprintType)
struct FItemData
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|ItemData")
	int32 ItemID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|ItemData")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|ItemData")
	uint8 Width = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|ItemData")
	uint8 Height = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Visual")
	UTexture2D* Texture = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Visual")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Visual")
	UMaterialInterface* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Visual")
	UMaterialInterface* IconRotated = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|ItemData")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|ItemData")
	float Weight = 0.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|ItemData")
	//TEnumAsByte<EItemSize> ItemSize = EItemSize::Tiny;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|ItemData")
	TEnumAsByte<EItemType> ItemType = EItemType::EIT_Etc;
};

