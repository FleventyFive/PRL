#include "game.hpp"

Game::Game(): world(), viewport(VIEWPORT_HEIGHT, VIEWPORT_WIDTH, 0, 0), 
	dialogue(VIEWPORT_HEIGHT - 3, 45, VIEWPORT_WIDTH + 1, 3), hud(3, 45, VIEWPORT_WIDTH + 1, 0),
	level(), objectFactory(world, "/Users/markcalhoun/main/c++/Roguelike/objects.blpt"), updateCount(0),
	levelHeight(0) {

	level.createLevel(objectFactory, true);

	for(int i = 0; i < 100; ++i) {
		eventPool.add(std::unique_ptr<Spark::Event>{ std::make_unique<Spark::Event>() });
	}

	auto e = eventPool.getResource();

	PositionEventData pe = level.getPlayerSpawnPosition();
	
	player = objectFactory.createObject("Player");
	setPosition(pe.x, pe.y, player, e);

 	addToInventory(player, objectFactory.createObject("DevSword"), e);
	
	dialogue.addMessage("Welcome to Roguelike %s", getRenderData(player, e).name.c_str());
	draw();

	while(!quit) {
		while(playerCanAct() && !quit) {
			processInput();
			if(playerCanAct()) {
				// Only draw if player can continue acting to avoid flickering
				// from printing here and directly after when the entities update
				draw();
			}
		}
		if(!quit) {
			update();
			draw();
		}
	}

	if(!isAlive(player, e)) {
		viewport.clear();
		viewport.mvprint(0, 0, "Player dead");
		viewport.refresh();
		getch();
	}
}

void Game::draw() {
	drawViewport();
	viewport.refresh();

	hud.clear();
	drawHud();
	hud.refresh();

	dialogue.printMessages();
	dialogue.resetCounter();
}

void Game::drawViewport() {
	level.draw(viewport);

	auto renderDataEvent = eventPool.getResource();
	renderDataEvent->type = EVENT_GET_RENDER_DATA;
	renderDataEvent->data = GetRenderEventData();
	
	auto positionDataEvent = eventPool.getResource();
	positionDataEvent->type = EVENT_GET_POSITION_DATA;
	positionDataEvent->data = PositionEventData();

	const GetRenderEventData& renderData = std::any_cast<const GetRenderEventData&>(renderDataEvent->data);

	PositionEventData positionData = getPlayerPos();

	player->fireEvent(renderDataEvent);
	wattron(viewport.get(), COLOR_PAIR(renderData.color));
	viewport.printChar(positionData.x, positionData.y, renderData.symbol);
	wattroff(viewport.get(), COLOR_PAIR(renderData.color));

	viewport.refresh();
}

void Game::drawHud() {
	auto energyDataEvent = eventPool.getResource();
	auto healthDataEvent = eventPool.getResource();
	auto renderDataEvent = eventPool.getResource();
	auto statsDataEvent = eventPool.getResource();
	auto getInventoryDataEvent = eventPool.getResource();

	EnergyEventData energyData = getEnergyData(player, energyDataEvent);
	HealthEventData healthData = getHealthData(player, healthDataEvent);
	GetRenderEventData renderData = getRenderData(player, renderDataEvent);
	GetStatsEventData statsData = getStatsData(player, statsDataEvent);
	InventorySizeEventData inventoryData = getInventoryData(player, getInventoryDataEvent);

	wattron(hud.get(), COLOR_PAIR(COLOR_WHITE));

	hud.mvprint(0, 0, "%s - Update: %d, Inventory size: %d\nEnergy: %d/%d, Health: %0.0f/%0.0f, Defense: %0.0f\nLevel Pos: %d",
		renderData.name.c_str(), updateCount, inventoryData.size, 
		energyData.energy, energyData.maxEnergy, healthData.health, healthData.maxHealth, statsData.defense,
		levelHeight);

	wattroff(hud.get(), COLOR_PAIR(COLOR_WHITE));
}

void Game::update() {
	auto e = eventPool.getResource();
	e->type = EVENT_UPDATE;
	e->data = UpdateEventData({this});

	level.update(this);

	player->fireEvent(e);

	playerWaiting = false;
	++updateCount;
}

