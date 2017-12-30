#include "armorType.hpp"

void ArmorTypeComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_ARMOR_TYPE:
			std::any_cast<TypeEventData&>(e->data).type = type;
			break;
		default:
			break;
	}
}

TypeEventData getArmorTypeData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_ARMOR_TYPE;
	e->data = TypeEventData();

	g->fireEvent(e);

	return std::any_cast<TypeEventData>(e->data);
}
