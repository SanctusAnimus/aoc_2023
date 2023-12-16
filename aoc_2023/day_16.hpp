#ifndef AOC_DAY_16
#define AOC_DAY_16

#include "solution.hpp"
#include <unordered_set>
#include <fstream>
#include <deque>
#include <execution>
#include <algorithm>

class Day16 : public Solution<Day16> {
public:
	Day16() {
		day_num = 16;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;

	// x y dx dy
	using Beam_T = std::tuple<int, int, int, int>;

	intmax_t do_raycast(SolutionInput_T solution_input, Beam_T& start) {
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
		
		Beam_T p1_beam = { -1, 0, 1, 0 };
		p1_result = do_raycast(solution_input, p1_beam);

		// p2
		size_t columns_count = solution_input[0].size();
		size_t rows_count = solution_input.size();

		std::vector<Beam_T> starting_beams;
		std::vector<intmax_t> energized_per_beam;
		
		// rows, left and right start
		for (int i = 0; i < rows_count; i++) {
			starting_beams.emplace_back(-1, i, 1, 0);
			starting_beams.emplace_back(columns_count, i, -1, 0);
		}

		// columns, bottom and top start
		for (int i = 0; i < columns_count; i++) {
			starting_beams.emplace_back(i, 0, 0, 1);
			starting_beams.emplace_back(i, rows_count, 0, -1);
		}

		energized_per_beam.resize(starting_beams.size(), 0);

		std::transform(std::execution::par_unseq, starting_beams.begin(), starting_beams.end(), energized_per_beam.begin(), [this, &solution_input](Beam_T& beam) {
			return this->do_raycast(solution_input, beam);
		});

		p2_result = *std::max_element(energized_per_beam.begin(), energized_per_beam.end());

		// 7951 - 8148
		return { p1_result, p2_result };
	}
};

#endif