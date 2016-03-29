// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "LevelGen.h"

// Sets default values
ALevelGen::ALevelGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	roomMeshes.SetNumZeroed(0);
}

// Called when the game starts or when spawned
void ALevelGen::BeginPlay()
{
	Super::BeginPlay();

	generateRooms(numGenRooms);
}

/**
 * Returns a reference to a Room object that is located at the coords x and y.
 * 
 * @param	x	horizontal position in the grid
 * @param	y	vertical position in the grid
 * @return		Reference to the Room at the specified position
 */
Room* ALevelGen::getRoomAt(int32 x, int32 y)
{
	for (int i = 0; i < roomsCount; i++)
	{
		if(rooms[i] && rooms[i]->x == x && rooms[i]->y == y)
		{
			return rooms[i];
		}
	}

	return nullptr;
}

/**
 * Generates (count) amount of rooms in a grid by randomly "walking" on it.
 * Builds the grid of rooms in the world with meshes defined in by UPROPERTies in the .h file.
 * UPROPERTY numGenRooms defines how many rooms to create.
 * UPROPERTY straightFactor is a factor that defines how straight or how random the "walking" is.
 * 
 * @param	count	how many rooms to generate, max 256
 */
void ALevelGen::generateRooms(uint8 count)
{
	// Reset and setup variables
	for (AActor* mesh : roomMeshes)
	{
		mesh->Destroy();	// Delete previous rooms, if any.
	}
	roomMeshes.Empty();
	roomsCount = count;

	int32 xCoord = 0, yCoord = 0;
	Room::Direction direction = Room::getRandomDirection();
	Room::Direction backDirection;

	// Generate layout
	for (int32 roomIndex = 0, INFILOOP = 0; roomIndex < count && INFILOOP < 10000; INFILOOP++)
	{
		// Try make a room at current position
		if(getRoomAt(xCoord, yCoord) == nullptr)
		{
			Room::Type roomType = roomIndex == 0 ? Room::Type::START : Room::Type::NORMAL;

			Room* newRoom = new Room(xCoord, yCoord, roomType);
			rooms[roomIndex] = newRoom;
			roomIndex++;
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[RndGen] Added room at: ") + FString::FromInt(xCoord) + FString::FString(", ") + FString::FromInt(yCoord));
		}
		
		// Walk randomly or straight depending on the straightness factor.
		if (straightFactor == 0 || FMath::RandRange(1, 100 / straightFactor) != 1) {
			// Walk any direction but backwards
			backDirection = Room::getInverseDirection(direction);
			for (direction = Room::getRandomDirection(); direction == backDirection; direction = Room::getRandomDirection()) {}
		}

		switch(direction)
		{
		case Room::Direction::NORTH:
			xCoord++;
			break;
		case Room::Direction::SOUTH:
			xCoord--;
			break;
		case Room::Direction::WEST:
			yCoord--;
			break;
		case Room::Direction::EAST:
			yCoord++;
			break;
		}
	}

	// Build the rooms
	FVector wallPosition;
	wallPosition.Set(0,0,0);

	float floorLevel = 200.f;
	float roomWidth = 1140;
	float roomHeight = 780;

	Room* roomNorth = nullptr;
	Room* roomSouth = nullptr;
	Room* roomWest = nullptr;
	Room* roomEast = nullptr;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;

	for (int i = 0; i < roomsCount; i++)
	{
		if(rooms[i])
		{
			// Get adjacent rooms
			roomNorth = getRoomAt(rooms[i]->x, rooms[i]->y + 1);	// North = y+1
			roomSouth = getRoomAt(rooms[i]->x, rooms[i]->y - 1);	// South = y-1
			roomWest = getRoomAt(rooms[i]->x - 1, rooms[i]->y);		// West = x-1
			roomEast = getRoomAt(rooms[i]->x + 1, rooms[i]->y);		// East = x+1

			// Floor (notice: x & y are reversed in the world)
			wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
			AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomFloor, wallPosition, FRotator::ZeroRotator, spawnParams);
			roomMeshes.Add(wall);

			// NW corner
			if ((!roomNorth || roomNorth && !roomNorth->built) && (!roomWest || roomWest && !roomWest->built))
			{
				wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomCorner1, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}
			
			// NE corner
			if ((!roomNorth || roomNorth && !roomNorth->built) && (!roomEast || roomEast && !roomEast->built))
			{
				wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth + roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomCorner1, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// SW corner
			if ((!roomSouth || roomSouth && !roomSouth->built) && (!roomWest || roomWest && !roomWest->built))
			{
				wallPosition.Set(rooms[i]->y * roomHeight - roomHeight, rooms[i]->x * roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomCorner1, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// SE corner
			if ((!roomSouth || roomSouth && !roomSouth->built) && (!roomEast || roomEast && !roomEast->built))
			{
				wallPosition.Set(rooms[i]->y * roomHeight - roomHeight, rooms[i]->x * roomWidth + roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomCorner1, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// North long wall
			if(roomNorth)
			{
				if (!roomNorth->built)
				{
					// There is a room north of us and no walls have been placed on it. Let's build a door to that room.
					wallPosition.Set(rooms[i]->y * roomHeight + roomHeight, rooms[i]->x * roomWidth, floorLevel);
					AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomLongDoor, wallPosition, FRotator::ZeroRotator, spawnParams);
					roomMeshes.Add(wall);
				}
			}
			else
			{
				// There is no room north of us. Let's build a wall.
				wallPosition.Set(rooms[i]->y * roomHeight + roomHeight, rooms[i]->x * roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomLongWall, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// South long wall
			if (roomSouth)
			{
				if (!roomSouth->built)
				{
					// There is a room south of us and no walls have been placed on it. Let's build a door to that room.
					wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
					AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomLongDoor, wallPosition, FRotator::ZeroRotator, spawnParams);
					roomMeshes.Add(wall);
				}
			}
			else
			{
				// There is no room south of us. Let's build a wall.
				wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomLongWall, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// West short wall
			if (roomWest)
			{
				if (!roomWest->built)
				{
					// There is a room west of us and no walls have been placed on it. Let's build a door to that room.
					wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
					AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomShortDoor, wallPosition, FRotator::ZeroRotator, spawnParams);
					roomMeshes.Add(wall);
				}
			}
			else
			{
				// There is no room west of us. Let's build a wall.
				wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomShortWall, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// East short wall
			if (roomEast)
			{
				if (!roomEast->built)
				{
					// There is a room east of us and no walls have been placed on it. Let's build a door to that room.
					wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth + roomWidth, floorLevel);
					AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomShortDoor, wallPosition, FRotator::ZeroRotator, spawnParams);
					roomMeshes.Add(wall);
				}
			}
			else
			{
				// There is no room east of us. Let's build a wall.
				wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth + roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomShortWall, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			rooms[i]->built = true;
		}
	}
	
	// Clean up
	for (int i = 0; i < roomsCount; i++)
	{
		delete rooms[i];
		rooms[i] = nullptr;
	}
	roomsCount = 0;
}