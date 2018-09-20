#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>

#include "pathfindingGrid.hpp"

namespace pathfinding {
		template<typename T, typename priority_t>
		class PriorityQueue {
		public:
			using PQElement = std::pair<priority_t, T>;

			bool empty() { return elements.empty(); }

			void put(T item, priority_t priority) {
				elements.emplace(priority, item);
			}

			T get() {
				T bestItem = elements.top().second;
				elements.pop();
				return bestItem;
			}
		private:
			std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;
		};

		std::vector<std::tuple<int, int>> reconstructPath(std::tuple<int, int> start, std::tuple<int, int> goal, 
			std::unordered_map<std::tuple<int, int>, std::tuple<int, int>>& cameFrom);
	namespace dijkstra {
		std::unordered_map<std::tuple<int, int>, std::tuple<int, int>> createPath(std::tuple<int, int> start, 
			std::tuple<int, int> goal, PathfindingGrid grid);
	};

	namespace aStar {
		inline double heuristic(std::tuple<int, int> a, std::tuple<int, int> b) {
			auto [x1, y1] = a;
			auto [x2, y2] = b;

			return std::abs(x1 - x2) + std::abs(y1 - y2);
		}

		std::unordered_map<std::tuple<int, int>, std::tuple<int, int>> createPath(std::tuple<int, int> start, 
			std::tuple<int, int> goal, PathfindingGrid grid);
	};
};