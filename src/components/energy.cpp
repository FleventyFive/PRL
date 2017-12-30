#include "energy.hpp"

void EnergyComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_ENERGY_DATA:
		{
			EnergyEventData& ee = std::any_cast<EnergyEventData&>(e->data);
			ee.energy = energy;
			ee.maxEnergy = maxEnergy;
		}
			break;
		case EVENT_USE_ENERGY:
			energy -= std::any_cast<EnergyEventData&>(e->data).energy;
			break;
		case EVENT_CAN_ACT:
			std::any_cast<CanActEventData&>(e->data).canAct = (energy > 0);
			break;
		case EVENT_GAIN_ENERGY:
		{
			EnergyEventData& ee = std::any_cast<EnergyEventData&>(e->data);
			((energy + ee.energy) < maxEnergy) ? (energy += ee.energy) : energy = maxEnergy;
		}
			break;
		default:
			break;
	}
}

EnergyEventData getEnergyData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_ENERGY_DATA;
	e->data = EnergyEventData();

	g->fireEvent(e);

	return std::any_cast<EnergyEventData>(e->data);
}

void gainEnergy(Spark::GameObject* g, int energy, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GAIN_ENERGY;
	e->data = EnergyEventData({energy, 0});

	g->fireEvent(e);
}

void useEnergy(Spark::GameObject* g, int energy, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_USE_ENERGY;
	e->data = EnergyEventData({energy, 0});

	g->fireEvent(e);
}

bool canAct(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_CAN_ACT;
	e->data = CanActEventData({false});

	g->fireEvent(e);

	return std::any_cast<CanActEventData>(e->data).canAct;
}
