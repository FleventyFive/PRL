#pragma once

#include <iostream>
#include <ncurses.h>

#include "spark.hpp"
#include "events.hpp"

class DoorComponent: public Spark::Component {
private:
	bool open;

public:
	void fireEvent(Spark::Event* e) final;

	DoorComponent(bool _open = false): Component(Spark::getComponentID<DoorComponent>()), open(_open) { }
};
