#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

enum GameObjectType {
	OBJ_NOT_FOUND = 0,
	OBJ_WEAPON,
	OBJ_ARMOR,
	OBJ_ENTITY,
	OBJ_TERRAIN
};

class TypeComponent: public Spark::Component {
private:
	unsigned int type = 0;
public:
	void fireEvent(Spark::Event* e) final;
	
	TypeComponent(unsigned int _type): Component(Spark::getComponentID<TypeComponent>()), type(_type) { }
};

TypeEventData getTypeData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);