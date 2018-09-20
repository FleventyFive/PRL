#pragma once

#include <iostream>

#include "spark.hpp"
#include "objectFactory.hpp"

class tileDatabase {
private:
	std::map<std::string, Spark::GameObject*> tiles;
public:
	Spark::GameObject* getTile(const std::string& name);

	tileDatabase(ObjectFactory& objectFactory);
	~tileDatabase() = default;
};