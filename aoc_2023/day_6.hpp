#ifndef AOC_DAY_6
#define AOC_DAY_6


#include "solution.hpp"
#include <array>


class Day6 : public Solution<Day6> {
public:
	Day6() {
		day_num = 6;
	};

	using Result_T = std::pair<int, int>;

	unsigned int parse_input_line_into(const std::string& line, std::array<unsigned int, 4>& target, long long int& p2_target) {
		unsigned int num_index = 0;
		int parsed_num = 0;

		size_t char_index = 9;
		size_t limit = line.size();

		do {
			const char& c = line[char_index++];

			if (isdigit(c)) {
				parsed_num = parsed_num * 10 + c - '0';
				p2_target = p2_target * 10 + c - '0';
 			}
			else if (parsed_num > 0) {
				target[num_index++] = parsed_num;
				parsed_num = 0;
			}
		} while (char_index <= limit);

		return num_index;
	}


	inline long long int get_win_count(long long int time, long long int min_distance) {
		// if we hold for H, then we travel at H * (time - H)
		// to find boundaries, solve this for min_distance, as H * (time - H) = min_distance, or -H2 + H*time - min_distance = 0, usual quadratic equasion

		// NOTE: Double is important here, cause we have to compare with ints for range boundaries, even though it's technically bad either 
		double d = sqrt(time * time + 4 * (-min_distance));
		double x1 = (-time + d) / -2.0;
		double x2 = (-time - d) / -2.0;

		long long int left_border = ceil(x1);
		long long int right_border = floor(x2);

		// still have to compensate for boundaries being resolved as integers, meaning that equasion resolved for exact min distance, while we need to go higher
		return right_border - left_border + 1 - (x1 == left_border) - (x2 == right_border);
	}


	Result_T _get_solutions(SolutionInput_T solution_input) {
		unsigned int p1_result = 1;
		unsigned int p2_result = 0;

		std::array<unsigned int, 4> time_per_race;
		std::array<unsigned int, 4> min_distance_per_race;

		long long int p2_time = 0;
		long long int p2_distance = 0;

		unsigned int times_total = parse_input_line_into(solution_input[0], time_per_race, p2_time);
		unsigned int distances_total = parse_input_line_into(solution_input[1], min_distance_per_race, p2_distance);

		for (int i = 0; i < times_total; i++) {
			//std::cout << "p1 " << i << " : " << time << " " << min_distance << "\n";
			p1_result *= get_win_count(time_per_race[i], min_distance_per_race[i]);
		}

		p2_result = get_win_count(p2_time, p2_distance);

		// 1731600 - 40087680
		return { p1_result, p2_result };
	}
};


#endif // !AOC_DAY_6
