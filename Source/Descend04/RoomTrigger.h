// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "RoomTrigger.generated.h"

UCLASS()
class DESCEND04_API ARoomTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomTrigger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void setPosition(int32 x, int32 y);
	FIntPoint getPosition();

private:
	int32 xPosition;
	int32 yPosition;
};
