// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "Pickup.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

USTRUCT()
struct FSlot
{
	GENERATED_USTRUCT_BODY()

	enum OverflowAction { NONE, SWAP, DESTROY };
	UClass* acceptedType;
	TArray<APickup*> items;
	int stackSize;

	OverflowAction overflowAction;

	FSlot() {} // We don't need this but Unreal demands it.

	FSlot(UClass* type)
	{
		acceptedType = type;
		stackSize = 1;
		items.SetNumZeroed(0);
		overflowAction = OverflowAction::NONE;
	}

	bool handleItem(APickup* item) {
		if (items.Num() < stackSize)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("handleItem: add"));
			items.Add(item);
			//item hide from world
			return true;
		}
		else
		{
			switch (overflowAction) {
			case OverflowAction::NONE:
				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("handleItem: NONE"));
				return false;
				break;
			case OverflowAction::SWAP:
				//swap items

				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("handleItem: SWAP"));
				return true;
				break;
			case OverflowAction::DESTROY:
				item->Destroy();

				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("handleItem: DESTROY"));
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
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	bool recieveItem(APickup* item);
	
private:
	TArray<FSlot*> slots;
};
