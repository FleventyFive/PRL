#include "position.hpp"

void PositionComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_POSITION_DATA:
		{
			PositionEventData& pe = std::any_cast<PositionEventData&>(e->data);
			pe.x = position.x;
			pe.y = position.y;
		}
			break;
		case EVENT_SET_POSITION_DATA:
		{
			PositionEventData& pe = std::any_cast<PositionEventData&>(e->data);
			position.x = pe.x;
			position.y = pe.y;
		}
			break;
		case EVENT_MOD_POSITION_DATA:
		{
			PositionEventData& pe = std::any_cast<PositionEventData&>(e->data);
			position.x += pe.x;
			position.y += pe.y;
		}
			break;
		default:
			break;
	}
}

PositionEventData getPosition(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_POSITION_DATA;
	e->data = PositionEventData();

	g->fireEvent(e);

	return std::any_cast<PositionEventData>(e->data);
}

void setPosition(unsigned int x, unsigned int y, Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_SET_POSITION_DATA;
	e->data = PositionEventData({x, y});

	g->fireEvent(e);
}

void modPosition(unsigned int x, unsigned int y, Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_MOD_POSITION_DATA;
	e->data = PositionEventData({x, y});

	g->fireEvent(e);
}
