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
	slotAmethyst->stackSize = 99;
	slots.Add(slotAmethyst);

	
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("Inventory constructed!"));

	bool res = false;
	APickup* newItem = new APickup();

	//res = recieveItem(new APickup());

	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("recieveItem says: ") + res?FString("yes"):FString("No"));
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
		/*if(item->IsA(slot.acceptedType.StaticClass()))
		{
			return slot.handleItem(item);
		}*/
		//UClass* c = APickup::StaticClass();
		if (item->IsA(slot->acceptedType))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("recieveItem: accepted"));
			return slot->handleItem(item);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("recieveItem: false"));
	return false;
}