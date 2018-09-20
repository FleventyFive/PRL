#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class TraversableComponent: public Spark::Component {
private:
	bool traversable;
public:
	void fireEvent(Spark::Event* e) final;

	explicit TraversableComponent(bool _traversable): Component(Spark::getComponentID<TraversableComponent>()), traversable(_traversable) { }
};

bool isTraversable(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);

void setTraversable(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);