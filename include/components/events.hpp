#pragma once

#include <iostream>
#include <string>

#include "spark.hpp"

class Game;

// TODO: use preprocessor to generate event names?
enum EventType {
	EVENT_UPDATE = 0,
	// Health events
	EVENT_HEAL,
	EVENT_IS_ALIVE,
	EVENT_GET_HEALTH_DATA,
	EVENT_TAKE_DAMAGE,
	// Render events
	EVENT_GET_RENDER_DATA,
	EVENT_SET_RENDER_DATA,
	// Energy events
	EVENT_GET_ENERGY_DATA,
	EVENT_GAIN_ENERGY,
	EVENT_USE_ENERGY,
	EVENT_CAN_ACT,
	// Stats events
	EVENT_GET_STATS_DATA,
	EVENT_GET_CORE_STATS_DATA,
	EVENT_GET_DEFENSE,
	// Position events
	EVENT_GET_POSITION_DATA,
	EVENT_SET_POSITION_DATA,
	EVENT_MOD_POSITION_DATA,
	// Traversable event
	EVENT_GET_TRAVERSABLE,
	EVENT_SET_TRAVERSABLE,
	// Inventory events
	EVENT_GET_PORTABLE,
	EVENT_ADD_TO_INVENTORY,
	EVENT_GET_INVENTORY_DATA,
	EVENT_GET_INVENTORY_OBJECTS,
	// Door events
	EVENT_DOOR_INTERACT,
	EVENT_IS_DOOR,
	// Type events
	EVENT_GET_TYPE_DATA,
	EVENT_GET_ENTITY_TYPE,
	EVENT_GET_RARITY_DATA,
	EVENT_GET_ARMOR_TYPE,
	EVENT_GET_DAMAGE_TYPE,
	// Equipment events
	EVENT_EQUIP_WEAPON,
	EVENT_EQUIP_CHEST_ARMOR,
	EVENT_EQUIP_HEAD_ARMOR,
	EVENT_EQUIP_LEG_ARMOR,
	EVENT_EQUIP_FOOT_ARMOR,
	EVENT_GET_EQUIPPED_WEAPON,
	EVENT_GET_EQUIPPED_CHEST_ARMOR,
	EVENT_GET_EQUIPPED_HEAD_ARMOR,
	EVENT_GET_EQUIPPED_LEG_ARMOR,
	EVENT_GET_EQUIPPED_FOOT_ARMOR,
	EVENT_GET_IS_EQUIPPED,
	EVENT_SET_IS_EQUIPPED,
	// Combat events
	EVENT_DEAL_MELEE_DAMAGE
};

// Event payloads
struct GetRenderEventData { std::string name, description; std::string symbol; short color; };

struct EnergyEventData { int energy, maxEnergy; };

struct HealthEventData { float health, maxHealth; };

struct GetStatsEventData { double speed = 0, strength = 0, vitality = 0, defense = 0; };

struct DoorInteractEventData { bool open; };

struct PositionEventData { unsigned int x, y; };

struct TraversableEventData { bool traversable; };

struct CanActEventData { bool canAct; };

struct UpdateEventData { Game* game; };

struct PortableEventData { bool portable; };

struct ObjectEventData { Spark::GameObject* obj; };

struct EquipObjectEventData { Spark::GameObject* obj; bool success; };

struct InventorySizeEventData { unsigned int size; };

struct InventoryObjectsEventData { std::vector<Spark::GameObject*>* inventory; };

struct DamageEventData { unsigned int damage; };

struct IsAliveEventData { bool isAlive; };

struct IsDoorEventData { bool isDoor; };

struct TypeEventData { unsigned int type; };

struct WeaponTypeEventData { unsigned int type; };

struct RarityEventData { unsigned int rarity; };

struct EquippedEventData { bool equipped; };

