#ifndef AOC_DAY_17
#define AOC_DAY_17

#include "solution.hpp"
#include <queue>
#include <deque>
#include <unordered_set>

struct Grid_D17 {
	std::vector<std::string>& content;

	size_t width;
	size_t height;
};

struct StepData {
	int x;
	int y;
	int dx;
	int dy;
	int forward;
	int cost;

	friend std::ostream& operator<<(std::ostream& os, const StepData& lhs) {
		os << "<" << lhs.x << ":" << lhs.y << " " << lhs.dx << ":" << lhs.dy << " " << lhs.forward << " " << lhs.cost;

		return os;
	}
};

bool step_compare(const StepData& lhs, const StepData& rhs) {
	return lhs.cost > rhs.cost;
}

class Day17 : public Solution<Day17> {
public:
	Day17() {
		day_num = 17;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;

	intmax_t calculate_best_path(Grid_D17& grid, int min_steps, int max_steps) {
		std::unordered_map<std::tuple<int, int, int, int>, int> visited;
		std::priority_queue<StepData, std::deque<StepData>, decltype(&step_compare)> active(step_compare);

		active.push({0, 0, 1, 0, 0, 0});
		active.push({0, 0, 0, 1, 0, 0});

		while (!active.empty()) {
			const StepData& step = active.top();
			active.pop();

			if (step.x == grid.width - 1 && step.y == grid.height - 1) return step.cost;

			auto apply_turn = [&grid, &active, &visited, &step, &min_steps, &max_steps](int dy, int dx) {
				int d_cost = 0;

				for (int i = 1; i < max_steps + 1; i++) {
					int ty = step.y + dy * i;
					int tx = step.x + dx * i;

					if (ty < 0 || tx < 0 || ty >= grid.height || tx >= grid.width) break;

					d_cost += (grid.content[ty][tx] - '0');

					if (i >= min_steps) {
						int new_cost = step.cost + d_cost;

						std::tuple<int, int, int, int> t = { tx, ty, dx, dy };

						// if previous time we passed through this node had higher cost, override with lower, and continue processing
						// works as a cached stop for cases where we pass nodes with higher overall traversal cost
						if (!visited.contains(t) || visited[t] > new_cost) {
							visited[t] = new_cost;
							active.push({ tx, ty, dx, dy, 0, new_cost });
						}
					}
				}
			};

			// process turns, each steps <max_steps> into new direction
			// and pushes new process nodes for every node after <min_steps>
			apply_turn(-step.dx, step.dy);
			apply_turn(step.dx, -step.dy);
		}

		return 0;
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		Grid_D17 grid{solution_input, solution_input[0].size(), solution_input.size()};
		
		intmax_t p1_result = calculate_best_path(grid, 1, 3);
		intmax_t p2_result = calculate_best_path(grid, 4, 10);

		// 791 - 900
		return { p1_result, p2_result };
	}
};


#endif // !AOC_DAY_17
