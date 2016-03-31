// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "Pickup.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

/**
 * This struct defines slots that make up the inventory.
 * 
 */
USTRUCT(BlueprintType)
struct FSlot
{
	GENERATED_USTRUCT_BODY()

	enum OverflowAction { NONE, SWAP, DESTROY };
	UClass* acceptedType;
	TArray<APickup*> items;
	int stackSize;
	bool isMoney;

	OverflowAction overflowAction;

	FSlot() {} // We don't need this but Unreal demands it.

	FSlot(UClass* type)
	{
		acceptedType = type;
		stackSize = 1;
		isMoney = false;
		items.SetNumZeroed(0);
		overflowAction = OverflowAction::NONE;
	}

	bool handleItem(APickup* item) {
		if (items.Num() < stackSize)
		{
			// We have space in this slot, let's possess the item.
			item->possessed();
			if (item->isConsumed()) {
				// It appears this item was an Instant Consumable item, let's not add it to the list and instead destroy it.
				item->Destroy();
				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] handleItem: instant"));
			}
			else
			{
				for (int i = 0; i < item->StackSize; i++)
					items.Add(item);

				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] handleItem: add (") + FString::FromInt(item->StackSize) + FString::FString(")"));
			}

			return true;
		}
		else
		{
			switch (overflowAction) {
			case OverflowAction::NONE:
				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] handleItem: NONE"));
				return false;
				break;
			case OverflowAction::SWAP:
				//swap items? EDIT: No, swapping will be handled in blueprint.

				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] handleItem: SWAP"));
				return true;
				break;
			case OverflowAction::DESTROY:
				item->Destroy();

				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] handleItem: DESTROY"));
				return true;
				break;
			}
		}

		return false;
	}
};

UCLASS()
class DESCEND04_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Hand over item to the Inventory")
	bool receiveItem(APickup* item);

	UFUNCTION(BlueprintCallable, Category = "Activate an item from the inventory")
	bool activateItem(APickup* item);

	UFUNCTION(BlueprintCallable, Category = "Get a list of all slots in the inventory")
	TArray<FSlot> getSlots();
	
private:
	TArray<FSlot*> slots;		// The inventory consists of slots. Each slot can contain different items/amounts etc.
};
