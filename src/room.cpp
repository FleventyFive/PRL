#include "room.hpp"

Room::Room(int _x, int _y, int _width, int _height): position(Position({_x, _y})), width(_width), height(_height) { }

bool Room::intersects(Room other) {
	const auto& [x, y] = position;
	const auto& [otherX, otherY] = other.position;

	return (y - 2 < otherY + other.height + 2 && y + height + 2 > otherY - 2 && 
			x - 2 < otherX + other.width + 2 && x + width + 2 > otherX - 2);
}
