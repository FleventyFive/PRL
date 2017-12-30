#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class InventoryComponent: public Spark::Component {
private:
	std::vector<Spark::GameObject*> inventory;
public:
	void fireEvent(Spark::Event* e) final;

	InventoryComponent(): Component(Spark::getComponentID<InventoryComponent>()) { }
};

bool isPortable(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);

void addToInventory(Spark::GameObject* inv, Spark::GameObject* obj, Spark::Pool<Spark::Event>::ptrType &e);

InventorySizeEventData getInventoryData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);

InventoryObjectsEventData getInventoryObjects(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);
