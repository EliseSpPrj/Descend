// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "PickupActivatable.h"

APickupActivatable::APickupActivatable()
{

}

void APickupActivatable::possessed()
{
	Super::possessed();
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] APickupActivatable possessed!"));
}

void APickupActivatable::action()
{
	Super::action();
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] APickupActivatable activated!"));
}