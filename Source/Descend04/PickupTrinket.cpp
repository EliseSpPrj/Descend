// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "PickupTrinket.h"

APickupTrinket::APickupTrinket()
{

}

void APickupTrinket::possessed()
{
	Super::possessed();
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] APickupTrinket possessed!"));
}

void APickupTrinket::action()
{
	Super::action();
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] APickupTrinket activated!"));
}