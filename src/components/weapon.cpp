#include "weapon.hpp"

void WeaponComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_EQUIP_WEAPON:
			weapon = std::any_cast<EquipObjectEventData&>(e->data).obj;
			std::any_cast<EquipObjectEventData&>(e->data).success = true;
			break;
		case EVENT_DEAL_MELEE_DAMAGE:
			if(weapon != nullptr)
				weapon->fireEvent(e);
			break;
		case EVENT_GET_EQUIPPED_WEAPON:
			if(weapon != nullptr)
				std::any_cast<ObjectEventData&>(e->data).obj = weapon;
			else
				std::any_cast<ObjectEventData&>(e->data).obj = nullptr;
			break;
		default:
			break;
	}
}