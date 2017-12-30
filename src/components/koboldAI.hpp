#pragma once

#include <iostream>

#include "spark.hpp"
#include "events.hpp"
#include "../pathfinding.hpp"
#include "../pathfindingGrid.hpp"

class KoboldAIComponent: public Spark::Component {
private:
	PositionEventData lastPlayerPosition;

	std::vector<std::tuple<int, int>> currentPath;

	int currentPathIndex;

	bool hasPathToPlayer = false;

public:
	void fireEvent(Spark::Event* e) final;

	KoboldAIComponent(): Component(Spark::getComponentID<KoboldAIComponent>()), lastPlayerPosition({0, 0}), currentPathIndex(2) { }
};