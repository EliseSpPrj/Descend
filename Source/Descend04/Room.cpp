// Copyright © 2016 Descend group. All rights reserved.

#include "Descend04.h"
#include "Room.h"

Room::Room(int32 x, int32 y, Room::Type type) : x(x), y(y), type(type), built(false)
{
}

Room::~Room()
{
}

/**
 * Returns a Direction that is cardinally inverse of the input direction.
 * 
 * @param	direction	the direction used to lookup the inversion of
 * @return				the reverse direction of the input
 */
Room::Direction Room::getInverseDirection(Direction direction)
{
	switch (direction)
	{
	case Direction::NORTH:
		return Direction::SOUTH;
		break;
	case Direction::SOUTH:
		return Direction::NORTH;
		break;
	case Direction::WEST:
		return Direction::EAST;
		break;
	default:
		return Direction::WEST;
		break;
	}
}

/**
 * Returns a random cardinal direction.
 * 
 * @return	a random direction, one of NORTH, SOUTH, WEST and EAST
 */
Room::Direction Room::getRandomDirection()
{
	return (Room::Direction)FMath::RandRange(0, 3);
}