// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "PickupActivatable.h"
#include "PickupActivatable_Scream.generated.h"

/**
 * 
 */
UCLASS()
class DESCEND04_API APickupActivatable_Scream : public APickupActivatable
{
	GENERATED_BODY()
	
public:
	APickupActivatable_Scream();

	virtual void possessed() override;
	virtual void action() override;
};
