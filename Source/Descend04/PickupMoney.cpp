// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "PickupMoney.h"

APickupMoney::APickupMoney()
{

}

void APickupMoney::possessed()
{
	Super::possessed();
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] APickupMoney possessed!"));
}

void APickupMoney::action()
{
	Super::action();
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[INV] APickupMoney activated!"));
}