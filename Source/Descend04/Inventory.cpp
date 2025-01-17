// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "Inventory.h"
#include "PickupMoney.h"
#include "PickupActivatable.h"

AInventory::AInventory()
{
	PrimaryActorTick.bCanEverTick = false;

	// Let's set up the slots for the inventory
	FSlot* newSlot;

	newSlot = new FSlot(APickupMoney::StaticClass());
	newSlot->overflowAction = FSlot::OverflowAction::DESTROY;
	newSlot->stackSize = 99;
	slots.Add(newSlot);

	newSlot = new FSlot(APickupActivatable::StaticClass());
	newSlot->overflowAction = FSlot::OverflowAction::SWAP;
	newSlot->stackSize = 1;
	slots.Add(newSlot);
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] Inventory constructed!"));
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
					if (item->currencyItem->IsChildOf(slotMoney->acceptedType))
					{
						if (slotMoney->items.Num() < item->CostToBuy)
						{
							// Can't afford it!
							//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] receiveItem: can't afford"));
							return false;
						}
						else
						{
							// Deduct the money
							slotMoney->items.SetNum(slotMoney->items.Num() - item->CostToBuy);
							paid = true;
							break;
						}
						
					}
				}
				
				if (!paid) {
					// No slots with money found, can't pay!
					//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] receiveItem: no money"));
					return false;
				}
			}

			// Try to handle the item.
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] receiveItem: handling.."));
			
			bool bHandled = slot->handleItem(item);

			return bHandled;
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] receiveItem: no slot for that item"));
	return false;
}

/**
 * Loops through the slots looking for a slot that has the item type given, then activates an item in that slot (if any).
 * 
 * @param		item	the item to look for and activate.
 * @return				returns true if a matching item was found, otherwise false.
 */
bool AInventory::activateItem(UClass* itemType, bool activateAll)
{
	bool foundItems = false;

	for (FSlot* slot : slots)
	{
		if (itemType == slot->acceptedType)
		{
			if (slot->items.Num() > 0)
			{
				for (APickup* itemToUse : slot->items)
				{
					itemToUse->activate();
					if (itemToUse->isConsumed())
					{
						slot->items.Pop();
						itemToUse->Destroy();
						//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] activateItem: yes (consumed)"));
					}
					else
					{
						//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] activateItem: yes"));
					}

					if (!activateAll) return true;
					foundItems = true;
				}
			}
		}
	}

	return foundItems;
}

/**
 * Returns a list of accepted types.
 * 
 * @return		list of types the inventory accepts/has a slot for.
 */
TArray<UClass*> AInventory::getAcceptedTypes()
{
	TArray<UClass*> list;

	for (FSlot* slot : slots)
	{
		list.Add(slot->acceptedType);
	}

	return list;
}

/**
* Returns the amount of items of specified type there are in the inventory.
*
* @return		number of items in the stack, or -1 if the specified item was not found.
*/
int32 AInventory::getItemCount(UClass* itemType)
{
	for (FSlot* slot : slots)
	{
		if (itemType == slot->acceptedType)
			return slot->items.Num();
	}

	return -1;
}