void Game::processInput() {
	input = getch();

	auto e = eventPool.getResource();

	switch (input) {
		case 'Q':
			quit = true;
			break;
		case 'w':
		case 'a':
		case 's':
		case 'd':
			movePlayer(input);
			break;
		case 'g':
			playerPickUpItem();
			break;
		case 'i':
			playerInspectInventory();
			break;
		case '.':
			playerWaiting = true;
			break;
		case 'x':
			// Inspect terrain, inspects data will be displayed in the
			// dialogue window
			break;
		case 'c':
			playerCloseDoor();
			break;
		case '<':
			moveLevelUp();
			break;
		case '>':
			moveLevelDown();
			break;
		default:
			dialogue.addMessage("Command %c not recognized", input);
			break;
	}
}

void Game::movePlayer(const char& direction) {
	auto e = eventPool.getResource();

	PositionEventData position = getPosition(player, e);

	int xNew = position.x, yNew = position.y;

	switch(direction) {
		case 'w':
			--yNew;
			break;
		case 'a':
			--xNew;
			break;
		case 's':
			++yNew;
			break;
		case 'd':
			++xNew;
			break;
	}

	if(Spark::GameObject* enemy = level.getEnemy(xNew, yNew); enemy != nullptr) {
		GetRenderEventData rde = getRenderData(enemy, e);

		e->type = EVENT_DEAL_MELEE_DAMAGE;
		e->data = DamageEventData();
		player->fireEvent(e);

		e->type = EVENT_TAKE_DAMAGE;

		enemy->fireEvent(e);

		dialogue.addMessage("You dealt %d damage", std::any_cast<DamageEventData>(e->data).damage);

		if(!isAlive(enemy, e)) {
			world.destroyGameObject(enemy);
			level.eraseEnemy(enemy->getID());
			dialogue.addMessage("You killed the %s", rde.name.c_str());
		}
		useEnergy(player, 100, e);
	} else if(level.isTraversable(xNew, yNew)) {
		setPosition(xNew, yNew, player, e);

		useEnergy(player, 100, e);
	} else if(level.isDoor(xNew, yNew)) {
		// Interact with the door only if it is closed
		// AKA, not trversable. This way the door won't
		// Be closed if you try to walk on it.
		level.interactWithDoor(xNew, yNew);

		useEnergy(player, 50, e);
	} else {
		dialogue.addMessage("This terrain is not traversable");
	}
}

void Game::endGame() {
	quit = true;
}

void Game::playerInspectInventory() {
	bool done = false;

	auto e = eventPool.getResource();

	InventoryObjectsEventData ioe = getInventoryObjects(player, e);

	assert(ioe.inventory != nullptr);

	std::vector<Spark::GameObject*>& inventory = *ioe.inventory;

	if(inventory.size() > 0) {
		// The objectSelectionWindow gets 8 lines of display, and sits next to viewport
		Window itemSelectionWindow = Window(8, 57, VIEWPORT_WIDTH + 1, 0);
		// The controlsWindow gets 2 lines of display
		Window controlsWindow = Window(2, 57, VIEWPORT_WIDTH + 1, 8);
		// The descriptionWindow gets the rest of the space
		Window descriptionWindow = Window(VIEWPORT_HEIGHT - 10, 57, VIEWPORT_WIDTH + 1, 10);

		// Print description controls
		wattron(descriptionWindow.get(), COLOR_PAIR(COLOR_WHITE));
		descriptionWindow.fill('%');
		descriptionWindow.refresh();
		wattroff(descriptionWindow.get(), COLOR_PAIR(COLOR_WHITE));

		// Print menu controls
		wattron(controlsWindow.get(), COLOR_PAIR(COLOR_WHITE));
		controlsWindow.mvprint(0, 0, "[w/s] Scroll [enter] Select [c] Cancel");
		controlsWindow.refresh();
		wattroff(controlsWindow.get(), COLOR_PAIR(COLOR_WHITE));

		auto isEquipped = eventPool.getResource();
		isEquipped->type = EVENT_GET_IS_EQUIPPED;
		isEquipped->data = EquippedEventData();
		const EquippedEventData& ee = std::any_cast<const EquippedEventData&>(isEquipped->data);

		auto renderData = eventPool.getResource();
		renderData->type = EVENT_GET_RENDER_DATA;
		renderData->data = GetRenderEventData();
		const GetRenderEventData& rde = std::any_cast<const GetRenderEventData&>(renderData->data);

		int menuPos = 0, itemPos = 0;
		char menuInput = '\0';

		while(menuInput != 'c' && menuInput != ESCAPE_KEY && !done) {
			descriptionWindow.clear();

			descriptionWindow.clear();
			descriptionWindow.fill('%');
			descriptionWindow.refresh();

			itemSelectionWindow.clear();

			wattron(itemSelectionWindow.get(), COLOR_PAIR(COLOR_WHITE));
			// Print the items
			for(std::size_t i = 0; i < std::min(inventory.size(), (size_t)8); ++i) {
				inventory[i + (itemPos - menuPos)]->fireEvent(renderData);
				inventory[i + (itemPos - menuPos)]->fireEvent(isEquipped);

				if(ee.equipped != true)
					itemSelectionWindow.mvprint(1, i, " - %s", rde.name.c_str());
				else
					itemSelectionWindow.mvprint(1, i, " * %s", rde.name.c_str());
			}

			// Print the cursor
			itemSelectionWindow.printChar(0, menuPos, '>');
			wattroff(itemSelectionWindow.get(), COLOR_PAIR(COLOR_WHITE));

			// Display the changes
			itemSelectionWindow.refresh();
			descriptionWindow.refresh();

			// Clear the cursor
			itemSelectionWindow.printChar(0, menuPos, ' ');

			menuInput = getch();

			switch(menuInput) {
				case 'w':
				case 'W':
					if(menuPos > 0) {
						--menuPos;
					}
					if(itemPos > 0) {
						--itemPos;
					}
					break;
				case 's':
				case 'S':
					if((std::size_t)menuPos < std::min((inventory.size() - 1), (std::size_t)7)) {
						++menuPos;
					}
					if((std::size_t)itemPos < (inventory.size() - 1)) {
						++itemPos;
					}
					break;
				case '\n':
					dialogue.printMessages();
					
					hud.clear();
					hud.refresh();
					
					viewport.clear();
					viewport.refresh();
					
					playerPickAction(inventory, itemPos);
					done = true;
					break;
				default:
					break;
			}
		}

	descriptionWindow.clear();
	descriptionWindow.refresh();
	} else {
		dialogue.addMessage("Your inventory is empty");
	}

	hud.clear();
	drawHud();
	hud.refresh();
}

