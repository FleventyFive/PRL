#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <array>

#include "traversable.hpp"

#include "position.hpp"
#include "spark.hpp"

namespace std {
	template <>
	struct hash<tuple<int,int> > {
		inline size_t operator()(const tuple<int,int>& t) const {
			const auto& [x, y] = t;
			return x * 1812433253 + y;
		}
	};
}

class PathfindingGrid {
private:
	const std::array<std::tuple<int, int>, 4> DIRS = {{
		std::make_tuple(1, 0),
		std::make_tuple(0, -1),
		std::make_tuple(-1, 0),
		std::make_tuple(0, 1)
	}};

	const int height, width;

	std::unordered_set<std::tuple<int, int>> walls;

	std::unordered_map<std::tuple<int, int>, double> costs;

	bool inBounds(int x, int y) const noexcept { return 0 <= x && x <= width && 0 <= y && y <= height; }
	bool passable(int x, int y) const noexcept { return !walls.count(std::make_tuple(x, y)); }

public:
	std::vector<std::tuple<int, int>> neighbors(std::tuple<int, int> id) const;

	void clearWalls() { walls.clear(); }
	void deleteWall(const std::tuple<int, int>& id) noexcept { walls.erase(id); }
	void createWall(const std::tuple<int, int>& id) noexcept { walls.insert(id); }

	double cost(const std::tuple<int, int>& id) noexcept { return costs[id]; }

	PathfindingGrid(int _height, int _width);
	~PathfindingGrid() = default;
};
