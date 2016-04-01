// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "Pickup.h"
#include "PickupMoney.generated.h"

/**
 * 
 */
UCLASS()
class DESCEND04_API APickupMoney : public APickup
{
	GENERATED_BODY()
	
public:
	APickupMoney();

	virtual void possessed() override;
	virtual void action() override;
};
