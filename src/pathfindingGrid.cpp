#include "pathfindingGrid.hpp"

PathfindingGrid::PathfindingGrid(int _height, int _width): height(_height), width(_width) {
	// initialize all costs to 1
	for(int i = 0; i < _height; ++i) {
		for(int j = 0; j < _width; ++j) {
			costs[std::make_tuple(i, j)] = 1.0;
		}
	}
}

std::vector<std::tuple<int, int>> PathfindingGrid::neighbors(std::tuple<int, int> id) const {
	const auto& [x, y] = id;
	std::vector<std::tuple<int, int>> results;

	Spark::Event e;
	e.type = EVENT_GET_TRAVERSABLE;
	e.data = TraversableEventData({false});

	for(const auto& dir: DIRS) {
		const auto& [dx, dy] = dir;
			
		if(inBounds(x + dx, y + dy) && passable(x + dx, y + dy)) {
			results.push_back(std::make_tuple(x + dx, y + dy));
		}
		e.data = TraversableEventData({false});
	}

	if((x + y) % 2 == 0) {
		// aesthetic improvement on square grids
		std::reverse(results.begin(), results.end());
	}

	return results;
}