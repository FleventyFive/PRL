#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"

class RenderComponent: public Spark::Component {
private:
	std::string name, description, symbol;
	short color;
public:
	void fireEvent(Spark::Event* e) final;
	
	RenderComponent(const std::string& _name, const std::string& _description, const std::string& _symbol, short _color): Component(Spark::getComponentID<RenderComponent>()), 
		name(std::move(_name)), description(std::move(_description)), symbol(_symbol), color(_color) { }
};

GetRenderEventData getRenderData(Spark::GameObject* g, Spark::Pool<Spark::Event>::ptrType &e);