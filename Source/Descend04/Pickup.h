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

	virtual bool isConsumed() { return bIsConsumed; };
	virtual void possessed();
	virtual void activateItem();
	
	UPROPERTY(Category = Properties, EditAnywhere, BlueprintReadWrite)
		bool IsConsumedOnUse;

	UPROPERTY(Category = Properties, EditAnywhere, BlueprintReadWrite)
		bool IsInstant;

	UPROPERTY(Category = Properties, EditAnywhere, BlueprintReadWrite)
		int32 CostToBuy;
	
	UPROPERTY(Category = Properties, EditAnywhere, BlueprintReadWrite)
		uint8 Level;

	UPROPERTY(Category = Properties, EditAnywhere, BlueprintReadWrite)
		int32 StackSize;

private:
	bool bIsConsumed;
};
