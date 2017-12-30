#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class WeaponComponent: public Spark::Component {
private:
	Spark::GameObject* weapon;
public:
	void fireEvent(Spark::Event* e) final;

	WeaponComponent(): Component(Spark::getComponentID<WeaponComponent>()), weapon(nullptr) { }
};