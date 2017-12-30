#include "damage.hpp"

void DamageComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_DEAL_MELEE_DAMAGE:
			std::any_cast<DamageEventData&>(e->data).damage = die.roll();
			break;
		case EVENT_GET_DAMAGE_TYPE:
			std::any_cast<TypeEventData&>(e->data).type = type;
			break;
		default:
			break;
	}
}