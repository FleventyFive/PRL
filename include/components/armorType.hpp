#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

enum ArmorType {
	ARMOR_CHEST = 0,
	ARMOR_HEAD,
	ARMOR_LEG,
	ARMOR_FOOT
};

class ArmorTypeComponent: public Spark::Component {
private:
	ArmorType type;
public:
	void fireEvent(Spark::Event* e) final;

	explicit ArmorTypeComponent(ArmorType _type): Component(Spark::getComponentID<ArmorTypeComponent>()), type(_type) { }
};

TypeEventData getArmorTypeData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);