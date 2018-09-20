#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "spark.hpp"
#include "dialogueWindow.hpp"
#include "objectFactory.hpp"
#include "window.hpp"
#include "level.hpp"

#include "components/events.hpp"
#include "components/health.hpp"
#include "components/render.hpp"
#include "components/energy.hpp"
#include "components/stats.hpp"
#include "components/position.hpp"
#include "components/koboldAI.hpp"
#include "components/traversable.hpp"
#include "components/inventory.hpp"
#include "components/portable.hpp"
#include "components/damage.hpp"
#include "components/weapon.hpp"
#include "components/armor.hpp"
#include "components/type.hpp"
#include "components/equipped.hpp"

// auto begin = std::chrono::high_resolution_clock::now();
// auto end = std::chrono::high_resolution_clock::now();
// times.push_back(std::chrono::duration_cast<std::chrono::miliseconds>(end-begin).count());

#define NO_ENEMY -1

enum InspectInventoryChoices {
	CHOICE_EQUIP = 0
};

class Game {
private:
	Spark::World world;
	Spark::Pool<Spark::Event> eventPool;

	Spark::GameObject* player;

	Window viewport;
	DialogueWindow dialogue;
	Window hud;

	Level level;

	ObjectFactory objectFactory;

	unsigned int updateCount;
	unsigned int levelHeight;

	bool quit = false;
	bool playerWaiting = false;
	int input;

	std::vector<double> times;

	void playerInspectInventory();
	void playerPickAction(std::vector<Spark::GameObject*>& inventory, const int itemPos);
	void playerEquip(std::vector<Spark::GameObject*>& inventory, const int itemPos);
	void playerDrop(std::vector<Spark::GameObject*>& inventory, int itemPos);
	void playerCloseDoor();
	void playerPickUpItem();
	void moveLevelUp();
	void moveLevelDown();

	bool playerCanAct();

public:
	void draw();
	void drawHud();
	void drawViewport();
	void update();
	void processInput();
	void movePlayer(const char& direction);
	void endGame();

	Spark::GameObject* getPlayer() const noexcept { return player; }

	PositionEventData getPlayerPos();

	constexpr std::vector<double>& getTimes() noexcept { return times; }

	constexpr Level& getLevel() noexcept { return level; }

	constexpr unsigned int getRequiredScreenHeight() const noexcept { return VIEWPORT_HEIGHT; }
	constexpr unsigned int getRequiredScreenWidth() const noexcept { return VIEWPORT_WIDTH + 57; }

	constexpr bool getQuit() const noexcept { return quit; }

	Game();
	~Game() = default;
};