void Game::playerPickAction(std::vector<Spark::GameObject*>& inventory, int itemPos) {
	bool done = false;
	auto e = eventPool.getResource();
	// controlsWindow holds The controls and takes the top line of the viewport
	Window controlsWindow = Window(1, VIEWPORT_WIDTH, 0, 0);
	// detailsWindow holds the item name an flavor text and takes the left half of the viewport
	Window descriptionWindow = Window(VIEWPORT_HEIGHT - 2, VIEWPORT_WIDTH / 2, 0, 2);
	// menuWindow holds the options available and takes the right half of the viewport
	Window menuWindow = Window(VIEWPORT_HEIGHT - 2, VIEWPORT_WIDTH / 2 - 1, VIEWPORT_WIDTH / 2 + 1, 2);

	char menuInput = '\0';
	int menuPos = 0;

	wattron(controlsWindow.get(), COLOR_PAIR(COLOR_WHITE));
	controlsWindow.mvprint(0, 0, "[w/s] Scroll [enter] Select [c] Cancel");
	controlsWindow.refresh();
	wattroff(controlsWindow.get(), COLOR_PAIR(COLOR_WHITE));

	std::vector<std::string> menuChoices;
	menuChoices.push_back("Equip");
	menuChoices.push_back("Drop");

	wattron(menuWindow.get(), COLOR_PAIR(COLOR_WHITE));
	for(std::size_t i = 0; i < menuChoices.size(); ++i)
		menuWindow.mvprint(2, i, menuChoices[i].c_str());
	wattroff(menuWindow.get(), COLOR_PAIR(COLOR_WHITE));

	// Get name and description
	GetRenderEventData rde = getRenderData(inventory[itemPos], e);

	std::string divider;
	divider.insert(0, VIEWPORT_WIDTH / 2, '-');

	// print name an description
	wattron(descriptionWindow.get(), COLOR_PAIR(COLOR_WHITE));
	descriptionWindow.mvprint(0, 0, rde.name.c_str());
	descriptionWindow.mvprint(0, 1, divider.c_str());
	descriptionWindow.mvprint(0, 2, wrapByWord(VIEWPORT_WIDTH / 2, rde.description).c_str());
	descriptionWindow.refresh();
	wattroff(descriptionWindow.get(), COLOR_PAIR(COLOR_WHITE));

	while(menuInput != 'c' && menuInput != ESCAPE_KEY && !done) {
		wattron(menuWindow.get(), COLOR_PAIR(COLOR_WHITE));
		// Print cursor
		menuWindow.printChar(0, menuPos, '>');
		menuWindow.refresh();
		wattroff(menuWindow.get(), COLOR_PAIR(COLOR_WHITE));
		// Erase cursor for next print
		menuWindow.printChar(0, menuPos, ' ');

		menuInput = getch();
		
		switch(menuInput) {
			case 'w':
			case 'W':
				if(menuPos > 0) {
					menuPos--;
				}
				break;
			case 's':
			case 'S':
				if((std::size_t)menuPos < menuChoices.size() - 1) {
					menuPos++;
				}
				break;
			case '\n':
				if(menuChoices[menuPos] == "Equip")
					playerEquip(inventory, itemPos);
				else if(menuChoices[menuPos] == "Drop")
					playerDrop(inventory, itemPos);
				done = true;
				break;
			default:
				break;
		}
	}
}

