// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "PickupActivatable_Scream.h"

APickupActivatable_Scream::APickupActivatable_Scream()
{

}

void APickupActivatable_Scream::possessed()
{
	Super::possessed();
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] APickupActivatable_Scream possessed!"));
}

void APickupActivatable_Scream::action()
{
	Super::action();
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT(" * You scream from the top of your lungs! AAAAAAARRrrghhhh!!!!!!!!!"));
}