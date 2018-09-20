#pragma once

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <string>
#include <map>

#include "spark.hpp"

#include "components/events.hpp"
#include "components/health.hpp"
#include "components/render.hpp"
#include "components/energy.hpp"
#include "components/stats.hpp"
#include "components/position.hpp"
#include "components/traversable.hpp"
#include "components/koboldAI.hpp"
#include "components/inventory.hpp"
#include "components/portable.hpp"
#include "components/damage.hpp"
#include "components/weapon.hpp"
#include "components/armor.hpp"
#include "components/type.hpp"
#include "components/equipped.hpp"
#include "components/rarity.hpp"
#include "components/armorType.hpp"
#include "components/entityType.hpp"
#include "components/door.hpp"

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
	
	// std::map<std::map<std::vector<ObjectSpawnData>>>

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
