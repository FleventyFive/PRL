#include "tileDatabase.hpp"

tileDatabase::tileDatabase(ObjectFactory& objectFactory) {
	tiles["Ground"] = objectFactory.createObject("Ground");
	tiles["Wall"] = objectFactory.createObject("Wall");
}

Spark::GameObject* tileDatabase::getTile(const std::string& name) {
	return tiles[name];
}
