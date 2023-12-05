#ifndef AOC_DAY_4
#define AOC_DAY_4

#include "solution.hpp"

#include <unordered_set>
#include <ranges>
#include <algorithm>

namespace rv = std::ranges::views;

class Day4 : public Solution<Day4> {
public:
	Day4() {
		day_num = 4;
	};

	using Result_T = std::pair<int, int>;

	Result_T _get_solutions(SolutionInput_T solution_input) {
		unsigned int p1_result = 0;
		unsigned int p2_result = 0;

		size_t row_no = 0;
		size_t input_size = solution_input.size();

		const unsigned int winner_sequence_count = 10;

		unsigned int* win_numbers = new unsigned int[winner_sequence_count];
		
		std::vector<unsigned int> cards_counts;
		cards_counts.resize(input_size, 1);

		size_t offset = solution_input[0].find(':') + 2;
		size_t line_size = solution_input[0].size();

		for (const auto& line : solution_input) {
			bool done_reading_winning_side = false;
			const char* parse_ptr = line.data() + offset;
			const char* end = line.data() + line_size;

			size_t winners_counter = 0;
			size_t guess_counter = 0;

			unsigned int match_counter = 0;

			do {
				unsigned int parsed = 0;

				if (*parse_ptr == '|') {
					done_reading_winning_side = true;
					parse_ptr += 2;
				}

				auto [ptr, ec] = std::from_chars(parse_ptr, parse_ptr + 2, parsed);

				if (ec == std::errc()) {
					if (!done_reading_winning_side) {
						win_numbers[winners_counter++] = parsed;
					}
					else {
						for (int i = 0; i < winner_sequence_count; i++) {
							if (win_numbers[i] == parsed) {
								match_counter++;
								break;
							}
						}
					}
				}

				parse_ptr = ptr + 1;

				if (parse_ptr - line.data() >= line_size) break;
			} while (1);

			unsigned int current_card_count = cards_counts[row_no];
			for (int i = row_no + 1; i <= row_no + match_counter; i++) {
				cards_counts[i] += current_card_count;
			}

			p1_result += match_counter == 0 ? 0 : 0x01 << (match_counter - 1);

			row_no++;
		}

		p2_result = std::accumulate(cards_counts.begin(), cards_counts.end(), 0);

		delete[] win_numbers;
		// 21213 - 8549735
		return { p1_result, p2_result };
	}
};

#endif