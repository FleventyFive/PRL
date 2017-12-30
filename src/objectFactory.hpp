#pragma once

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <string>
#include <map>

#include <ncurses.h>

#include "spark.hpp"
#include "events.hpp"

#include "health.hpp"
#include "render.hpp"
#include "energy.hpp"
#include "stats.hpp"
#include "position.hpp"
#include "traversable.hpp"
#include "koboldAI.hpp"
#include "inventory.hpp"
#include "portable.hpp"
#include "damage.hpp"
#include "weapon.hpp"
#include "armor.hpp"
#include "type.hpp"
#include "equipped.hpp"
#include "rarity.hpp"
#include "armorType.hpp"
#include "entityType.hpp"
#include "door.hpp"

class ObjectFactory {
private:
	Spark::World& world;

	struct ObjectSpawnData {
		std::string name;
		unsigned int rarity, type, subtype;
	};

	std::vector<ObjectSpawnData> entitySpawnDataVec;
	std::vector<ObjectSpawnData> weaponSpawnDataVec;
	std::vector<ObjectSpawnData> armorSpawnDataVec;

	std::mt19937 eng{std::random_device{}()};

	short getColor(const std::string& name);
	
	unsigned int getType(const std::string& type);
	unsigned int getRarity(const std::string& rarity);

	DamageType getDamageType(const std::string& dt);
	EntityType getEntityType(const std::string& et);
	ArmorType getArmorType(const std::string& at);
public:

	Spark::GameObject* createObject(const std::string& name);
	Spark::GameObject* spawnObject(unsigned int rarity, unsigned int type);

	ObjectFactory(Spark::World& _world, const std::string& blueprintsPath);
};
