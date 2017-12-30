#include "koboldAI.hpp"
#include "game.hpp"

void KoboldAIComponent::fireEvent(Spark::Event* e) {
	switch(e->type) {
		case EVENT_UPDATE:
		{
			Spark::Event canAct;
			canAct.type = EVENT_CAN_ACT;
			canAct.data = CanActEventData({false});

			Spark::Event useEnergy;
			useEnergy.type = EVENT_USE_ENERGY;

			getOwner()->fireEvent(&canAct);
			while(std::any_cast<CanActEventData>(canAct.data).canAct) {
				Spark::Event aiEvent;

				// get the update event data
				UpdateEventData& ue = std::any_cast<UpdateEventData&>(e->data);

				// get player position
				PositionEventData playerPosition = ue.game->getPlayerPos();

				// create a new path if the player has moved
				if(lastPlayerPosition.x != playerPosition.x || lastPlayerPosition.y != playerPosition.y) {
					aiEvent.type = EVENT_GET_POSITION_DATA;
					aiEvent.data = PositionEventData();

					// get current position
					getOwner()->fireEvent(&aiEvent);

					PositionEventData& koboldPosition = std::any_cast<PositionEventData&>(aiEvent.data);

					// create the path
					auto path = pathfinding::aStar::createPath(
						std::make_tuple(koboldPosition.x, koboldPosition.y),
						std::make_tuple(playerPosition.x, playerPosition.y),
						ue.game->getLevel().getPathfindingGrid()
					);

					// ensure a path to the player is found by checking if the player position was reached
					if(path.count(std::make_tuple(playerPosition.x, playerPosition.y))) {
						// if path to player is found, start following it
						auto reconstructedPath = pathfinding::reconstructPath(
							std::make_tuple(koboldPosition.x, koboldPosition.y),
							std::make_tuple(playerPosition.x, playerPosition.y),
							path
						);

						currentPath = reconstructedPath;
						currentPathIndex = 2;

						hasPathToPlayer = true;
					} else {
						hasPathToPlayer = false;
					}

					lastPlayerPosition = playerPosition;
				}
				// check if player is next to kobold
				if(hasPathToPlayer) {
					const auto& [xNew, yNew] = currentPath[currentPathIndex];

					if(playerPosition.x == static_cast<unsigned int>(xNew) && playerPosition.y == static_cast<unsigned int>(yNew)) {
						aiEvent.type = EVENT_DEAL_MELEE_DAMAGE;
						aiEvent.data = DamageEventData();

						getOwner()->fireEvent(&aiEvent);

						aiEvent.type = EVENT_TAKE_DAMAGE;

						ue.game->getPlayer()->fireEvent(&aiEvent);

						aiEvent.type = EVENT_GET_HEALTH_DATA;
						aiEvent.data = HealthEventData();

						ue.game->getPlayer()->fireEvent(&aiEvent);

						if(std::any_cast<HealthEventData>(aiEvent.data).health <= 0) {
							ue.game->endGame();
						}
					} else {
						// assert(false);
						aiEvent.type = EVENT_SET_POSITION_DATA;
						aiEvent.data = PositionEventData({static_cast<unsigned int>(xNew), static_cast<unsigned int>(yNew)});
	
						getOwner()->fireEvent(&aiEvent);
						++currentPathIndex;
					}
				}

				useEnergy.data = EnergyEventData({100, 0});

				getOwner()->fireEvent(&useEnergy);
				getOwner()->fireEvent(&canAct);
			}
		}
			break;
		default:
			break;
	}
}
