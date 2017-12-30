#include "stats.hpp"

#include "game.hpp"

void StatsComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_GET_STATS_DATA:
		{
			GetStatsEventData& sde = std::any_cast<GetStatsEventData&>(e->data);
			sde.speed += speed;
			sde.strength += strength;
			sde.vitality += vitality;
			sde.defense += defense;
		}
			break;
		case EVENT_GET_CORE_STATS_DATA:
		{
			GetStatsEventData& sde = std::any_cast<GetStatsEventData&>(e->data);
			sde.speed = speed;
			sde.strength = strength;
			sde.vitality = vitality;
			sde.defense = defense;
		}
			break;
		case EVENT_UPDATE:
		{
			Spark::Event updateEvent;
			updateEvent.type = EVENT_GAIN_ENERGY;
			updateEvent.data = EnergyEventData({ static_cast<int>((50 + (speed * 10))), 0 });
			getOwner()->fireEvent(&updateEvent);
			updateEvent.type = EVENT_HEAL;
			updateEvent.data = HealthEventData({ static_cast<float>((static_cast<float>(vitality) / 5.0)), 0 });
			getOwner()->fireEvent(&updateEvent);
		}
			break;
		case EVENT_DEAL_MELEE_DAMAGE:
			std::any_cast<DamageEventData&>(e->data).damage = std::max(((static_cast<int>(strength) / 5) * 2), 1);
			break;
		case EVENT_GET_DEFENSE:
			std::any_cast<GetStatsEventData&>(e->data).defense += defense;
			break;
		default:
			break;
	}
}

GetStatsEventData getStatsData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_STATS_DATA;
	e->data = GetStatsEventData();

	g->fireEvent(e);

	return std::any_cast<GetStatsEventData>(e->data);;
}

GetStatsEventData getCoreStatsData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_CORE_STATS_DATA;
	e->data = GetStatsEventData();

	g->fireEvent(e);

	return std::any_cast<GetStatsEventData>(e->data);;
}
