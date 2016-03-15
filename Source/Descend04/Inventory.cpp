// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "Inventory.h"
#include "PickupAmethyst.h"


// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FSlot* slotAmethyst = new FSlot(APickupAmethyst::StaticClass());
	slotAmethyst->overflowAction = FSlot::OverflowAction::NONE;
	slotAmethyst->stackSize = 2;
	slots.Add(slotAmethyst);

	
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] Inventory constructed!"));

	bool res = false;

	APickupAmethyst* newItem = NewObject<APickupAmethyst>();
	res = recieveItem(newItem);
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] recieveItem returns: ") + FString::FString(res?"true":"false"));

	APickupAmethyst* newItem2 = NewObject<APickupAmethyst>();
	res = recieveItem(newItem2);
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] recieveItem returns: ") + FString::FString(res ? "true" : "false"));

	APickupAmethyst* newItem3 = NewObject<APickupAmethyst>();
	res = recieveItem(newItem3);
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] recieveItem returns: ") + FString::FString(res ? "true" : "false"));
}

// Called every frame
void AInventory::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

bool AInventory::recieveItem(APickup* item)
{
	for (FSlot* slot : slots)
	{
		if (item->IsA(slot->acceptedType))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] recieveItem: accepted"));

			bool bHandled = slot->handleItem(item);

			if (bHandled) {
				item->possessed();
				if (item->isConsumed())
					item->Destroy();
			}
			
			return bHandled;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] recieveItem: false"));
	return false;
}