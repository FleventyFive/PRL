#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class EnergyComponent: public Spark::Component {
private:
	int energy, maxEnergy;
public:
	void fireEvent(Spark::Event* e) final;

	EnergyComponent(int _energy, int _maxEnergy): Component(Spark::getComponentID<EnergyComponent>()), energy(_energy), maxEnergy(_maxEnergy) { }
};

EnergyEventData getEnergyData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);

void gainEnergy(Spark::GameObject* g, int energy, Spark::Pool<Spark::Event>::ptrType &e);
void useEnergy(Spark::GameObject* g, int energy, Spark::Pool<Spark::Event>::ptrType &e);

bool canAct(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);
