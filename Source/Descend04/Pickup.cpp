// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "Pickup.h"


// Sets default values
APickup::APickup() : bIsConsumed(false), IsConsumedOnUse(false), IsInstant(false), CostToBuy(0), Level(0), StackSize(1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickup::possessed()
{
	if (IsInstant)
		activateItem();
}

void APickup::activateItem()
{
	if (IsConsumedOnUse)
		bIsConsumed = true;
}