bool Game::playerCanAct() {
	if(!playerWaiting) {
		auto e = eventPool.getResource();
		return canAct(player, e);
	}

	return false;
}

void Game::playerPickUpItem() {
	auto e = eventPool.getResource();

	// The objectSelectionWindow gets 8 lines of display, and sits next to viewport
	Window itemSelectionWindow = Window(8, 57, VIEWPORT_WIDTH + 1, 0);
	// The controlsWindow gets 2 lines of display
	Window controlsWindow = Window(2, 57, VIEWPORT_WIDTH + 1, 8);
	// The descriptionWindow gets the rest of the space
	Window descriptionWindow = Window(VIEWPORT_HEIGHT - 10,  57, VIEWPORT_WIDTH + 1, 10);

	wattron(itemSelectionWindow.get(), COLOR_PAIR(COLOR_WHITE));
	itemSelectionWindow.mvprint(0, 0, "Pick up where? w,a,s,d, [c] Cancel");
	wattroff(itemSelectionWindow.get(), COLOR_PAIR(COLOR_WHITE));
	itemSelectionWindow.refresh();
	itemSelectionWindow.mvprint(0, 0, "                                  ");

	PositionEventData positionData = getPosition(player, e);

	int pickUpPositionX = positionData.x, pickUpPositionY = positionData.y;

	char dir = getch();

	switch(dir) {
		case 'w':
		case 'W':
			--pickUpPositionY;
			break;
		case 'a':
		case 'A':
			--pickUpPositionX;
			break;
		case 's':
		case 'S':
			++pickUpPositionY;
			break;
		case 'd':
		case 'D':
			++pickUpPositionX;
			break;
		case 'c':
		case 'C':
			dialogue.addMessage("Canceled");
			return;
			break;
		default:
			dialogue.addMessage("Direction not recognized");
			return;
			break;
	}

	if(static_cast<unsigned int>(pickUpPositionX) >= VIEWPORT_WIDTH || pickUpPositionX < 0
	|| static_cast<unsigned int>(pickUpPositionY) >= VIEWPORT_HEIGHT || pickUpPositionY < 0) {
		dialogue.addMessage("Out of bounds");
		return;
	}

	std::vector<Spark::GameObject*> items = level.getObjectsOnTile(pickUpPositionX, pickUpPositionY);

	if(items.size() > 0) {
		int menuPos = 0, itemPos = 0;
		char menuInput = '\0';

		wattron(controlsWindow.get(), COLOR_PAIR(COLOR_WHITE));
		// Print menu controls
		controlsWindow.mvprint(0, 0, "[w/s] Scroll [enter] Select [c] Cancel");
		controlsWindow.refresh();
		wattroff(controlsWindow.get(), COLOR_PAIR(COLOR_WHITE));

		// Allow the player to search through the menu
		while(menuInput != 'c' && menuInput != ESCAPE_KEY) {
			itemSelectionWindow.clear();
			descriptionWindow.clear();

			e->type = EVENT_GET_RENDER_DATA;
			e->data = GetRenderEventData();

			const GetRenderEventData& rde = std::any_cast<const GetRenderEventData&>(e->data);

			// Print the objects
			wattron(itemSelectionWindow.get(), COLOR_PAIR(COLOR_WHITE));
			for(std::size_t i = 0; i < std::min(items.size(), (std::size_t)8); ++i) {
				items[i + (itemPos - menuPos)]->fireEvent(e);
				itemSelectionWindow.mvprint(1, i, " - %s", rde.name.c_str());
			}

			// Print the cursor
			itemSelectionWindow.printChar(0, menuPos, '>');
			wattroff(itemSelectionWindow.get(), COLOR_PAIR(COLOR_WHITE));

			itemSelectionWindow.refresh();
			descriptionWindow.refresh();

			menuInput = getch();

			switch(menuInput) {
				case 'w':
				case 'W':
					if(menuPos > 0) {
						--menuPos;
					}
					if(itemPos > 0) {
						--itemPos;
					}
					break;
				case 's':
				case 'S':
					if((std::size_t)menuPos < std::min((items.size() - 1), (std::size_t)7)) {
						++menuPos;
					}
					if((std::size_t)itemPos < (items.size() - 1)) {
						++itemPos;
					}
					break;
				case '\n':
					// Get render data using pre-existing event
					items[itemPos]->fireEvent(e);
					dialogue.addMessage("You got the %s", rde.name.c_str());

					addToInventory(player, items[itemPos], e);

					level.eraseObject(items[itemPos]->getID());

					useEnergy(player, 50, e);
					return;
					break;
				case 'c':
				case 'C':
					dialogue.addMessage("Canceled");
					break;
				default:
					break;
			}
		}
	} else {
		dialogue.addMessage("There's nothing there");
	}
}

