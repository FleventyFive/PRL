#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

struct Position {
	int x, y;
	bool operator==(const Position& p) const {
		return x == p.x && y == p.y;
	}
};

class PositionComponent: public Spark::Component {
private:
	Position position;
public:
	void fireEvent(Spark::Event* e) final;

	PositionComponent(int _x, int _y): Component(Spark::getComponentID<PositionComponent>()), position(Position({_x, _y})) { }
};

PositionEventData getPosition(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);

void setPosition(unsigned int x, unsigned int y, Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);
void modPosition(unsigned int x, unsigned int y, Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);
