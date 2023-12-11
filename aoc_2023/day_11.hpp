#ifndef AOC_DAY_11
#define AOC_DAY_11

#include <cstdint>
#include "solution.hpp"


class Day11 : public Solution<Day11> {
public:
	Day11() {
		day_num = 11;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;

	using Galaxy_T = std::pair<intmax_t, intmax_t>;

	inline intmax_t get_distance(const Galaxy_T& p1, const Galaxy_T& p2) {
		return abs(p2.first - p1.first) + abs(p2.second - p1.second);
	}

	void condense(std::vector<int>& v) {
		// turn vector of flags into much shorter vector of flag positions to iterate later
		// 0 0 1 0 0 1 => 2 6
		int appropriate = 0;

		for (int i = 0; i < v.size(); i++) {
			if (v[i] == 1) {
				v[appropriate++] = i;
				v[i] = 0;
			}
		}

		v.resize(appropriate);
	}

	inline intmax_t get_shift_from(std::vector<int>& source, int coordinate) {
		// iterate backwards, and reduce shift from size to 0 gradually
		intmax_t shift = source.size();

		if (coordinate < source.front()) return 0;
		if (coordinate > source.back()) return shift;

		for (auto& i : std::views::reverse(source)) {
			if (coordinate > i) break;
			shift--;
		}

		return shift;
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

		size_t row_no = 0;

		size_t line_size = solution_input[0].size();

		// start with all flag shifts being set, and unset whichever have galaxies in them
		std::vector<int> row_shift;
		row_shift.resize(solution_input.size(), 1);

		std::vector<int> column_shift;
		column_shift.resize(line_size, 1);

		std::vector<Galaxy_T> p1_galaxies;
		std::vector<Galaxy_T> p2_galaxies;

		// find all galaxies, and find empty rows at the same time
		for (const auto& line : solution_input) {
			size_t char_no = 0;
			for (const char& c : line) {
				if (c == '#') {
					row_shift[row_no] = 0;
					column_shift[char_no] = 0;

					p1_galaxies.emplace_back(char_no, row_no);
				}
				char_no++;
			}

			row_no++;
		}

		p2_galaxies.reserve(p1_galaxies.size());

		condense(row_shift);
		condense(column_shift);

		// offset by 1 cause starting is already in the input and accounted for
		const int p2_multiplier = 1000000 - 1;

		for (auto& galaxy : p1_galaxies) {
			intmax_t shift_x = get_shift_from(column_shift, galaxy.first);
			intmax_t shift_y = get_shift_from(row_shift, galaxy.second);
			const auto& t = p2_galaxies.emplace_back(galaxy.first + shift_x * p2_multiplier, galaxy.second + shift_y * p2_multiplier);

			galaxy.first += shift_x;
			galaxy.second += shift_y;
		}

		for (int i = 0; i < p1_galaxies.size(); i++) {
			const auto& g1 = p1_galaxies[i];
			const auto& p2_g1 = p2_galaxies[i];

			for (int j = i + 1; j < p1_galaxies.size(); j++) {
				const auto& g2 = p1_galaxies[j];
				const auto& p2_g2 = p2_galaxies[j];

				p1_result += get_distance(g2, g1);
				p2_result += get_distance(p2_g2, p2_g1);
			}
		}

		// 10289334 - 649862989626
		return { p1_result, p2_result };
	}
};


#endif // !AOC_DAY_11
