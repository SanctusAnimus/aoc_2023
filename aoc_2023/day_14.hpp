#ifndef AOC_DAY_14
#define AOC_DAY_14

#include <cstdint>
#include "solution.hpp"

namespace std
{

	template <>
	struct hash<std::vector<std::string>> {
		size_t operator()(const std::vector<std::string>& vec) const {
			size_t hash_value = 0;

			// Combine the hash values of individual strings in the vector
			for (const auto& str : vec) {
				hash_value ^= std::hash<std::string>{}(str)+0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
			}

			return hash_value;
		}
	};

}


class Day14 : public Solution<Day14> {
public:
	Day14() {
		day_num = 14;		
		// 103333 - 97241
		p1_expected = 103333;
		p2_expected = 97241;

		resolve_benchmark_tries = 100;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;
	using HashSet_T = std::unordered_map<std::vector<std::string>, unsigned>;

	void tilt_north(SolutionInput_T buffer) {
		size_t line_size = buffer[0].size();

		for (int i = 1; i < buffer.size(); i++) {
			// try moving all 0 upwards, if supposed place is .

			int lines_traced = i;
			bool moved = false;
			do {
				moved = false;
				for (int j = 0; j < line_size; j++) {
					if (buffer[lines_traced][j] == 'O' && buffer[lines_traced - 1][j] == '.') {
						buffer[lines_traced - 1][j] = 'O';
						buffer[lines_traced][j] = '.';
						moved = true;
					}
				}
				lines_traced--;
			} while (lines_traced > 0 && moved);
		}
	}

	void tilt_south(SolutionInput_T buffer) {
		size_t line_size = buffer[0].size();

		for (int i = buffer.size() - 2; i >= 0; i--) {
			// try moving all 0 downwards, if supposed place is .

			int lines_traced = i;
			bool moved = false;
			do {
				moved = false;
				for (int j = 0; j < line_size; j++) {
					if (buffer[lines_traced][j] == 'O' && buffer[lines_traced + 1][j] == '.') {
						buffer[lines_traced + 1][j] = 'O';
						buffer[lines_traced][j] = '.';
						moved = true;
					}
				}
				lines_traced++;
			} while (lines_traced < buffer.size() - 1 && moved);
		}
	}

	void tilt_east(SolutionInput_T buffer) {
		size_t line_size = buffer[0].size();

		for (auto& line : buffer) {
			for (int i = line_size - 1; i >= 0; i--) {
				if (line[i] == 'O') {
					int offset = 1;
					while (offset < line_size && line[i + offset] == '.') {
						line[i + offset] = 'O';
						line[i + offset - 1] = '.';
						offset++;
					}
				}
			}
		}
	}

	void tilt_west(SolutionInput_T buffer) {
		size_t line_size = buffer[0].size();

		for (auto& line : buffer) {
			//std::cout << line << "\n";
			for (int i = 0; i < line_size; i++) {
				if (line[i] == 'O') {
					int offset = 1;
					//std::cout << i - offset << " " << line[i - offset] << "\n";
					while ((i - offset) >= 0 && line[i - offset] == '.') {
						line[i - offset] = 'O';
						line[i - offset + 1] = '.';
						offset++;
					}
				}
			}
		}
	}

	void tilt_into(SolutionInput_T buffer, int dir_x, int dir_y) {
		if (dir_x != 0) {

		}

		if (dir_y != 0) {
			if (dir_y < 0) tilt_north(buffer);
		}
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

		HashSet_T known_cycle_states{};

		tilt_north(solution_input);

		size_t line_weight = solution_input.size();
		for (const auto& line : solution_input) {
			for (const auto& c : line) {
				if (c == 'O') p1_result += line_weight;
			}
			line_weight--;
		}

		while (1) {
			if (known_cycle_states.contains(solution_input)) break;

			known_cycle_states[solution_input] = known_cycle_states.size();
			tilt_north(solution_input);
			tilt_west(solution_input);
			tilt_south(solution_input);
			tilt_east(solution_input);
		}

		unsigned repeating_pos = known_cycle_states.at(solution_input);
		unsigned cycle_length = known_cycle_states.size() - repeating_pos;

		unsigned remaining_iterations = (1000000000 - repeating_pos) % cycle_length;

		for (int i = 0; i < remaining_iterations; i++) {
			tilt_north(solution_input);
			tilt_west(solution_input);
			tilt_south(solution_input);
			tilt_east(solution_input);
		}

		line_weight = solution_input.size();
		for (const auto& line : solution_input) {
			for (const auto& c : line) {
				if (c == 'O') p2_result += line_weight;
			}
			line_weight--;
		}

		// 103333 - 97241
		return { p1_result, p2_result };
	}
};

#endif // !AOC_DAY_14
