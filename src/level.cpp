#include "level.hpp"
#include "game.hpp"

Level::Level(): LEVEL_HEIGHT(VIEWPORT_HEIGHT), LEVEL_WIDTH(VIEWPORT_WIDTH), grid(VIEWPORT_HEIGHT, VIEWPORT_WIDTH), eventPool() {
	for(int i = 0; i < 100; ++i)
		eventPool.add(std::unique_ptr<Spark::Event>{ std::make_unique<Spark::Event>() });
}

void Level::draw(Window& win) {
	auto renderDataEvent = eventPool.getResource();
	renderDataEvent->type = EVENT_GET_RENDER_DATA;
	renderDataEvent->data = GetRenderEventData();

	auto positionDataEvent = eventPool.getResource();
	positionDataEvent->type = EVENT_GET_POSITION_DATA;
	positionDataEvent->data = PositionEventData();

	const GetRenderEventData& renderData = std::any_cast<const GetRenderEventData&>(renderDataEvent->data);
	const PositionEventData& positionData = std::any_cast<const PositionEventData&>(positionDataEvent->data);

	for(unsigned int i = 0; i < LEVEL_WIDTH; ++i) {
		for(unsigned int j = 0; j < LEVEL_HEIGHT; ++j) {
			tiles[i][j]->fireEvent(renderDataEvent);
			wattron(win.get(), COLOR_PAIR(renderData.color));
			win.printChar(i, j, renderData.symbol);
			wattroff(win.get(), COLOR_PAIR(renderData.color));
		}
	}

	for(const auto& object : objects) {
		object->fireEvent(renderDataEvent);
		object->fireEvent(positionDataEvent);
		wattron(win.get(), COLOR_PAIR(renderData.color));
		win.printChar(positionData.x, positionData.y, renderData.symbol);
		wattroff(win.get(), COLOR_PAIR(renderData.color));
	}

	for(const auto& enemy : enemies) {
		enemy->fireEvent(renderDataEvent);
		enemy->fireEvent(positionDataEvent);
		wattron(win.get(), COLOR_PAIR(renderData.color));
		win.printChar(positionData.x, positionData.y, renderData.symbol);
		wattroff(win.get(), COLOR_PAIR(renderData.color));
	}
}

void Level::update(Game* game) {
	auto e = eventPool.getResource();
	e->type = EVENT_UPDATE;
	e->data = UpdateEventData({game});

	for(Spark::GameObject* enemy : enemies) {
		if(game->getQuit() == true) {
			return;
		}
		enemy->fireEvent(e);
	}
}

