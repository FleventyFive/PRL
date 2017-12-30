#include "inventory.hpp"

void InventoryComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_ADD_TO_INVENTORY:
			inventory.push_back(std::any_cast<ObjectEventData&>(e->data).obj);
			break;
		case EVENT_GET_INVENTORY_OBJECTS:
			std::any_cast<InventoryObjectsEventData&>(e->data).inventory = &inventory;
			break;
		case EVENT_GET_INVENTORY_DATA:
			std::any_cast<InventorySizeEventData&>(e->data).size = inventory.size();
			break;
		default:
			break;
	}
}

bool isPortable(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_PORTABLE;
	e->data = PortableEventData();

	g->fireEvent(e);

	return std::any_cast<PortableEventData>(e->data).portable;
}

void addToInventory(Spark::GameObject* inv, Spark::GameObject* obj, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_ADD_TO_INVENTORY;
	e->data = ObjectEventData({obj});

	inv->fireEvent(e);
}

InventorySizeEventData getInventoryData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_INVENTORY_DATA;
	e->data = InventorySizeEventData();

	g->fireEvent(e);

	return std::any_cast<InventorySizeEventData>(e->data);
}

InventoryObjectsEventData getInventoryObjects(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_INVENTORY_OBJECTS;
	e->data = InventoryObjectsEventData();

	g->fireEvent(e);

	return std::any_cast<InventoryObjectsEventData>(e->data);
}
