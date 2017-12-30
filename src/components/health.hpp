#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class HealthComponent: public Spark::Component {
private:
	float health, maxHealth;
public:
		void fireEvent(Spark::Event* e) final;

	HealthComponent(float _health, float _maxHealth): Component(Spark::getComponentID<HealthComponent>()), health(_health), maxHealth(_maxHealth) { }
};

HealthEventData getHealthData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);

void heal(float healAmount, Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);
void takeDamage(float damage, Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);

bool isAlive(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);
