#ifndef AOC_DAY_4
#define AOC_DAY_4

#include "solution.hpp"

#include <unordered_set>
#include <ranges>
#include <algorithm>

namespace rv = std::ranges::views;

class Day4 : public Solution {
public:
	Day4() {
		day_num = 4;
	};

private:
	inline bool is_digit(const char& c) {
		return c > 47 && c < 58;
	}

	SolutionReturn_T _get_solutions(SolutionInput_T solution_input) {
		unsigned int p1_result = 0;
		unsigned int p2_result = 0;

		size_t row_no = 0;
		size_t input_size = solution_input.size();

		std::vector<unsigned int> cards_counts;
		cards_counts.resize(input_size, 1);

		std::unordered_set<unsigned int> winning_numbers;

		for (const auto& line : solution_input) {
			bool done_reading_winning_side = false;
			unsigned int line_value = 0;

			size_t card_num_offset = 3;

			unsigned int line_matches = 0;
			for (size_t char_no = 6 + card_num_offset; char_no < line.size(); char_no++) {
				const char& c = line[char_no];

				if (c == '|') {
					done_reading_winning_side = true;
					char_no++;
					continue;
				}

				if (is_digit(c)) {
					unsigned int parsed_num = 0;
					do {
						parsed_num = parsed_num * 10 + (line[char_no++] - '0');
					} while (is_digit(line[char_no]));

					if (!done_reading_winning_side) {
						winning_numbers.insert(parsed_num);
					}
					else if (winning_numbers.contains(parsed_num)) {
						line_matches++;
						line_value = line_value == 0 ? 1 : line_value * 2;
					}
				}
			}

			if (line_matches > 0) {
				unsigned int current_card_count = cards_counts[row_no];

				for (unsigned int i = row_no + 1; i < std::min(row_no + line_matches + 1, input_size); i++) {
					cards_counts[i] += current_card_count;
				}
			}

			row_no++;
			p1_result += line_value;
			winning_numbers.clear();
		}

		p2_result = std::accumulate(cards_counts.begin(), cards_counts.end(), 0);

		// 21213 - 8549735
		return { std::to_string(p1_result), std::to_string(p2_result) };
	}
};

#endif