// Copyright © 2016 Descend group. All rights reserved.

#pragma once

/**
 * A class for storing room info such as coords and type, and some helper functions.
 */

class DESCEND04_API Room
{
public:
	enum class Direction { NORTH, SOUTH, WEST, EAST };
	enum class Type { START, NORMAL, SHOP, BOSS };

	Room(int32 x, int32 y, Room::Type type);
	~Room();

	bool built;
	bool locked;
	int32 x;
	int32 y;
	float const roomWidth = 1140;
	float const roomHeight = 780;
	Room::Type type;
	TArray<AActor*> doors;

	static Direction getInverseDirection(Direction direction);
	static Direction getRandomDirection();
};