void Level::createLevel(ObjectFactory& objectFactory, bool firstLevel) {
	grid.clearWalls();
	enemies.clear();
	objects.clear();

	for(unsigned int i = 0; i < LEVEL_WIDTH; ++i) {
		for(unsigned int j = 0; j < LEVEL_HEIGHT; ++j) {
			tiles[i][j] = nullptr;
		}
	}

	std::vector<Room> rooms;

	int tries = 0;
	while(tries < 1000) {
		Room room = createRoom();

		bool passes = true;
		for(std::size_t i = 0; i < rooms.size(); ++i) {
			if(rooms[i].intersects(room)) {
				passes = false;
			}
			// create a buffer of at least 3 tiles between the edges of the map and the room
			if(room.position.x < 3 || room.position.x + static_cast<unsigned int>(room.width) >= LEVEL_WIDTH - 3 ||
			   room.position.y < 3 || room.position.y + static_cast<unsigned int>(room.height) >= LEVEL_HEIGHT - 3) {
				passes = false;
			}
		}
		if(passes) {
			if(rooms.size() == 0) {
				playerSpawnPosition.x = (room.position.x + (room.position.x + room.width)) / 2;
				playerSpawnPosition.y = (room.position.y + (room.position.y + room.height)) / 2;
				
				if(!firstLevel) {
					stairsDownPosition = playerSpawnPosition;
					tiles[stairsDownPosition.x][stairsDownPosition.y] = objectFactory.createObject("Stairs Down");
				}
			}
			Position doorPos = {0, 0}, doorExit = {0, 0};
			switch(std::uniform_int_distribution<unsigned int>{0, 3}(eng)) {
				case DIR_NORTH:
					doorPos.x = std::uniform_int_distribution<int>{room.position.x + 1, room.position.x + room.width - 1}(eng);
					doorPos.y = room.position.y - 1;
					doorExit = doorPos;
					--doorExit.y;
					break;
				case DIR_EAST:
					doorPos.x = room.position.x + room.width;
					doorPos.y = std::uniform_int_distribution<int>{room.position.y + 1, room.position.y + room.height - 1}(eng);
					doorExit = doorPos;
					++doorExit.x;
					break;
				case DIR_SOUTH:
					doorPos.x = std::uniform_int_distribution<int>{room.position.x + 1, room.position.x + room.width - 1}(eng);
					doorPos.y = room.position.y + room.height;
					doorExit = doorPos;
					++doorExit.y;
					break;
				case DIR_WEST:
					doorPos.x = room.position.x - 1;
					doorPos.y = std::uniform_int_distribution<int>{room.position.y + 1, room.position.y + room.height - 1}(eng);
					doorExit = doorPos;
					--doorExit.x;
					break;
				default:
					assert(false && "GENERATED DIRECTION THAT DOESNT EXIST");
					break;
			}
			room.doorPos = doorPos;
			room.doorExit = doorExit;
			buildRoom(room, objectFactory);

			rooms.push_back(room);
		}
		++tries;
	}

	stairsUpPosition.x = (rooms.back().position.x + (rooms.back().position.x + rooms.back().width)) / 2;
	stairsUpPosition.y = (rooms.back().position.y + (rooms.back().position.y + rooms.back().height)) / 2;

	tiles[stairsUpPosition.x][stairsUpPosition.y] = objectFactory.createObject("Stairs Up");

	std::vector<std::vector<std::tuple<int, int>>> paths;
	for(std::size_t i = 1; i < rooms.size(); ++i) {
		auto path = pathfinding::aStar::createPath(
			std::make_tuple(rooms[i - 1].doorExit.x, rooms[i - 1].doorExit.y),
			std::make_tuple(rooms[i].doorExit.x, rooms[i].doorExit.y),
			grid
		);
		auto reconstructedPath = pathfinding::reconstructPath(
			std::make_tuple(rooms[i - 1].doorExit.x, rooms[i - 1].doorExit.y),
			std::make_tuple(rooms[i].doorExit.x, rooms[i].doorExit.y),
			path
		);
		
		paths.push_back(reconstructedPath);
	}

	for(const auto& path : paths) {
		for(const auto& pos : path) {
			const auto& [x, y] = pos;
			tiles[x][y] = objectFactory.createObject("Ground");
		}
	}

	for(unsigned int i = 0; i < LEVEL_WIDTH; ++i) {
		for(unsigned int j = 0; j < LEVEL_HEIGHT; ++j) {
			if(tiles[i][j] == nullptr) {
				tiles[i][j] = objectFactory.createObject("Wall");
				grid.createWall(std::make_tuple(i, j));
			}
		}
	}

	// TEST SPAWNS
	auto e = eventPool.getResource();

	enemies.push_back(objectFactory.createObject("Kobold"));
	setPosition(playerSpawnPosition.x - 3, playerSpawnPosition.y + 1, enemies.back(), e);

	for(int i = 0; i < 2; ++i) {
		objects.push_back(objectFactory.spawnObject(COMMON, OBJ_WEAPON));
		setPosition(playerSpawnPosition.x + 1, playerSpawnPosition.y + 1, objects.back(), e);
	}

	for(int i = 0; i < 20; ++i) {
		objects.push_back(objectFactory.spawnObject(RARE, OBJ_ARMOR));
		setPosition(playerSpawnPosition.x + 2, playerSpawnPosition.y + 1, objects.back(), e);
	}
}

void Level::buildRoom(Room room, ObjectFactory& objectFactory) {
	for(int i = 0; i < room.width; ++i) {
		for(int j = 0; j < room.height; ++j) {
			if(tiles[i + room.position.x][j + room.position.y] == nullptr) {
				tiles[i + room.position.x][j + room.position.y] = objectFactory.createObject("Ground");
			}
		}
	}

	for(int i = 0; i < room.height; ++i) {
		tiles[room.position.x - 1][i + room.position.y] = objectFactory.createObject("Wall");
		tiles[room.position.x + room.width][i + room.position.y] = objectFactory.createObject("Wall");

		grid.createWall(std::make_tuple(room.position.x - 1, i + room.position.y));
		grid.createWall(std::make_tuple(room.position.x + room.width, i + room.position.y));
	}
	for(int i = 0; i < room.width; ++i) {
		tiles[i + room.position.x][room.position.y - 1] = objectFactory.createObject("Wall");
		tiles[i + room.position.x][room.position.y + room.height] = objectFactory.createObject("Wall");

		grid.createWall(std::make_tuple(i + room.position.x, room.position.y - 1));
		grid.createWall(std::make_tuple(i + room.position.x, room.position.y + room.height));
	}
	tiles[room.doorPos.x][room.doorPos.y] = objectFactory.createObject("Door");

	// Generate furnishings based off of a few room templates??
}

