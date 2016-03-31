// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "Inventory.h"
#include "PickupMoney.h"

AInventory::AInventory()
{
	PrimaryActorTick.bCanEverTick = false;

	// Let's set up the slots for the inventory
	FSlot* slot1 = new FSlot(APickupMoney::StaticClass());
	slot1->overflowAction = FSlot::OverflowAction::NONE;
	slot1->stackSize = 99;
	slot1->isMoney = true;			// Let's designate this slot as the currency holder.
	slots.Add(slot1);

	// more lsots here
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] Inventory constructed!"));
}

/**
 * Takes a subclass of APickup and hands it over to the inventory for handling.
 * If there's a slot that can successfully handle the item then this returns TRUE, otherwise FALSE.
 * 
 * @param		item	the items that will be attempted to be placed into the inventory.
 * @return				true if the item is considered handled, otherwise false.
 */
bool AInventory::receiveItem(APickup* item)
{
	// Loop through the slots to see if there's a slot that can accept the received item.
	for (FSlot* slot : slots)
	{
		if (item->IsA(slot->acceptedType))
		{
			// Check cost to get item.
			if (item->CostToBuy > 0)
			{
				bool paid = false;

				// Look for a slot that is designated as money.
				for (FSlot* slotMoney : slots)
				{
					if (slotMoney->isMoney)
					{
						if (slotMoney->items.Num() < item->CostToBuy)
						{
							// Can't afford it!
							GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] receiveItem: can't afford"));

							return false;
						}
						else
						{
							paid = true;
							break;
						}
						
					}
				}
				
				if (!paid) {
					// No slots with money found, can't pay!
					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] receiveItem: no money"));
					return false;
				}
			}

			// Try to handle the item.
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] receiveItem: handling"));
			
			bool bHandled = slot->handleItem(item);

			return bHandled;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] receiveItem: no go"));
	return false;
}

/**
 * Loops through the slots looking for a slot that has the item type given, then activates an item in that slot (if any).
 * 
 * @param		item	the item to look for and activate.
 * @return				returns true if a matching item was found, otherwise false.
 */
bool AInventory::activateItem(APickup* item)
{
	for (FSlot* slot : slots)
	{
		if (item->IsA(slot->acceptedType))
		{
			if (slot->items.Num() > 0)
			{
				APickup* itemToUse = slot->items.Top();
				itemToUse->activateItem();
				if (itemToUse->isConsumed())
				{
					slot->items.Pop();
					itemToUse->Destroy();
					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] activateItem: yes (consumed)"));
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] activateItem: yes"));
				}

				return true;
			}
		}
	}

	return false;
}

/**
 * Returns a list, that is not pointers, of all the slots.
 * 
 * @return		a TArray of all the slots (byval).
 */
TArray<FSlot> AInventory::getSlots()
{
	TArray<FSlot> allSlots;

	allSlots.SetNumZeroed(0);

	for (FSlot* slot : slots)
	{
		allSlots.Add(*slot);
	}

	return allSlots;
}