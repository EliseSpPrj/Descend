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
	int32 x;
	int32 y;
	Room::Type type;

	static Direction getInverseDirection(Direction direction);
	static Direction getRandomDirection();
};