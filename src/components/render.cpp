#include "render.hpp"

void RenderComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_RENDER_DATA:
		{
			GetRenderEventData& rde = std::any_cast<GetRenderEventData&>(e->data);
			rde.symbol = symbol;
			rde.name = name;
			rde.description = description;
			rde.color = color;
		}
		case EVENT_SET_RENDER_DATA:
		{
			GetRenderEventData& rde = std::any_cast<GetRenderEventData&>(e->data);
			symbol = rde.symbol;
			name = rde.name;
			description = rde.description;
			color = rde.color;
		}
			break;
		default:
			break;
	}
}

GetRenderEventData getRenderData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_RENDER_DATA;
	e->data = GetRenderEventData();

	g->fireEvent(e);

	return std::any_cast<GetRenderEventData>(e->data);
}