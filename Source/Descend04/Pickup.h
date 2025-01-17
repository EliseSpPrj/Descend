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
	virtual void activate();		// Tries to activate the item action
	virtual void action();			// What the item does when activated
	
	UPROPERTY(Category = "Pickup properties", EditAnywhere, BlueprintReadWrite)
		bool IsConsumedOnUse;

	UPROPERTY(Category = "Pickup properties", EditAnywhere, BlueprintReadWrite)
		bool IsInstant;

	UPROPERTY(Category = "Pickup properties", EditAnywhere, BlueprintReadWrite)
		int32 CostToBuy;

	UPROPERTY(Category = "Pickup properties", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APickup> currencyItem;		// Sets what item is used as currency.
	
	UPROPERTY(Category = "Pickup properties", EditAnywhere, BlueprintReadWrite)
		uint8 Level;

	UPROPERTY(Category = "Pickup properties", EditAnywhere, BlueprintReadWrite)
		int32 StackSize;

	UPROPERTY(Category = "Pickup properties", EditAnywhere, BlueprintReadWrite)
		float RepeatActivationDelay;

private:
	bool bIsConsumed;
	float timeLastActivated;
};
