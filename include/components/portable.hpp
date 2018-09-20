#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class PortableComponent: public Spark::Component {
public:
	void fireEvent(Spark::Event* e) final;

	PortableComponent(): Component(Spark::getComponentID<PortableComponent>()) { }
};