#include "type.hpp"

void TypeComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_TYPE_DATA:
		{
			TypeEventData& te = std::any_cast<TypeEventData&>(e->data);
			te.type = type;
		}
			break;
		default:
			break;
	}
}

TypeEventData getTypeData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_TYPE_DATA;
	e->data = TypeEventData();

	g->fireEvent(e);

	return std::any_cast<TypeEventData>(e->data);
}