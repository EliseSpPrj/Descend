// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "Pickup.h"


// Sets default values
APickup::APickup() : bIsConsumedOnUse(false), bIsConsumed(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

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
}

void APickup::activateItem()
{
}
