#ifndef AOC_DAY_9
#define AOC_DAY_9


#include "solution.hpp"

#include <array>
#include <cstdint>
#include <algorithm>
#include <execution>
#include <numeric>


class Day9 : public Solution<Day9> {
public:
	Day9() {
		day_num = 9;

		// 1757008019 - 995
		p1_expected = 1757008019;
		p2_expected = 995;

		resolve_benchmark_tries = 10000;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;
	using Row_T = std::array<intmax_t, 21>;

	inline void print_row(Row_T& row, int item_count) {
		for (int i = 0; i < item_count; i++) {
			std::cout << row[i] << " ";
		}
		std::cout << "\n";
	}

	int parse_line_into(const std::string& line, Row_T& row) {
		size_t size = line.size();
		int counter = 0;
		const char* parse_ptr = line.data();

		do {
			intmax_t parsed = 0;
			auto [ptr, ec] = std::from_chars(parse_ptr, parse_ptr + size, parsed);

			if (ec == std::errc()) {
				//std::cout << "parsed " << parsed << "\n";
				row[counter++] = parsed;
				parse_ptr = ptr + 1;
			}
			else {
				std::cout << "failed to parse at " << parse_ptr << "\n";
				parse_ptr++;
			}

		} while (parse_ptr - line.data() < size);

		return counter;
	}

	int p1_resolve_line(Row_T row, int item_count) {
		int starting_count = item_count;

		do {
			for (int i = 0; i < item_count - 1; i++) {
				row[i] = row[i + 1] - row[i];
			}
			item_count--;

			// if we're starting with 0, check that all values are 0 and break if true
			if (row[0] == 0) {
				bool finished = true;

				for (int j = 1; j < item_count; j++) {
					if (row[j] != 0) {
						finished = false;
						break;
					}
				}

				if (finished) break;
			}

		} while (1);

		intmax_t res = 0;
		for (int i = item_count; i < starting_count; i++) {
			res += row[i];
		}

		return res;
	}

	// same approach, just iterate backwards and extrapolate in backwards too
	int p2_resolve_line(Row_T row, int item_count) {
		int starting_count = item_count;
		int reserved = 0;

		do {
			for (int i = starting_count - 1; i > reserved; i--) {
				row[i] = row[i] - row[i - 1];
			}
			reserved++;

			// if we're starting with 0, check that all values are 0 and break if true
			if (row[starting_count - 1] == 0) {
				bool finished = true;

				for (int j = starting_count - 1; j > reserved; j--) {
					if (row[j] != 0) {
						finished = false;
						break;
					}
				}

				if (finished) break;
			}

		} while (1);

		intmax_t res = 0;
		for (int i = reserved; i >= 0; i--) {
			res = row[i] - res;
		}

		return res;
	}

	// slightly more fancy, but slower
	Result_T _get_solutions_old(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

		Row_T row{};

		for (const auto& line : solution_input) {
			int counter = parse_line_into(line, row);
			p1_result += p1_resolve_line(row, counter);
			p2_result += p2_resolve_line(row, counter);
		}

		// 1757008019 - 995
		return { p1_result, p2_result };
	}

	// direct and fast way
	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

		Row_T row{};

		for (const auto& line : solution_input) {
			int counter = parse_line_into(line, row);

			p1_result += row[counter - 1];
			p2_result += row[0];

			int multiplier = -1;

			do {
				for (int i = 0; i < counter - 1; i++) {
					row[i] = row[i + 1] - row[i];
				}
				counter--;

				int start = row[0];
				int end = row[counter - 1];

				if (start == 0 && end == 0) break;

				p1_result += end;
				p2_result += start * multiplier;

				multiplier *= -1;

			} while (1);
		}

		return { p1_result, p2_result };
	}
};


#endif // !AOC_DAY_9
