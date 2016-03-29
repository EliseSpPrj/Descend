// Copyright © 2016 Descend group. All rights reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "RoomWall.h"
#include "Room.h"
#include "LevelGen.generated.h"

UCLASS()
class DESCEND04_API ALevelGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGen();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<AActor*> roomMeshes;		// Walls, Corners and floors.
	Room* rooms[256];
	int roomsCount;

	Room* getRoomAt(int32 x, int32 y);

	UFUNCTION(BlueprintCallable, Category = "This function generates a new level")
	void generateRooms(uint8 count);
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Room corner1")
	TSubclassOf<ARoomWall> roomCorner1;

	UPROPERTY(EditDefaultsOnly, Category = "Room corner2")
	TSubclassOf<ARoomWall> roomCorner2;

	UPROPERTY(EditDefaultsOnly, Category = "Amount of rooms to generate")
	uint8 numGenRooms;

	UPROPERTY(EditDefaultsOnly, Category = "Straightness factor (0 to 100)")
	uint8 straightFactor;
};