void Game::playerEquip(std::vector<Spark::GameObject*>& inventory, int itemPos) {
	auto e = eventPool.getResource();

	if(!isEquipped(inventory[itemPos], e)) {
		TypeEventData te = getTypeData(inventory[itemPos], e);

		auto getEquipped = eventPool.getResource();
		getEquipped->data = ObjectEventData();

		switch(te.type) {
			case OBJ_WEAPON:
				getEquipped->type = EVENT_GET_EQUIPPED_WEAPON;
				e->type = EVENT_EQUIP_WEAPON;
				break;
			case OBJ_ARMOR:
			{
				TypeEventData subtype = getArmorTypeData(inventory[itemPos], e);

				switch(subtype.type) {
					case ARMOR_CHEST:
						getEquipped->type = EVENT_GET_EQUIPPED_CHEST_ARMOR;
						e->type = EVENT_EQUIP_CHEST_ARMOR;
						break;
					case ARMOR_HEAD:
						getEquipped->type = EVENT_GET_EQUIPPED_HEAD_ARMOR;
						e->type = EVENT_EQUIP_HEAD_ARMOR;
						break;
					case ARMOR_LEG:
						getEquipped->type = EVENT_GET_EQUIPPED_LEG_ARMOR;
						e->type = EVENT_EQUIP_LEG_ARMOR;
						break;
					case ARMOR_FOOT:
						getEquipped->type = EVENT_GET_EQUIPPED_FOOT_ARMOR;
						e->type = EVENT_EQUIP_FOOT_ARMOR;
						break;
					default:
						dialogue.addMessage("Attempting to equip obj of unknown armor subtype.");	
						return;
						break;
				}
			}
				break;
			default:
				dialogue.addMessage("Attempting to equip obj of unknown type.");
				return;
				break;
		}

		player->fireEvent(getEquipped);

		Spark::GameObject* lastEquipped = std::any_cast<ObjectEventData&>(getEquipped->data).obj;

		e->data = EquipObjectEventData({inventory[itemPos], false});
		player->fireEvent(e);

		if(std::any_cast<EquipObjectEventData>(e->data).success) {
			setIsEquipped(inventory[itemPos], true, e);

			dialogue.addMessage("You equipped the %s", getRenderData(inventory[itemPos], e).name.c_str());

			if(lastEquipped != nullptr) {
				useEnergy(player, 50, e);

				setIsEquipped(lastEquipped, false, e);
			} else {
				useEnergy(player, 25, e);
			}
		}
	} else {
		dialogue.addMessage("The object is already equipped");
	}
}

