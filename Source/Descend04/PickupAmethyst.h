// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "Pickup.h"
#include "PickupAmethyst.generated.h"

/**
 * 
 */
UCLASS()
class DESCEND04_API APickupAmethyst : public APickup
{
	GENERATED_BODY()
	
public:
	APickupAmethyst();

	virtual void possessed() override;
	virtual void activateItem() override;
};
