#include "entityType.hpp"

void EntityTypeComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_ENTITY_TYPE:
			std::any_cast<TypeEventData&>(e->data).type = type;
			break;
		default:
			break;
	}
}

TypeEventData getEntityTypeData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_ENTITY_TYPE;
	e->data = TypeEventData();

	g->fireEvent(e);

	return std::any_cast<TypeEventData>(e->data);
}