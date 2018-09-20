#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class ChestArmorComponent: public Spark::Component {
private:
	Spark::GameObject* armor{nullptr};
public:
	void fireEvent(Spark::Event* e) final;

	ChestArmorComponent(): Component(Spark::getComponentID<ChestArmorComponent>()) { }
};

class FootArmorComponent: public Spark::Component {
private:
	Spark::GameObject* armor{nullptr};
public:
	void fireEvent(Spark::Event* e) final;

	FootArmorComponent(): Component(Spark::getComponentID<FootArmorComponent>()) { }
};

class HeadArmorComponent: public Spark::Component {
private:
	Spark::GameObject* armor{nullptr};
public:
	void fireEvent(Spark::Event* e) final;

	HeadArmorComponent(): Component(Spark::getComponentID<HeadArmorComponent>()) { }
};

class LegArmorComponent: public Spark::Component {
private:
	Spark::GameObject* armor{nullptr};
public:
	void fireEvent(Spark::Event* e) final;

	LegArmorComponent(): Component(Spark::getComponentID<LegArmorComponent>()) { }
};

EquipObjectEventData equipObject(Spark::GameObject* equipper, Spark::GameObject* obj, unsigned int type, Spark::Pool<Spark::Event>::ptrType &e);

ObjectEventData getEquipped(Spark::GameObject* g, unsigned int type, Spark::Pool<Spark::Event>::ptrType &e);

int getDefense(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);