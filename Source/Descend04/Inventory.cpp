// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "Inventory.h"


// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventory::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

