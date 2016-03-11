// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class DESCEND04_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual bool isConsumedOnUse() { return bIsConsumedOnUse; };
	virtual bool isConsumed() { return bIsConsumed; };
	virtual void possessed();
	virtual void activateItem();
	
private:
	bool bIsConsumed;
	bool bIsConsumedOnUse;
};
