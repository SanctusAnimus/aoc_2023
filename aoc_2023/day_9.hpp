#ifndef AOC_DAY_9
#define AOC_DAY_9


#include "solution.hpp"

#include <array>
#include <cstdint>


class Day9 : public Solution<Day9> {
public:
	Day9() {
		day_num = 9;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;
	using Row_T = std::array<intmax_t, 21>;

	inline void print_row(Row_T& row, int item_count) {
		for (int i = 0; i < item_count; i++) {
			std::cout << row[i] << " ";
		}
		std::cout << "\n";
	}

	std::pair<Row_T, int> parse_line(const std::string& line) {
		size_t size = line.size();
		int counter = 0;
		const char* parse_ptr = line.data();

		Row_T row{};

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

		return { row, counter };
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

	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

		for (const auto& line : solution_input) {
			auto [row, counter] = parse_line(line);
			p1_result += p1_resolve_line(row, counter);
			p2_result += p2_resolve_line(row, counter);
		}

		// 1757008019 - 995
		return { p1_result, p2_result };
	}
};


#endif // !AOC_DAY_9
