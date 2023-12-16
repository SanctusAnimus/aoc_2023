#ifndef AOC_DAY_16
#define AOC_DAY_16

#include "solution.hpp"
#include <unordered_set>
#include <fstream>
#include <deque>

class Day16 : public Solution<Day16> {
public:
	Day16() {
		day_num = 16;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;

	// x y dx dy
	using Beam_T = std::tuple<int, int, int, int>;

	intmax_t do_raycast(SolutionInput_T solution_input, Beam_T&& start) {
		size_t input_width = solution_input[0].size();
		size_t input_height = solution_input.size();

		std::deque<Beam_T> beams{ start };
		std::vector<std::vector<int>> energized(input_height, std::vector<int>(input_width, 0));

		std::unordered_set<Beam_T> known_states;

		do {
			Beam_T& beam = beams.front();

			if (known_states.contains(beam)) {
				beams.pop_front();
				continue;
			}

			known_states.insert(beam);

			auto& [x, y, dx, dy] = beam;

			x += dx;
			y += dy;

			if (y < 0 || y >= input_height || x < 0 || x >= input_width) {
				beams.pop_front();
				continue;
			}

			switch (solution_input[y][x]) {
				case '/':
				{
					int t = -dy;
					dy = -dx;
					dx = t;
					break;
				}
				case '\\':
				{
					int t = dy;
					dy = dx;
					dx = t;
					break;
				}
				case '|':
				{
					if (dx != 0) {
						beams.emplace_back(x, y, 0, -1);
						dx = 0;
						dy = 1;
					}
					break;
				}
				case '-':
				{
					if (dy != 0) {
						beams.emplace_back(x, y, -1, 0);
						dx = 1;
						dy = 0;
					}
					break;
				}
			}

			energized[y][x] = 1;

		} while (!beams.empty());

		intmax_t count = 0;
		for (const auto& row : energized) {
			for (const auto& el : row) {
				count += el;
			}
		}

		return count;
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;
		
		p1_result = do_raycast(solution_input, {-1, 0, 1, 0});

		// p2

		size_t columns_count = solution_input[0].size();
		size_t rows_count = solution_input.size();

		// rows, left and right start
		for (int i = 0; i < rows_count; i++) {
			intmax_t result_left = do_raycast(solution_input, { -1, i, 1, 0 });
			intmax_t result_right = do_raycast(solution_input, { columns_count, i, -1, 0 });

			if (result_left > p2_result) p2_result = result_left;
			if (result_right > p2_result) p2_result = result_right;
		}

		// columns, bottom and top start
		for (int i = 0; i < columns_count; i++) {
			intmax_t result_top = do_raycast(solution_input, { i, 0, 0, 1 });
			intmax_t result_bottom = do_raycast(solution_input, { i, rows_count, 0, -1 });

			if (result_top > p2_result) p2_result = result_top;
			if (result_bottom > p2_result) p2_result = result_bottom;
		}

		// 7951 - 8148
		return { p1_result, p2_result };
	}
};

#endif