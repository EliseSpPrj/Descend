// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "RoomTrigger.h"


// Sets default values
ARoomTrigger::ARoomTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ARoomTrigger::setPosition(int32 x, int32 y)
{
	xPosition = x;
	yPosition = y;
}

FIntPoint ARoomTrigger::getPosition()
{
	return FIntPoint(xPosition, yPosition);
}