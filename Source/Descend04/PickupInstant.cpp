// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "PickupInstant.h"

APickupInstant::APickupInstant()
{

}

void APickupInstant::possessed()
{
	activateItem();		// An instant item is immediately activated once picked up.
}

void APickupInstant::activateItem()
{

}