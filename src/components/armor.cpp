#include "armor.hpp"

void ChestArmorComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_EQUIP_CHEST_ARMOR:
			armor = std::any_cast<EquipObjectEventData&>(e->data).obj;
			std::any_cast<EquipObjectEventData&>(e->data).success = true;
			break;
		case EVENT_GET_STATS_DATA:
			if(armor != nullptr)
				armor->fireEvent(e);
			break;
		case EVENT_GET_EQUIPPED_CHEST_ARMOR:
			if(armor != nullptr)
				std::any_cast<ObjectEventData&>(e->data).obj = armor;
			else
				std::any_cast<ObjectEventData&>(e->data).obj = nullptr;
			break;
		default:
			break;
	}
}

void FootArmorComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_EQUIP_FOOT_ARMOR:
			armor = std::any_cast<EquipObjectEventData&>(e->data).obj;
			std::any_cast<EquipObjectEventData&>(e->data).success = true;
			break;
		case EVENT_GET_STATS_DATA:
			if(armor != nullptr)
				armor->fireEvent(e);
			break;
		case EVENT_GET_EQUIPPED_FOOT_ARMOR:
			if(armor != nullptr)
				std::any_cast<ObjectEventData&>(e->data).obj = armor;
			else
				std::any_cast<ObjectEventData&>(e->data).obj = nullptr;
			break;
		default:
			break;
	}
}

void HeadArmorComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_EQUIP_HEAD_ARMOR:
			armor = std::any_cast<EquipObjectEventData&>(e->data).obj;
			std::any_cast<EquipObjectEventData&>(e->data).success = true;
			break;
		case EVENT_GET_STATS_DATA:
			if(armor != nullptr)
				armor->fireEvent(e);
			break;
		case EVENT_GET_EQUIPPED_HEAD_ARMOR:
			if(armor != nullptr)
				std::any_cast<ObjectEventData&>(e->data).obj = armor;
			else
				std::any_cast<ObjectEventData&>(e->data).obj = nullptr;
			break;
		default:
			break;
	}
}

void LegArmorComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_EQUIP_LEG_ARMOR:
			armor = std::any_cast<EquipObjectEventData&>(e->data).obj;
			std::any_cast<EquipObjectEventData&>(e->data).success = true;
			break;
		case EVENT_GET_STATS_DATA:
			if(armor != nullptr)
				armor->fireEvent(e);
			break;
		case EVENT_GET_EQUIPPED_LEG_ARMOR:
			if(armor != nullptr)
				std::any_cast<ObjectEventData&>(e->data).obj = armor;
			else
				std::any_cast<ObjectEventData&>(e->data).obj = nullptr;
			break;
		default:
			break;
	}
}

EquipObjectEventData equipObject(Spark::GameObject* equipper, Spark::GameObject* obj, unsigned int type, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = type;
	e->data = EquipObjectEventData({obj, false});

	equipper->fireEvent(e);

	return std::any_cast<EquipObjectEventData>(e->data);
}

ObjectEventData getEquipped(Spark::GameObject* g, unsigned int type, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = type;
	e->data = ObjectEventData({nullptr});

	g->fireEvent(e);

	return std::any_cast<ObjectEventData>(e->data);
}

int getDefense(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_DEFENSE;
	e->data = GetStatsEventData();

	g->fireEvent(e);

	return std::any_cast<GetStatsEventData>(e->data).defense;
}
