// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "Pickup.h"
#include "PickupTrinket.generated.h"

/**
 * 
 */
UCLASS()
class DESCEND04_API APickupTrinket : public APickup
{
	GENERATED_BODY()
	
public:
	APickupTrinket();

	virtual void possessed() override;
	virtual void action() override;
};
