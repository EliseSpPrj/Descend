// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "Pickup.h"
#include "PickupActivatable.generated.h"

/**
 * 
 */
UCLASS()
class DESCEND04_API APickupActivatable : public APickup
{
	GENERATED_BODY()
	
public:
	APickupActivatable();

	virtual void possessed() override;
	virtual void action() override;
};
