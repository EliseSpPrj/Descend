// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "LootMaster.h"


// Sets default values
ALootMaster::ALootMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALootMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<TSubclassOf<AActor>> ALootMaster::getLootFromTable(TableName lootTable)
{
	switch (lootTable)
	{
		case TableName::Boss:
		{
			return selectItems(bossDrops, 1);
		}
		break;
		case TableName::Mob:
		{
			return selectItems(mobDrops, 1, 0.5);
		}
		break;
	}

	return TArray<TSubclassOf<AActor>>();
}

TArray<TSubclassOf<AActor>> ALootMaster::selectItems(TArray<TSubclassOf<AActor>> table, int32 numberOfItemsToSelect, float dropChange)
{
	// This used to be a weighted loot table system but then Unreal happened :( Can't use TMultiMap in blueprints.
	TArray<TSubclassOf<AActor>> selectedDrops;
	TArray<TSubclassOf<AActor>> allDrops = TArray<TSubclassOf<AActor>>(table);
	selectedDrops.SetNumZeroed(0);

	if (FMath::RandRange(0, 100) / 100.f > dropChange) return selectedDrops;

	for (int i = 0; i < numberOfItemsToSelect && allDrops.Num() > 0; i++)
	{
		TSubclassOf<AActor> item = allDrops[FMath::RandRange(0, allDrops.Num() - 1)];
		selectedDrops.Add(item);
		allDrops.Remove(item);
	}

	return selectedDrops;
}