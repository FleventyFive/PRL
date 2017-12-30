#include "rarity.hpp"

void RarityComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_RARITY_DATA:
			std::any_cast<RarityEventData&>(e->data).rarity = rarity;
			break;
		default:
			break;
	}
}

RarityEventData getRarityData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_RARITY_DATA;
	e->data = RarityEventData();

	g->fireEvent(e);

	return std::any_cast<RarityEventData>(e->data);
}