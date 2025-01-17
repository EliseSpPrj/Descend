// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "LootMaster.generated.h"

UENUM(BlueprintType)
enum class TableName : uint8
{
	Mob			UMETA(DisplayName = "Mob"),
	Boss		UMETA(DisplayName = "Boss")
};

UCLASS()
class DESCEND04_API ALootMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootMaster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot from normal mobs")
	TArray<TSubclassOf<AActor>> mobDrops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot from bosses")
	TArray<TSubclassOf<AActor>> bossDrops;

	UFUNCTION(BlueprintCallable, Category = "Return a list of loot according to the loot table")
	TArray<TSubclassOf<AActor>> getLootFromTable(TableName lootTable);

private:
	TArray<TSubclassOf<AActor>> selectItems(TArray<TSubclassOf<AActor>> table, int32 numberOfItemsToSelect, float dropChange = 1.f);
};
