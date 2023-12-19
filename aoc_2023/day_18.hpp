#ifndef AOC_DAY_18
#define AOC_DAY_18

#include "solution.hpp"


class Day18 : public Solution<Day18> {
public:
	Day18() {
		day_num = 18;

		// 50465 - 82712746433310
		p1_expected = 50465;
		p2_expected = 82712746433310;

		resolve_benchmark_tries = 10000;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;

	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

		intmax_t p1_x = 0;
		intmax_t p1_y = 0;

		intmax_t p2_x = 0;
		intmax_t p2_y = 0;

		intmax_t p1_wall_total = 0;
		intmax_t p2_wall_total = 0;
		long double p1_area = 0;
		intmax_t p2_area = 0;

		for (const auto& line : solution_input) {
			size_t p2_hash_start = 6;
			const char& direction = line[0];

			unsigned distance = line[2] - '0';
			if (isdigit(line[3])) {
				distance = distance * 10 + line[3] - '0';
				p2_hash_start += 1;
			}

			intmax_t p1_new_x = p1_x;
			intmax_t p1_new_y = p1_y;

			switch (direction) {
				case 'U':
				{
					p1_new_y -= distance;
					break;
				}
				case 'D':
				{
					p1_new_y += distance;
					break;
				}
				case 'L':
				{
					p1_new_x -= distance;
					break;
				}
				case 'R':
				{
					p1_new_x += distance;
					break;
				}
				default:
					std::cerr << "P1 Unknown direction <" << direction << ">\n";
			}

			p1_wall_total += distance;
			p1_area += p1_x * p1_new_y - p1_new_x * p1_y;
			p1_x = p1_new_x;
			p1_y = p1_new_y;


			const char& p2_direction = line[p2_hash_start + 5];
			intmax_t p2_distance = 0;
			auto [ptr, ec] = std::from_chars(line.data() + p2_hash_start, line.data() + p2_hash_start + 5, p2_distance, 16);

			if (ec != std::errc()) {
				std::cout << "failed to parse hex in " << line.substr(p2_hash_start, 5) << "\n";
			}

			intmax_t p2_new_x = p2_x;
			intmax_t p2_new_y = p2_y;

			switch (p2_direction) {
				case '3':
				{
					p2_new_y -= p2_distance;
					break;
				}
				case '1':
				{
					p2_new_y += p2_distance;
					break;
				}
				case '2':
				{
					p2_new_x -= p2_distance;
					break;
				}
				case '0':
				{
					p2_new_x += p2_distance;
					break;
				}
				default:
					std::cerr << "P2 Unknown direction <" << p2_direction << ">\n";
			}

			p2_wall_total += p2_distance;
			p2_area += p2_x * p2_new_y - p2_new_x * p2_y;
			p2_x = p2_new_x;
			p2_y = p2_new_y;
		}

		p1_result = abs(p1_area) / 2 + p1_wall_total / 2 + 1;
		p2_result = abs(p2_area) / 2 + p2_wall_total / 2 + 1;

		return { p1_result , p2_result };
	}
};

#endif // !AOC_DAY_18
