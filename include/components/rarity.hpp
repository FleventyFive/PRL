#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

enum ObjectRarity {
	COMMON = 0,
	UNCOMMON,
	SUPERIOR,
	RARE,
	LEGENDARY,
	MYTHICAL,
	SPAWN_ONLY
};

class RarityComponent: public Spark::Component {
private:
	unsigned int rarity;
public:
	void fireEvent(Spark::Event* e) final;

	RarityComponent(unsigned int _rarity): Component(Spark::getComponentID<RarityComponent>()), rarity(_rarity) { }
};

RarityEventData getRarityData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);
