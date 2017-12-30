#pragma once

#include <iostream>
#include <random>
#include <array>

#include "position.hpp"

class Room {
public:
	const Position position;
	const int width, height;
	Position doorPos, doorExit;

	bool intersects(Room other);

	Room(int _x, int _y, int _width, int _height);
};