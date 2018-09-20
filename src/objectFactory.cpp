#include "objectFactory.hpp"

ObjectFactory::ObjectFactory(Spark::World& _world, const std::string& blueprintsPath): world(_world) {
	world.loadBlueprints(blueprintsPath);

	std::vector<Spark::Blueprint> blueprints = world.getBlueprints();

	for(auto& blueprint : blueprints) {
		ObjectSpawnData osd;
		
		for(auto& component : blueprint.components) {
			osd.name = blueprint.name;
			if(component.name == "TypeComponent") {
				osd.type = getType(component.arguments["type"]);
			} else if(component.name == "RarityComponent") {
				osd.rarity = getRarity(component.arguments["rarity"]);
			}
		}

		// TRANSFER TO A MAP OF A MAP OF VECTORS
		if(osd.type == OBJ_WEAPON) {
			weaponSpawnDataVec.push_back(osd);
		} else if (osd.type == OBJ_ARMOR) {
			armorSpawnDataVec.push_back(osd);
		} else if(osd.type == OBJ_ENTITY) {
			entitySpawnDataVec.push_back(osd);
		}
	}
}

Spark::GameObject* ObjectFactory::createObject(const std::string& name) {
	Spark::GameObject* obj = world.createGameObject();

	Spark::Blueprint blueprint = world.getBlueprintByName(name);
	
	for(auto& component : blueprint.components) {
		if(component.name == "RenderComponent") {
			obj->addComponent<RenderComponent>(
				component.arguments["name"],
				component.arguments["description"],
				component.arguments["symbol"],
				getColor(component.arguments["color"]));
		} else if(component.name == "DamageComponent") {
			obj->addComponent<DamageComponent>(
				Die(static_cast<int>(std::stol(component.arguments["rolls"])),
					static_cast<int>(std::stol(component.arguments["sides"]))),
				getDamageType(component.arguments["type"]));
		} else if(component.name == "StatsComponent") {
			obj->addComponent<StatsComponent>(
				static_cast<int>(std::stol(component.arguments["speed"])),
				static_cast<int>(std::stol(component.arguments["strength"])),
				static_cast<int>(std::stol(component.arguments["vitality"])),
				static_cast<int>(std::stol(component.arguments["defense"])));
		} else if(component.name == "InventoryComponent") {
			obj->addComponent<InventoryComponent>();
		} else if(component.name == "WeaponComponent") {
			obj->addComponent<WeaponComponent>();
		} else if(component.name == "ChestArmorComponent") {
			obj->addComponent<ChestArmorComponent>();
		} else if(component.name == "LegArmorComponent") {
			obj->addComponent<LegArmorComponent>();
		} else if(component.name == "HeadArmorComponent") {
			obj->addComponent<HeadArmorComponent>();
		} else if(component.name == "FootArmorComponent") {
			obj->addComponent<FootArmorComponent>();
		} else if(component.name == "HealthComponent") {
			obj->addComponent<HealthComponent>(
				static_cast<float>(std::stol(component.arguments["health"])),
				static_cast<float>(std::stol(component.arguments["maxHealth"])));
		} else if(component.name == "PositionComponent") {
			obj->addComponent<PositionComponent>(0, 0);
		} else if(component.name == "EnergyComponent") {
			obj->addComponent<EnergyComponent>(
				static_cast<int>(std::stol(component.arguments["energy"])),
				static_cast<int>(std::stol(component.arguments["maxEnergy"])));
		} else if(component.name == "KoboldAIComponent") {
			obj->addComponent<KoboldAIComponent>();
		} else if(component.name == "TypeComponent") {
			obj->addComponent<TypeComponent>(
				getType(component.arguments["type"]));
		} else if(component.name == "ArmorTypeComponent") {
			obj->addComponent<ArmorTypeComponent>(
				getArmorType(component.arguments["type"]));
		} else if(component.name == "EntityTypeComponent") {
			obj->addComponent<EntityTypeComponent>(
				getEntityType(component.arguments["type"]));
		} else if(component.name == "RarityComponent") {
			obj->addComponent<RarityComponent>(getRarity(component.arguments["rarity"]));
		} else if(component.name == "PortableComponent") {
			obj->addComponent<PortableComponent>();
		} else if(component.name == "EquippedComponent") {
			obj->addComponent<EquippedComponent>();
		} else if(component.name == "TraversableComponent") {
			std::string val = component.arguments["traversable"];
			std::transform(val.begin(), val.end(), val.begin(), ::tolower);
			std::istringstream iss(val);
			bool b;
			iss >> std::boolalpha >> b;
			obj->addComponent<TraversableComponent>(b);
		} else if(component.name == "DoorComponent") {
			std::string val = component.arguments["open"];
			std::transform(val.begin(), val.end(), val.begin(), ::tolower);
			std::istringstream iss(val);
			bool b;
			iss >> std::boolalpha >> b;
			obj->addComponent<DoorComponent>(b);
		}
	}

	assert(obj != nullptr);

	return obj;
}

