// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

UCLASS()
class DESCEND04_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
