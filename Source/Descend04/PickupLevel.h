// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "Pickup.h"
#include "PickupLevel.generated.h"

/**
 * 
 */
UCLASS()
class DESCEND04_API APickupLevel : public APickup
{
	GENERATED_BODY()
	
public:
	APickupLevel();

	virtual void possessed() override;
	virtual void activateItem() override;
private:
	uint8 level;
	
};
