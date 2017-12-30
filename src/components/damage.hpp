#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"
#include "die.hpp"

enum DamageType {
	DAMAGE_BLUNT = 0,
	DAMAGE_SLASH
};

class DamageComponent: public Spark::Component {
private:
	Die die;
	
	DamageType type;
public:
	void fireEvent(Spark::Event* e) final;

	DamageComponent(Die _die, DamageType _type): Component(Spark::getComponentID<DamageComponent>()), die(_die), type(_type) { }
};