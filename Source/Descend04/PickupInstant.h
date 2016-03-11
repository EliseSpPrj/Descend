// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "Pickup.h"
#include "PickupInstant.generated.h"

/**
 * 
 */
UCLASS()
class DESCEND04_API APickupInstant : public APickup
{
	GENERATED_BODY()
	
public:
	APickupInstant();

	virtual void possessed() override;
	virtual void activateItem() override;
};
