// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "PickupAmethyst.h"

APickupAmethyst::APickupAmethyst()
{
	
}

void APickupAmethyst::possessed()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] APickupAmethyst possessed!"));
}

void APickupAmethyst::activateItem()
{

}