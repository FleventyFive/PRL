#include "portable.hpp"

void PortableComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_PORTABLE:
			std::any_cast<PortableEventData&>(e->data).portable = true;
			break;
		default:
			break;
	}
}