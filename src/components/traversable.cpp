#include "traversable.hpp"

void TraversableComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_TRAVERSABLE:
			std::any_cast<TraversableEventData&>(e->data).traversable = traversable;
			break;
		case EVENT_SET_TRAVERSABLE:
			traversable = std::any_cast<TraversableEventData>(e->data).traversable;
			break;
		default:
			break;
	}
}

bool isTraversable(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_TRAVERSABLE;
	e->data = TraversableEventData({false});

	g->fireEvent(e);

	return std::any_cast<TraversableEventData>(e->data).traversable;
}

void setTraversable(bool traversable, Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_SET_TRAVERSABLE;
	e->data = TraversableEventData({traversable});

	g->fireEvent(e);
}