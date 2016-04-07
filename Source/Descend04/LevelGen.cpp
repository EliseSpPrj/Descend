// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "LevelGen.h"

// Sets default values
ALevelGen::ALevelGen() : scalingFactor(1.f)
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
		mesh->Destroy();	// Delete previous room walls etc, if any.
	}
	roomMeshes.Empty();
	roomsCount = count;

	int32 xCoord = 0, yCoord = 0, shopPos = 0;
	Room::Direction direction = Room::getRandomDirection();
	Room::Direction backDirection;
	Room::Type roomType;

	if (roomsCount > 3) shopPos = roomsCount / 2;

	/*
	 #
	 #  Generate layout
	 #
	 */
	for (int32 roomIndex = 0, INFILOOP = 0; roomIndex < roomsCount && INFILOOP < 10000; INFILOOP++)
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

			Room* newRoom = new Room(xCoord, yCoord, roomType, scalingFactor);
			rooms[roomIndex] = newRoom;
			roomIndex++;
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[RndGen] Added room (T:") + FString::FromInt((int)roomType) + FString::FString(") at: ") + FString::FromInt(xCoord) + FString::FString(", ") + FString::FromInt(yCoord));
		}
		
		// Walk randomly or straight depending on the straightness factor.
		if (straightFactor == 0 || FMath::RandRange(1, 100 / straightFactor) != 1 || ((getRoomAt(xCoord, yCoord - 1) && getRoomAt(xCoord, yCoord - 1)->type == Room::Type::SHOP) && direction == Room::Direction::SOUTH) || ((getRoomAt(xCoord, yCoord) && getRoomAt(xCoord, yCoord)->type == Room::Type::SHOP) && direction == Room::Direction::NORTH)) {
			
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

	/*
	 #
	 #  Build the rooms
	 #
	 */
	for (int i = roomsCount - 1; i >= 0; i--)
	{
		bool builtBossDoor = false;
		if(rooms[i] && !rooms[i]->built)
		{
			// Get adjacent rooms
			Room* roomNorth = getRoomAt(rooms[i]->x, rooms[i]->y + 1);		// North = y+1
			Room* roomSouth = getRoomAt(rooms[i]->x, rooms[i]->y - 1);		// South = y-1
			Room* roomWest = getRoomAt(rooms[i]->x - 1, rooms[i]->y);		// West = x-1
			Room* roomEast = getRoomAt(rooms[i]->x + 1, rooms[i]->y);		// East = x+1

			// Floor (notice: x & y are reversed in the world)
			AActor* wall = spawnThing(roomFloor, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
			roomMeshes.Add(wall);

			// NW corner
			if ((!roomNorth || !roomNorth->built) && (!roomWest || !roomWest->built))
			{
				AActor* wall = spawnThing(roomCorner, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(wall);
			}
			
			// NE corner
			if ((!roomNorth || !roomNorth->built) && (!roomEast || !roomEast->built))
			{
				AActor* wall = spawnThing(roomCorner, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth + rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(wall);
			}

			// SW corner
			if ((!roomSouth || !roomSouth->built) && (!roomWest || !roomWest->built))
			{
				AActor* wall = spawnThing(roomCorner, rooms[i]->y * rooms[i]->roomHeight - rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(wall);
			}

			// SE corner
			if ((!roomSouth || !roomSouth->built) && (!roomEast || !roomEast->built))
			{
				AActor* wall = spawnThing(roomCorner, rooms[i]->y * rooms[i]->roomHeight - rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth + rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(wall);
			}

			// North long wall
			if(roomNorth && rooms[i]->type != Room::Type::SHOP && (!builtBossDoor))	// Don't build a door north if this room is a shop
			{
				if (!roomNorth->built)
				{
					// There is a room north of us and no walls have been placed on it. Let's build a door to that room.
					AActor* wall = spawnThing(roomLongDoor, rooms[i]->y * rooms[i]->roomHeight + rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
					roomMeshes.Add(wall);

					// Let's put a door in that opening.
					AActor* door = spawnThing(roomDoorHorizontal, rooms[i]->y * rooms[i]->roomHeight + rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
					door->SetActorHiddenInGame(true);
					door->SetActorEnableCollision(false);
					roomMeshes.Add(door);
					rooms[i]->doors.Add(door);		// Add a handle to this door into the current room.
					roomNorth->doors.Add(door);		// Add a handle to this door into the adjacent room.

					if(rooms[i]->type == Room::Type::BOSS) builtBossDoor = true;	// This works as we're looping backwards and the last room is the boss room.
				}
			}
			else
			{
				// There is no room north of us. Let's build a wall.
				AActor* wall = spawnThing(roomLongWall, rooms[i]->y * rooms[i]->roomHeight + rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(wall);
			}

			// South long wall
			if (roomSouth && roomSouth->type != Room::Type::SHOP && (!builtBossDoor))	// Don't build a door to shop from north
			{
				if (!roomSouth->built)
				{
					// There is a room south of us and no walls have been placed on it. Let's build a door to that room.
					AActor* wall = spawnThing(roomLongDoor, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
					roomMeshes.Add(wall);

					// Let's put a door in that opening.
					AActor* door = spawnThing(roomDoorHorizontal, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
					door->SetActorHiddenInGame(true);
					door->SetActorEnableCollision(false);
					roomMeshes.Add(door);
					rooms[i]->doors.Add(door);		// Add a handle to this door into the current room.
					roomSouth->doors.Add(door);		// Add a handle to this door into the adjacent room.

					if (rooms[i]->type == Room::Type::BOSS) builtBossDoor = true;
				}
			}
			else
			{
				// There is no room south of us. Let's build a wall.
				AActor* wall = spawnThing(roomLongWall, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(wall);
			}

			// West short wall
			if (roomWest && (!builtBossDoor))
			{
				if (!roomWest->built)
				{
					// There is a room west of us and no walls have been placed on it. Let's build a door to that room.
					AActor* wall = spawnThing(roomShortDoor, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
					roomMeshes.Add(wall);

					// Let's put a door in that opening.
					AActor* door = spawnThing(roomDoorVertical, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
					door->SetActorHiddenInGame(true);
					door->SetActorEnableCollision(false);
					roomMeshes.Add(door);
					rooms[i]->doors.Add(door);		// Add a handle to this door into the current room.
					roomWest->doors.Add(door);		// Add a handle to this door into the adjacent room.

					if (rooms[i]->type == Room::Type::BOSS) builtBossDoor = true;
				}
			}
			else
			{
				// There is no room west of us. Let's build a wall.
				AActor* wall = spawnThing(roomShortWall, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(wall);
			}

			// East short wall
			if (roomEast && (!builtBossDoor))
			{
				if (!roomEast->built)
				{
					// There is a room east of us and no walls have been placed on it. Let's build a door to that room.
					AActor* wall = spawnThing(roomShortDoor, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth + rooms[i]->roomWidth, 0, scalingFactor);
					roomMeshes.Add(wall);

					// Let's put a door in that opening.
					AActor* door = spawnThing(roomDoorVertical, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth + rooms[i]->roomWidth, 0, scalingFactor);
					door->SetActorHiddenInGame(true);
					door->SetActorEnableCollision(false);
					roomMeshes.Add(door);
					rooms[i]->doors.Add(door);		// Add a handle to this door into the current room.
					roomEast->doors.Add(door);		// Add a handle to this door into the adjacent room.

					if (rooms[i]->type == Room::Type::BOSS) builtBossDoor = true;
				}
			}
			else
			{
				// There is no room east of us. Let's build a wall.
				AActor* wall = spawnThing(roomShortWall, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth + rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(wall);
			}

			/*
			 #
			 #  Handle room content
			 #
			 */
			if (rooms[i]->type == Room::Type::SHOP)
			{
				// Spawn in the shop walls
				AActor* wall = spawnThing(roomShop, rooms[i]->y * rooms[i]->roomHeight + (rooms[i]->roomHeight - 234 * scalingFactor - 60 * scalingFactor), rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(wall);


				// Spawn in the shop items
				int32 amountToSpawn = 3;
				TArray<TSubclassOf<APickup>> spawnItems = TArray<TSubclassOf<APickup>>(shopItems);

				for (int32 j = 0; j < amountToSpawn && spawnItems.Num() > 0; j++)
				{
					TSubclassOf<APickup> spawnItem = spawnItems[FMath::RandRange(0, spawnItems.Num() - 1)];

					AActor* newItem = spawnThing(spawnItem, rooms[i]->y * rooms[i]->roomHeight + (240 * scalingFactor), rooms[i]->x * rooms[i]->roomWidth - ((230 + (j * 230)) * scalingFactor), 40.f);
					roomMeshes.Add(newItem);

					spawnItems.Remove(spawnItem);
					//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, TEXT("[RndGen] Spawned shop item!"));
				}
			}
			else if (rooms[i]->type == Room::Type::NORMAL)
			{
				// Spawn in the room trigger
				AActor* newTrigger = spawnThing(roomTrigger, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(newTrigger);

				ARoomTrigger* trigger = Cast<ARoomTrigger>(newTrigger);
				if (trigger)
				{
					trigger->setPosition(rooms[i]->x, rooms[i]->y);
					trigger->type = (uint8)rooms[i]->type;
				}
			}
			else if (rooms[i]->type == Room::Type::BOSS)
			{
				// Spawn in the room trigger
				AActor* newTrigger = spawnThing(roomTrigger, rooms[i]->y * rooms[i]->roomHeight, rooms[i]->x * rooms[i]->roomWidth, 0, scalingFactor);
				roomMeshes.Add(newTrigger);

				ARoomTrigger* trigger = Cast<ARoomTrigger>(newTrigger);
				if (trigger)
				{
					trigger->setPosition(rooms[i]->x, rooms[i]->y);
					trigger->type = (uint8)rooms[i]->type;
				}
			}
			else if (rooms[i]->type == Room::Type::START)
			{

			}

			rooms[i]->built = true;
		}
	}
	
	
}

/**
 * Closes all doors connected to this room and spawns in the mobs.
 *
 * @param	roomTriggered	the room to be locked.
 */
void ALevelGen::lockRoom(ARoomTrigger* roomTriggered)
{
	int32 x = roomTriggered->getPosition().X;
	int32 y = roomTriggered->getPosition().Y;

	Room* room = getRoomAt(x, y);
	if (!room) return;

	if (room->locked) return;
	room->locked = true;

	for (AActor* door : room->doors)
	{
		door->SetActorHiddenInGame(false);
		door->SetActorEnableCollision(true);
	}

	if (room->type == Room::Type::NORMAL)
	{
		for (TSubclassOf<ACharacter> enemy : enemies)
		{
			spawnThing(enemy, room->y * room->roomHeight, room->x * room->roomWidth, 40.f);
		}
	}
	else if (room->type == Room::Type::BOSS)
	{
		for (TSubclassOf<ACharacter> enemy : enemies)
		{
			spawnThing(enemy, room->y * room->roomHeight, room->x * room->roomWidth, 40.f);
			spawnThing(enemy, room->y * room->roomHeight, room->x * room->roomWidth, 40.f);
			spawnThing(enemy, room->y * room->roomHeight, room->x * room->roomWidth, 40.f);
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[RndGen] LOCK room: ") + FString::FromInt(x) + FString::FString(", ") + FString::FromInt(y));
}

/**
 * Opens all doors connected to this room.
 *
 * @param	roomTriggered	the room to be unlocked.
 */
void ALevelGen::unlockRoom(ARoomTrigger* roomTriggered)
{
	int32 x = roomTriggered->getPosition().X;
	int32 y = roomTriggered->getPosition().Y;

	Room* room = getRoomAt(x, y);

	for (AActor* door : room->doors)
	{
		door->SetActorHiddenInGame(true);
		door->SetActorEnableCollision(false);
	}

	room->locked = false;
	
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("[RndGen] UNLOCK room: ") + FString::FromInt(x) + FString::FString(", ") + FString::FromInt(y));
}

/**
 * Spawns an actor into the world.
 *
 * @param	thing	the actor to be spawned in.
 * @param	x		horizontal location in the world
 * @param	y		vertical location in the world
 * @param	z		height location in the world above "floorLevel" (200)
 * @param	scale	scale the actors mesh
 * @return			returns a reference to the actor spawned or nullptr if spawning fails.
 */
AActor* ALevelGen::spawnThing(TSubclassOf<AActor> thing, float x, float y, float z, float scale)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;

	FVector position;
	position.Set(x, y, floorLevel + z);

	AActor* newThing = GetWorld()->SpawnActor<AActor>(thing, position, FRotator::ZeroRotator, spawnParams);
	
	if (scale != 1.f)
	{
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(newThing->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (StaticMeshComponent)
		{
			FTransform Transform = StaticMeshComponent->GetComponentTransform();

			Transform = Transform.GetScaled(scale);		// Resize object
			//Transform.ScaleTranslation(scale);			// Re-position object

			StaticMeshComponent->SetWorldTransform(Transform);
		}
		else
		{
			UBoxComponent* CollisionComponent = Cast<UBoxComponent>(newThing->GetComponentByClass(UBoxComponent::StaticClass()));
			if (CollisionComponent)
			{
				FTransform Transform = CollisionComponent->GetComponentTransform();

				Transform = Transform.GetScaled(scale);		// Resize object
				//Transform.ScaleTranslation(scale);			// Re-position object

				CollisionComponent->SetWorldTransform(Transform);
			}
		}
	}

	return newThing;
}