void Game::playerDrop(std::vector<Spark::GameObject*>& inventory, int itemPos) {
	auto e = eventPool.getResource();
	if(isEquipped(inventory[itemPos], e)) {
		TypeEventData te = getTypeData(inventory[itemPos], e);

		switch(te.type) {
			case OBJ_WEAPON:
				equipObject(player, nullptr, EVENT_EQUIP_WEAPON, e);
				break;
			case OBJ_ARMOR:
			{
				TypeEventData subtype = getArmorTypeData(inventory[itemPos], e);

				switch(subtype.type) {
					case ARMOR_CHEST:
						equipObject(player, nullptr, EVENT_EQUIP_CHEST_ARMOR, e);
						break;
					case ARMOR_HEAD:
						equipObject(player, nullptr, EVENT_EQUIP_HEAD_ARMOR, e);
						break;
					case ARMOR_LEG:
						equipObject(player, nullptr, EVENT_EQUIP_LEG_ARMOR, e);
						break;
					case ARMOR_FOOT:
						equipObject(player, nullptr, EVENT_EQUIP_FOOT_ARMOR, e);
						break;
					default:
						dialogue.addMessage("Attempting to drop obj of unknown armor subtype.");	
						return;
						break;
				}
			}
				break;
			default:
				dialogue.addMessage("Attempting to drop obj of unknown type.");
				return;
				break;
		}

		useEnergy(player, 50, e);

		setIsEquipped(inventory[itemPos], false, e);
	} else {
		useEnergy(player, 25, e);
	}

	drawViewport();

	wattron(hud.get(), COLOR_PAIR(COLOR_WHITE));
	hud.clear();
	hud.mvprint(0, 0, "[w] Up [a] Left [s] Down [d] Right");
	hud.refresh();
	wattroff(hud.get(), COLOR_PAIR(COLOR_WHITE));

	char dir = getch();
	PositionEventData positionData = getPosition(player, e);

	int xNew = positionData.x, yNew = positionData.y;

	switch(dir) {
		case 'w':
		case 'W':
			--yNew;
			break;
		case 'a':
		case 'A':
			--xNew;
			break;
		case 's':
		case 'S':
			++yNew;
			break;
		case 'd':
		case 'D':
			++xNew;
			break;
		default:
			break;
	}

	if(level.isTraversable(xNew, yNew)) {
		level.addObject(inventory[itemPos], xNew, yNew);

		dialogue.addMessage("You dropped the %s", getRenderData(inventory[itemPos], e).name.c_str());

		std::swap(inventory[itemPos], inventory.back());
		inventory.pop_back();
	} else {
		dialogue.addMessage("You cant drop an object there!");
	}
}

void Game::playerCloseDoor() {
	wattron(hud.get(), COLOR_PAIR(COLOR_WHITE));
	hud.clear();
	hud.mvprint(0, 0, "[w] Up [a] Left [s] Down [d] Right");
	hud.refresh();
	wattroff(hud.get(), COLOR_PAIR(COLOR_WHITE));

	auto e = eventPool.getResource();

	char dir = getch();
	PositionEventData positionData = getPosition(player, e);

	int xNew = positionData.x, yNew = positionData.y;

	switch(dir) {
		case 'w':
		case 'W':
			--yNew;
			break;
		case 'a':
		case 'A':
			--xNew;
			break;
		case 's':
		case 'S':
			++yNew;
			break;
		case 'd':
		case 'D':
			++xNew;
			break;
		default:
			break;
	}

	if(level.isDoor(xNew, yNew) && level.isTraversable(xNew, yNew)) {
		level.interactWithDoor(xNew, yNew);

		useEnergy(player, 50, e);
	} else {
		dialogue.addMessage("There's no door there!");
	}
}

void Game::moveLevelUp() {
	auto e = eventPool.getResource();

	PositionEventData playerPosition = getPosition(player, e);
	PositionEventData position = level.getStairsUpPosition();
	if(position.x == playerPosition.x && position.y == playerPosition.y) {
		++levelHeight;
		level.createLevel(objectFactory, false);

		PositionEventData pe = level.getPlayerSpawnPosition();
		setPosition(pe.x, pe.y, player, e);

		useEnergy(player, 100, e);
	} else {
		dialogue.addMessage("There are no stairs up here!");
	}
}

void Game::moveLevelDown() {
	auto e = eventPool.getResource();

	PositionEventData playerPosition = getPosition(player, e);
	PositionEventData position = level.getStairsDownPosition();
	if(position.x == playerPosition.x && position.y == playerPosition.y) {
		--levelHeight;
		level.createLevel(objectFactory, (levelHeight == 0));
		
		PositionEventData pe = level.getPlayerSpawnPosition();
		setPosition(pe.x, pe.y, player, e);
		
		useEnergy(player, 100, e);
	} else {
		dialogue.addMessage("There are no stairs down here!");
	}
}

PositionEventData Game::getPlayerPos() {
	auto e = eventPool.getResource();
	e->type = EVENT_GET_POSITION_DATA;
	e->data = PositionEventData();

	player->fireEvent(e);

	return std::any_cast<PositionEventData>(e->data);
}