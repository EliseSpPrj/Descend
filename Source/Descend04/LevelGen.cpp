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
	// Delete rooms
	for (int i = 0; i < roomsCount; i++)
	{
		delete rooms[i];
		rooms[i] = nullptr;
	}

	// Delete ingame objects
	for (AActor* mesh : roomMeshes)
	{
		mesh->Destroy();	// Delete previous rooms, if any.
	}
	roomMeshes.Empty();
	roomsCount = count;

	int32 xCoord = 0, yCoord = 0, shopPos = 0;
	Room::Direction direction = Room::getRandomDirection();
	Room::Direction backDirection;
	Room::Type roomType;

	if (roomsCount > 3) shopPos = roomsCount / 2;

	// Generate layout
	for (int32 roomIndex = 0, INFILOOP = 0; roomIndex < count && INFILOOP < 10000; INFILOOP++)
	{
		// Try make a room at current position
		if(getRoomAt(xCoord, yCoord) == nullptr)
		{
			// Select room type
			if (roomIndex == 0)
				roomType = Room::Type::START;
			else if(roomIndex == shopPos)
				roomType = Room::Type::SHOP;
			else if(roomIndex == roomsCount - 1)
				roomType = Room::Type::BOSS;
			else
				roomType = Room::Type::NORMAL;

			Room* newRoom = new Room(xCoord, yCoord, roomType);
			rooms[roomIndex] = newRoom;
			roomIndex++;
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[RndGen] Added room (T:") + FString::FromInt((int)roomType) + FString::FString(") at: ") + FString::FromInt(xCoord) + FString::FString(", ") + FString::FromInt(yCoord));
		}
		
		// Walk randomly or straight depending on the straightness factor.
		if (straightFactor == 0 || FMath::RandRange(1, 100 / straightFactor) != 1 || (roomIndex == shopPos && direction == Room::Direction::SOUTH) || (roomIndex -1 == shopPos && direction == Room::Direction::NORTH)) {
			
			backDirection = Room::getInverseDirection(direction);
			bool foundRoomSpawnPosition = false;

			// Loop untill acceptable direction is found.
			for (; !foundRoomSpawnPosition; )
			{
				direction = Room::getRandomDirection();

				if (direction != backDirection)
				{
					switch (direction)
					{
						case Room::Direction::NORTH:
						{
							Room* r = getRoomAt(xCoord, yCoord);
							if (!(r && r->type == Room::Type::SHOP))	// Can't go north from a shop
							{
								foundRoomSpawnPosition = true;
							}
						}break;
						case Room::Direction::SOUTH:
						{
							Room* r = getRoomAt(xCoord, yCoord - 1);
							if (roomIndex != shopPos && !(r && r->type == Room::Type::SHOP))	// Can't go south if there's a shop there or if a shop is about to be placed as the next room.
							{
								foundRoomSpawnPosition = true;
							}
						}break;
						case Room::Direction::WEST:
							foundRoomSpawnPosition = true;
							break;
						case Room::Direction::EAST:
							foundRoomSpawnPosition = true;
							break;
					}
				}
			}
		}

		// Move towards direction
		switch (direction)
		{
			case Room::Direction::NORTH:
				yCoord++;
				break;
			case Room::Direction::SOUTH:
				yCoord--;
				break;
			case Room::Direction::WEST:
				xCoord--;
				break;
			case Room::Direction::EAST:
				xCoord++;
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
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomCorner, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}
			
			// NE corner
			if ((!roomNorth || roomNorth && !roomNorth->built) && (!roomEast || roomEast && !roomEast->built))
			{
				wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth + roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomCorner, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// SW corner
			if ((!roomSouth || roomSouth && !roomSouth->built) && (!roomWest || roomWest && !roomWest->built))
			{
				wallPosition.Set(rooms[i]->y * roomHeight - roomHeight, rooms[i]->x * roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomCorner, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// SE corner
			if ((!roomSouth || roomSouth && !roomSouth->built) && (!roomEast || roomEast && !roomEast->built))
			{
				wallPosition.Set(rooms[i]->y * roomHeight - roomHeight, rooms[i]->x * roomWidth + roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomCorner, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// North long wall
			if(roomNorth && rooms[i]->type != Room::Type::SHOP)	// Don't build a door north if this room is a shop
			{
				if (!roomNorth->built)
				{
					// There is a room north of us and no walls have been placed on it. Let's build a door to that room.
					wallPosition.Set(rooms[i]->y * roomHeight + roomHeight, rooms[i]->x * roomWidth, floorLevel);
					AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomLongDoor, wallPosition, FRotator::ZeroRotator, spawnParams);
					roomMeshes.Add(wall);

					// Let's put a door in that opening.
					wallPosition.Set(rooms[i]->y * roomHeight + roomHeight, rooms[i]->x * roomWidth, floorLevel);
					AActor* door = GetWorld()->SpawnActor<ARoomWall>(roomDoorHorizontal, wallPosition, FRotator::ZeroRotator, spawnParams);
					door->SetActorHiddenInGame(true);
					door->SetActorEnableCollision(false);
					roomMeshes.Add(door);
					rooms[i]->doors.Add(door);		// Add a handle to this door into the current room.
					roomNorth->doors.Add(door);		// Add a handle to this door into the adjacent room.
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
			if (roomSouth && roomSouth->type != Room::Type::SHOP)	// Don't build a door to shop from north
			{
				if (!roomSouth->built)
				{
					// There is a room south of us and no walls have been placed on it. Let's build a door to that room.
					wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
					AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomLongDoor, wallPosition, FRotator::ZeroRotator, spawnParams);
					roomMeshes.Add(wall);

					// Let's put a door in that opening.
					wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
					AActor* door = GetWorld()->SpawnActor<ARoomWall>(roomDoorHorizontal, wallPosition, FRotator::ZeroRotator, spawnParams);
					door->SetActorHiddenInGame(true);
					door->SetActorEnableCollision(false);
					roomMeshes.Add(door);
					rooms[i]->doors.Add(door);		// Add a handle to this door into the current room.
					roomSouth->doors.Add(door);		// Add a handle to this door into the adjacent room.
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

					// Let's put a door in that opening.
					wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
					AActor* door = GetWorld()->SpawnActor<ARoomWall>(roomDoorVertical, wallPosition, FRotator::ZeroRotator, spawnParams);
					door->SetActorHiddenInGame(true);
					door->SetActorEnableCollision(false);
					roomMeshes.Add(door);
					rooms[i]->doors.Add(door);		// Add a handle to this door into the current room.
					roomWest->doors.Add(door);		// Add a handle to this door into the adjacent room.
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

					// Let's put a door in that opening.
					wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth + roomWidth, floorLevel);
					AActor* door = GetWorld()->SpawnActor<ARoomWall>(roomDoorVertical, wallPosition, FRotator::ZeroRotator, spawnParams);
					door->SetActorHiddenInGame(true);
					door->SetActorEnableCollision(false);
					roomMeshes.Add(door);
					rooms[i]->doors.Add(door);		// Add a handle to this door into the current room.
					roomEast->doors.Add(door);		// Add a handle to this door into the adjacent room.
				}
			}
			else
			{
				// There is no room east of us. Let's build a wall.
				wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth + roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomShortWall, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);
			}

			// Handle room content
			if (rooms[i]->type == Room::Type::SHOP)
			{
				// Spawn in the shop walls
				wallPosition.Set(rooms[i]->y * roomHeight + (roomHeight - 234 - 60), rooms[i]->x * roomWidth, floorLevel);
				AActor* wall = GetWorld()->SpawnActor<ARoomWall>(roomShop, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(wall);

				// Spawn in the shop items	// TODO: how many? item weight
				for (TSubclassOf<APickup> shopItem : shopItems)
				{
					wallPosition.Set(rooms[i]->y * roomHeight + 290, rooms[i]->x * roomWidth - 250 + (0 * 250), floorLevel + 40);
					AActor* newItem = GetWorld()->SpawnActor<APickup>(shopItem, wallPosition, FRotator::ZeroRotator, spawnParams);
					roomMeshes.Add(newItem);
				}
			}
			else if (rooms[i]->type == Room::Type::NORMAL)
			{
				// Spawn in the room trigger
				wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
				AActor* trigger = GetWorld()->SpawnActor<ARoomTrigger>(roomTrigger, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(trigger);

				ARoomTrigger* trig = Cast<ARoomTrigger>(trigger);
				trig->setPosition(rooms[i]->x, rooms[i]->y);
			}
			else if (rooms[i]->type == Room::Type::BOSS)
			{
				// Spawn in the room trigger
				wallPosition.Set(rooms[i]->y * roomHeight, rooms[i]->x * roomWidth, floorLevel);
				AActor* trigger = GetWorld()->SpawnActor<ARoomTrigger>(roomTrigger, wallPosition, FRotator::ZeroRotator, spawnParams);
				roomMeshes.Add(trigger);

				ARoomTrigger* trig = Cast<ARoomTrigger>(trigger);
				trig->setPosition(rooms[i]->x, rooms[i]->y);
			}

			rooms[i]->built = true;
		}
	}
	
	
}

void ALevelGen::lockRoom(ARoomTrigger* roomTriggered)
{
	int32 x = roomTriggered->getPosition().X;
	int32 y = roomTriggered->getPosition().Y;

	Room* room = getRoomAt(x, y);

	for (AActor* door : room->doors)
	{
		door->SetActorHiddenInGame(false);
		door->SetActorEnableCollision(true);
	}

	FVector position;
	float floorLevel = 200.f;
	float roomWidth = 1140;
	float roomHeight = 780;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;

	if (room->type == Room::Type::NORMAL)
	{
		for (TSubclassOf<ACharacter> enemy : enemies)
		{
			position.Set(room->y * roomHeight, room->x * roomWidth, floorLevel + 40);
			//AActor* newItem = GetWorld()->SpawnActor<ACharacter>(enemy, position, FRotator::ZeroRotator, spawnParams);
		}
	}
	else if (room->type == Room::Type::BOSS)
	{
		for (TSubclassOf<ACharacter> enemy : enemies)
		{
			position.Set(room->y * roomHeight, room->x * roomWidth, floorLevel + 40);
			//AActor* newItem = GetWorld()->SpawnActor<ACharacter>(enemy, position, FRotator::ZeroRotator, spawnParams);
			//AActor* newItem2 = GetWorld()->SpawnActor<ACharacter>(enemy, position, FRotator::ZeroRotator, spawnParams);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[RndGen] LOCK room: ") + FString::FromInt(x) + FString::FString(", ") + FString::FromInt(y));
}

void ALevelGen::unlockRoom(ARoomTrigger* room)
{
	int32 x = room->getPosition().X;
	int32 y = room->getPosition().Y;

	for (AActor* door : getRoomAt(x, y)->doors)
	{
		door->SetActorHiddenInGame(true);
		door->SetActorEnableCollision(false);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[RndGen] UNLOCK room: ") + FString::FromInt(x) + FString::FString(", ") + FString::FromInt(y));
}