void Level::eraseEnemy(unsigned int ID) {
	for(std::size_t i = 0; i < enemies.size(); ++i) {
		if(enemies[i]->getID() == ID) {
			std::swap(enemies[i], enemies.back());
			enemies.pop_back();
		}
	}
}

Room Level::createRoom() {
	unsigned int x = std::uniform_int_distribution<unsigned int>{3, LEVEL_WIDTH - (ROOM_WIDTH_MAX + 3)}(eng),
				 y = std::uniform_int_distribution<unsigned int>{3, LEVEL_HEIGHT - (ROOM_HEIGHT_MAX + 3)}(eng);

	unsigned int width = std::uniform_int_distribution<unsigned int>{ROOM_WIDTH_MIN, ROOM_WIDTH_MAX}(eng),
				 height = std::uniform_int_distribution<unsigned int>{ROOM_HEIGHT_MIN, ROOM_HEIGHT_MAX}(eng);

	return Room(x, y, width, height);
}

bool Level::isTraversable(int x, int y) {
	if(inBounds(x, y)) {
		auto e = eventPool.getResource();
		e->type = EVENT_GET_TRAVERSABLE;
		e->data = TraversableEventData();

		tiles[x][y]->fireEvent(e);

		return std::any_cast<TraversableEventData>(e->data).traversable;
	}

	return false;
}

bool Level::isDoor(int x, int y) {
	if(inBounds(x, y)) {
		auto e = eventPool.getResource();
		e->type = EVENT_IS_DOOR;
		e->data = IsDoorEventData();

		tiles[x][y]->fireEvent(e);

		return std::any_cast<IsDoorEventData>(e->data).isDoor;
	}

	return false;
}

bool Level::interactWithDoor(int x, int y) {
	bool open = false;
	if(inBounds(x, y)) {
		auto e = eventPool.getResource();
		e->type = EVENT_DOOR_INTERACT;
		e->data = DoorInteractEventData();

		tiles[x][y]->fireEvent(e);

		open = std::any_cast<DoorInteractEventData>(e->data).open;
	}

	if(open) {
		grid.deleteWall(std::make_tuple(x, y));
	} else {
		grid.createWall(std::make_tuple(x, y));
	}

	return open;
}

bool Level::enemyAt(unsigned int x, unsigned int y) {
	auto e = eventPool.getResource();
	e->type = EVENT_GET_POSITION_DATA;
	e->data = PositionEventData();
	
	const PositionEventData& positionData = std::any_cast<const PositionEventData&>(e->data);

	for(std::size_t i = 0; i < enemies.size(); ++i) {
		enemies[i]->fireEvent(e);
		if(positionData.x == x && positionData.y == y) {
			return true;
		}
	}

	return false;
}

Spark::GameObject* Level::getEnemy(unsigned int index) {
	if(static_cast<std::size_t>(index) < enemies.size())
		return enemies[index];

	return nullptr;
}

Spark::GameObject* Level::getEnemy(unsigned int x, unsigned int y) {
	auto e = eventPool.getResource();
	e->type = EVENT_GET_POSITION_DATA;
	e->data = PositionEventData();

	const PositionEventData& positionData = std::any_cast<const PositionEventData&>(e->data);

	for(const auto& enemy : enemies) {
		enemy->fireEvent(e);
		if(positionData.x == x && positionData.y == y) {
			return enemy;
		}
	}

	return nullptr;
}

std::vector<Spark::GameObject*> Level::getObjectsOnTile(unsigned int x, unsigned int y) {
	std::vector<Spark::GameObject*> objectVec;

	auto e = eventPool.getResource();
	e->type = EVENT_GET_POSITION_DATA;
	e->data = PositionEventData();

	PositionEventData pe = std::any_cast<PositionEventData&>(e->data);

	for(std::size_t i = 0; i < objects.size(); ++i) {
		pe = getPosition(objects[i], e);
		if(pe.x == static_cast<unsigned int>(x) && pe.y == static_cast<unsigned int>(y)) {
			objectVec.push_back(objects[i]);
		}
	}

	return objectVec;
}

void Level::eraseObject(unsigned int ID) {
	for(std::size_t i = 0; i < objects.size(); ++i) {
		if(ID == objects[i]->getID()) {
			std::swap(objects[i], objects.back());
			objects.pop_back();
		}
	}
}

void Level::addObject(Spark::GameObject* object, unsigned int x, unsigned int y) {
	auto e = eventPool.getResource();

	objects.push_back(object);	
	setPosition(x, y, objects.back(), e);
}
