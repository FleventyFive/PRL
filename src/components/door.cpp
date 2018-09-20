#include "door.hpp"

void DoorComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_IS_DOOR:
			std::any_cast<IsDoorEventData&>(e->data).isDoor = true;
			break;
		case EVENT_DOOR_INTERACT:
		{
			Spark::Event setDataEvent;
			if(open) {
				setDataEvent.type = EVENT_SET_RENDER_DATA;
				setDataEvent.data = GetRenderEventData({"Closed Door", "A sturdy steel door with an input panel beside it", "+", COLOR_BLACK});
				getOwner()->fireEvent(&setDataEvent);
				setDataEvent.type = EVENT_SET_TRAVERSABLE;
				setDataEvent.data = TraversableEventData({false});
				getOwner()->fireEvent(&setDataEvent);
				open = false;
			} else {
				setDataEvent.type = EVENT_SET_RENDER_DATA;
				setDataEvent.data = GetRenderEventData({"Open Door", "An open door", "/", COLOR_BLACK});
				getOwner()->fireEvent(&setDataEvent);
				setDataEvent.type = EVENT_SET_TRAVERSABLE;
				setDataEvent.data = TraversableEventData({true});
				getOwner()->fireEvent(&setDataEvent);
				open = true;
			}

			std::any_cast<DoorInteractEventData&>(e->data).open = open;
		}
			break;
		default:
			break;
	}
}
