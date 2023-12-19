#ifndef AOC_DAY_2
#define AOC_DAY_2

#include "solution.hpp"

static const unsigned int p1_limit_red = 12;
static const unsigned int p1_limit_green = 13;
static const unsigned int p1_limit_blue = 14;

class Day2 : public Solution<Day2> {
public:
	Day2() {
		day_num = 2;

		p1_expected = 2913;
		p2_expected = 55593;

		resolve_benchmark_tries = 10000;
	};

	using Result_T = std::pair<int, int>;

	inline bool p1_below_limit(const char& color, unsigned int num) {
		return color == 'r' ? num <= p1_limit_red 
			 : color == 'g' ? num <= p1_limit_green 
			 : color == 'b' ? num <= p1_limit_blue 
			 : false;
	}

	inline std::tuple<unsigned int, size_t> parse_game_id(const std::string& line) {
		size_t from = 5;
		size_t to = line.find(':', from);

		unsigned int result;
		auto [ptr, ec] = std::from_chars(line.data() + from, line.data() + to, result);

		return { result, to };
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		unsigned int p1_result = 0;
		unsigned int p2_result = 0;

		for (const auto& line : solution_input) {
			auto [game_id, starting_offset] = parse_game_id(line);
			//std::cout << "read game id <" << game_id << ">\n";
			bool valid_game = true;

			unsigned int max_red = 0;
			unsigned int max_green = 0;
			unsigned int max_blue = 0;

			for (size_t i = starting_offset + 1; i < line.size(); i++) {
				const char& c = line[i];

				bool reading_digit = isdigit(c);
				if (!reading_digit) continue;

				bool next_is_digit = isdigit(line[i + 1]);

				unsigned int num = 0;
				unsigned int offset = 2;

				if (next_is_digit) {
					offset += 1;
					num = 10 * (c - '0') + (line[i + 1] - '0');
				}
				else {
					num = c - '0';
				}

				i += offset; // step over whitespace and digit parts into color first letter

				const char& color = line[i];

				if (!p1_below_limit(color, num)) {
					valid_game = false;
				}

				switch (color) {
					case 'r':
						if (num > max_red) max_red = num;
						break;
					case 'g':
						if (num > max_green) max_green = num;
						break;
					case 'b':
						if (num > max_blue) max_blue = num;
						break;
				}

				i += 4; // step over color word (part of it, minimal for <red, >)
			}

			if (valid_game) p1_result += game_id;

			p2_result += (max_red * max_green * max_blue);
		}

		// 2913 - 55593
		return { p1_result, p2_result };
	}
};


#endif