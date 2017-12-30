#include "health.hpp"

void HealthComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_TAKE_DAMAGE:
			health -= std::any_cast<DamageEventData&>(e->data).damage;
			break;
		case EVENT_IS_ALIVE:
			std::any_cast<IsAliveEventData&>(e->data).isAlive = !(health <= 0);
			break;
		case EVENT_GET_HEALTH_DATA:
		{
			HealthEventData& he = std::any_cast<HealthEventData&>(e->data);
			he.health = health;
			he.maxHealth = maxHealth;
		}
			break;
		default:
			break;
	}
}

HealthEventData getHealthData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_GET_HEALTH_DATA;
	e->data = HealthEventData();

	g->fireEvent(e);

	return std::any_cast<HealthEventData>(e->data);
}

void heal(float healAmount, Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_HEAL;
	e->data = HealthEventData({healAmount, 0});

	g->fireEvent(e);
}

void takeDamage(float damage, Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_TAKE_DAMAGE;
	e->data = HealthEventData({damage, 0});

	g->fireEvent(e);
}

bool isAlive(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e) {
	e->type = EVENT_IS_ALIVE;
	e->data = IsAliveEventData({false});

	g->fireEvent(e);

	return std::any_cast<IsAliveEventData>(e->data).isAlive;
}