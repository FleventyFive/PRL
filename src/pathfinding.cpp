#include "pathfinding.hpp"

std::vector<std::tuple<int, int>> pathfinding::reconstructPath(std::tuple<int, int> start, std::tuple<int, int> goal, 
	std::unordered_map<std::tuple<int, int>, std::tuple<int, int>>& cameFrom)
{
	std::vector<std::tuple<int, int>> path;
	std::tuple<int, int> current = goal;
	path.push_back(current);

	while(current != start) {
		current = cameFrom[current];
		path.push_back(current);
	}

	path.push_back(start);
	std::reverse(path.begin(), path.end());

	return path;
}

std::unordered_map<std::tuple<int, int>, std::tuple<int, int>> pathfinding::dijkstra::createPath(std::tuple<int, int> start, 
	std::tuple<int, int> goal, PathfindingGrid grid)
{
	std::unordered_map<std::tuple<int, int>, std::tuple<int, int>> cameFrom;
	std::unordered_map<std::tuple<int, int>, double> costSoFar;

	PriorityQueue<std::tuple<int, int>, double> frontier;
	frontier.put(start, 0.0);

	cameFrom[start] = start;
	costSoFar[start] = 0.0;

	while(!frontier.empty()) {
		auto current = frontier.get();

		if(current == goal) {
			break;
		}

		for(const auto& next : grid.neighbors(current)) {
			double newCost = costSoFar[current] + grid.cost(next);
			if(!costSoFar.count(next) || newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				cameFrom[next] = current;
				frontier.put(next, newCost);
			}
		}
	}

	return cameFrom;
}

std::unordered_map<std::tuple<int, int>, std::tuple<int, int>> pathfinding::aStar::createPath(std::tuple<int, int> start, 
	std::tuple<int, int> goal, PathfindingGrid grid)
{
	std::unordered_map<std::tuple<int, int>, std::tuple<int, int>> cameFrom;
	std::unordered_map<std::tuple<int, int>, double> costSoFar;

	PriorityQueue<std::tuple<int, int>, double> frontier;
	frontier.put(start, 0.0);

	cameFrom[start] = start;
	costSoFar[start] = 0.0;

	while(!frontier.empty()) {
		auto current = frontier.get();

		if(current == goal) {
			break;
		}

		for(const auto& next : grid.neighbors(current)) {
			double newCost = costSoFar[current] + grid.cost(next);
			if(!costSoFar.count(next) || newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				cameFrom[next] = current;
				frontier.put(next, newCost + heuristic(next, goal));
			}
		}
	}

	return cameFrom;
}
