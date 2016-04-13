// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "RoomWall.h"
#include "RoomTrigger.h"
#include "Room.h"
#include "Pickup.h"
#include "LevelGen.generated.h"

USTRUCT(BlueprintType)
struct FEnemySpawnDataset
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> SelectionOfEnemies;
};

UCLASS()
class DESCEND04_API ALevelGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGen();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "This function generates a new level")
		void generateRooms(uint8 count);

	UFUNCTION(BlueprintCallable, Category = "Close all door in this room")
		void lockRoom(ARoomTrigger* roomTriggered);

	UFUNCTION(BlueprintCallable, Category = "Open all door in this room")
		void unlockRoom(ARoomTrigger* roomTriggered);

private:
	TArray<AActor*> roomMeshes;			// Walls, Corners, floors etc.
	Room* rooms[256];
	int roomsCount;
	int32 currentLevel;
	float const floorLevel = 200.f;		// Height; where to spawn stuff.

	Room* getRoomAt(int32 x, int32 y);
	AActor* spawnThing(TSubclassOf<AActor> thing, float x, float y, float z = 0, float scale = 1.0, FRotator rotation = FRotator::ZeroRotator);
	int32 spawnProps(Room* room, int32 minCount, int32 maxCount);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Room floor")
	TSubclassOf<ARoomWall> roomFloor;

	UPROPERTY(EditDefaultsOnly, Category = "Room short wall")
	TSubclassOf<ARoomWall> roomShortWall;

	UPROPERTY(EditDefaultsOnly, Category = "Room long wall")
	TSubclassOf<ARoomWall> roomLongWall;

	UPROPERTY(EditDefaultsOnly, Category = "Room short door")
	TSubclassOf<ARoomWall> roomShortDoor;

	UPROPERTY(EditDefaultsOnly, Category = "Room long door")
	TSubclassOf<ARoomWall> roomLongDoor;

	UPROPERTY(EditDefaultsOnly, Category = "Room corner")
	TSubclassOf<ARoomWall> roomCorner;

	UPROPERTY(EditDefaultsOnly, Category = "Room door horizontal")
	TSubclassOf<ARoomWall> roomDoorHorizontal;

	UPROPERTY(EditDefaultsOnly, Category = "Room door vertical")
	TSubclassOf<ARoomWall> roomDoorVertical;

	UPROPERTY(EditDefaultsOnly, Category = "Room trigger")
	TSubclassOf<ARoomTrigger> roomTrigger;

	UPROPERTY(EditDefaultsOnly, Category = "Room shop")
	TSubclassOf<ARoomWall> roomShop;

	UPROPERTY(EditDefaultsOnly, Category = "Shop items")
	TArray<TSubclassOf<APickup>> shopItems;

	UPROPERTY(EditDefaultsOnly, Category = "Props")
	TArray<TSubclassOf<AActor>> props;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Amount of rooms to generate")
	uint8 numGenRooms;

	UPROPERTY(EditDefaultsOnly, Category = "Straightness factor (0 to 100)")
	uint8 straightFactor;

	UPROPERTY(EditDefaultsOnly, Category = "Scaling of the room parts (wall, floor etc)")
	float scalingFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn enemies by level (index = level, 0 = fallback)")
	TArray<FEnemySpawnDataset> enemiesByLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn bosses by level (index = level, 0 = fallback)")
	TArray<FEnemySpawnDataset> bossesByLevel;
};