Spark::GameObject* ObjectFactory::spawnObject(unsigned int rarity, unsigned int type) {
	std::vector<std::string> candidates;
	if(type == OBJ_WEAPON) {
		for(const auto& data : weaponSpawnDataVec)
			if(data.rarity == rarity)
				candidates.push_back(data.name);
	} else if(type == OBJ_ARMOR) {
		for(const auto& data : armorSpawnDataVec)
			if(data.rarity == rarity)
				candidates.push_back(data.name);
	} else if(type == OBJ_ENTITY) {
		for(const auto& data : entitySpawnDataVec)
			if(data.rarity == rarity)
				candidates.push_back(data.name);
	}

	const int index = std::uniform_int_distribution<unsigned int>{0, (static_cast<unsigned int>(candidates.size()) - 1)}(eng);

	return createObject(candidates[index]);
}

short ObjectFactory::getColor(const std::string& name) {
	short color = std::numeric_limits<short>::max();

	if(name == "COLOR_BLACK")
		color = COLOR_BLACK;
	else if(name == "COLOR_RED")
		color = COLOR_RED;
	else if(name == "COLOR_GREEN")
		color = COLOR_GREEN;
	else if(name == "COLOR_YELLOW")
		color = COLOR_YELLOW;
	else if(name == "COLOR_BLUE")
		color = COLOR_BLUE;
	else if(name == "COLOR_MAGENTA")
		color = COLOR_MAGENTA;
	else if(name == "COLOR_CYAN")
		color = COLOR_CYAN;
	else if(name == "COLOR_WHITE")
		color = COLOR_WHITE;

	assert(color != std::numeric_limits<short>::max());

	return color;
}

unsigned int ObjectFactory::getType(const std::string& type) {
	unsigned int _type = std::numeric_limits<unsigned int>::max();

	if(type == "OBJ_WEAPON")
		_type = OBJ_WEAPON;
	else if(type == "OBJ_ARMOR")
		_type = OBJ_ARMOR;
	else if(type == "OBJ_ENTITY")
		_type = OBJ_ENTITY;
	else if(type == "OBJ_TERRAIN")
		_type = OBJ_TERRAIN;

	assert(_type != std::numeric_limits<unsigned int>::max());

	return _type;
}

unsigned int ObjectFactory::getRarity(const std::string& rarity) {
	unsigned int _rarity = std::numeric_limits<unsigned int>::max();

	if(rarity == "COMMON")
		_rarity = COMMON;
	else if(rarity == "UNCOMMON")
		_rarity = UNCOMMON;
	else if(rarity == "SUPERIOR")
		_rarity = SUPERIOR;
	else if(rarity == "RARE")
		_rarity = RARE;
	else if(rarity == "LEGENDARY")
		_rarity = LEGENDARY;
	else if(rarity == "MYTHICAL")
		_rarity = MYTHICAL;
	else if(rarity == "SPAWN_ONLY")
		_rarity = SPAWN_ONLY;

	assert(_rarity != std::numeric_limits<unsigned int>::max());

	return _rarity;
}

DamageType ObjectFactory::getDamageType(const std::string& dt) {
	int _damageType = -1;

	if(dt == "DAMAGE_BLUNT")
		_damageType = DAMAGE_BLUNT;
	else if(dt == "DAMAGE_SLASH")
		_damageType = DAMAGE_SLASH;

	assert(_damageType != -1);

	return static_cast<DamageType>(_damageType);
}

EntityType ObjectFactory::getEntityType(const std::string& et) {
	int _entityType = -1;

	if(et == "ENTITY_PLAYER")
		_entityType = ENTITY_PLAYER;
	else if(et == "ENTITY_ENEMY")
		_entityType = ENTITY_ENEMY;

	assert(_entityType != -1);

	return static_cast<EntityType>(_entityType);
}

ArmorType ObjectFactory::getArmorType(const std::string& at) {
	int _armorType = -1;

	if(at == "ARMOR_CHEST")
		_armorType = ARMOR_CHEST;
	else if(at == "ARMOR_HEAD")
		_armorType = ARMOR_HEAD;
	else if(at == "ARMOR_LEG")
		_armorType = ARMOR_LEG;
	else if(at == "ARMOR_FOOT")
		_armorType = ARMOR_FOOT;

	assert(_armorType != -1);

	return static_cast<ArmorType>(_armorType);
}
