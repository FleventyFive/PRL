#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class EquippedComponent: public Spark::Component {
private:
	bool equipped{false};
public:
	void fireEvent(Spark::Event* e) final;

	EquippedComponent(): Component(Spark::getComponentID<EquippedComponent>()) { }
};

bool isEquipped(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);

void setIsEquipped(Spark::GameObject* g, bool isEquipped, Spark::Pool<Spark::Event>::ptrType &e);