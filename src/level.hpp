#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <chrono>
#include <memory>
#include <vector>
#include <queue>

#include "spark.hpp"
#include "window.hpp"
#include "events.hpp"
#include "pathfindingGrid.hpp"
#include "objectFactory.hpp"
#include "pathfinding.hpp"
#include "room.hpp"

constexpr unsigned int ROOM_WIDTH_MIN = 6, ROOM_WIDTH_MAX = 6;
constexpr unsigned int ROOM_HEIGHT_MIN = 4, ROOM_HEIGHT_MAX = 4;

constexpr unsigned int VIEWPORT_HEIGHT = 20, VIEWPORT_WIDTH = 50;

// TODO: REMOVE DIJKSTRA PATHFINDING FROM LEVEL AND PUT INTO OWN CLASS

enum Direction {
	DIR_NORTH = 0,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST
};

class Game;

class Level {
private:
	const unsigned int LEVEL_HEIGHT, LEVEL_WIDTH;

	PathfindingGrid grid;

	std::array<std::array<Spark::GameObject*, VIEWPORT_HEIGHT>, VIEWPORT_WIDTH> tiles;

	std::vector<Spark::GameObject*> enemies;
	std::vector<Spark::GameObject*> objects;

	Spark::Pool<Spark::Event> eventPool;

	std::mt19937 eng { std::random_device{}() };

	PositionEventData playerSpawnPosition;
	PositionEventData stairsUpPosition, stairsDownPosition;

	void buildRoom(Room room, ObjectFactory& objectFactory);
	
	Room createRoom();
public:
	void draw(Window& win);
	void createLevel(ObjectFactory& objectFactory, bool firstLevel);
	void update(Game* game);
	void eraseEnemy(unsigned int ID);
	void eraseObject(unsigned int ID);
	void addObject(Spark::GameObject* object, unsigned int x, unsigned int y);

	bool isTraversable(int x, int y);
	bool isDoor(int x, int y);
	bool interactWithDoor(int x, int y);
	bool inBounds(int x, int y) const noexcept { return x < static_cast<int>(LEVEL_WIDTH) && x >= 0 && y < static_cast<int>(LEVEL_HEIGHT) && y >= 0; }
	bool enemyAt(unsigned int x, unsigned int y);

	Spark::GameObject* getEnemy(unsigned int index);
	Spark::GameObject* getEnemy(unsigned int x, unsigned int y);

	std::vector<Spark::GameObject*> getObjectsOnTile(unsigned int x, unsigned int y);

	PathfindingGrid getPathfindingGrid() { return grid; }

	PositionEventData getPlayerSpawnPosition() const noexcept { return playerSpawnPosition; }
	PositionEventData getStairsUpPosition() const noexcept { return stairsUpPosition; }
	PositionEventData getStairsDownPosition() const noexcept { return stairsDownPosition; }

	Level();
	~Level() = default;
};
