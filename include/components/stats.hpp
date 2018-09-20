#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class Game;

class StatsComponent: public Spark::Component {
private:
	double speed, strength, vitality, defense;
public:
	void fireEvent(Spark::Event* e) final;
	
	StatsComponent(double _speed, double _strength, double _vitality, double _defense): Component(Spark::getComponentID<StatsComponent>()), 
		speed(_speed), strength(_strength), vitality(_vitality), defense(_defense) { }
};

GetStatsEventData getStatsData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);
GetStatsEventData getCoreStatsData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);