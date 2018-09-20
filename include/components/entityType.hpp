#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

enum EntityType {
	ENTITY_PLAYER = 0,
	ENTITY_ENEMY
};

class EntityTypeComponent: public Spark::Component {
private:
	EntityType type;
public:
	void fireEvent(Spark::Event* e) final;

	explicit EntityTypeComponent(EntityType _type): Component(Spark::getComponentID<EntityTypeComponent>()), type(_type) { }
};

TypeEventData getEntityTypeData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);