#include "equipped.hpp"

void EquippedComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_IS_EQUIPPED:
			std::any_cast<EquippedEventData&>(e->data).equipped = equipped;
			break;
		case EVENT_SET_IS_EQUIPPED:
			equipped = std::any_cast<EquippedEventData&>(e->data).equipped;
			break;
		default:
			break;
	}
}

bool isEquipped(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_IS_EQUIPPED;
	e->data = EquippedEventData({false});

	g->fireEvent(e);

	return std::any_cast<EquippedEventData>(e->data).equipped;
}

void setIsEquipped(Spark::GameObject* g, bool isEquipped, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_SET_IS_EQUIPPED;
	e->data = EquippedEventData({isEquipped});

	g->fireEvent(e);
}