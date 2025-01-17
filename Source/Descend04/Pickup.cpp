// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "Pickup.h"


// Sets default values
APickup::APickup() : bIsConsumed(false), IsConsumedOnUse(false), IsInstant(false), CostToBuy(0), Level(0), StackSize(1), RepeatActivationDelay(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	timeLastActivated = GetWorld()->TimeSeconds;
}

// Called every frame
void APickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickup::possessed()
{
	if (IsInstant)
		activate();
}

void APickup::activate()
{
	// Confirm we're allowed to activate.
	if (GetWorld()->TimeSince(timeLastActivated) < RepeatActivationDelay) 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] activateItem: item is on cooldown!"));
		return;
	}
	
	timeLastActivated = GetWorld()->TimeSeconds;

	if (IsConsumedOnUse)
		bIsConsumed = true;

	action();
}

void APickup::action()
{

}