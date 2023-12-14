#ifndef AOC_DAY_13
#define AOC_DAY_13

#include <cstdint>
#include "solution.hpp"


class Day13 : public Solution<Day13> {
public:
	Day13() {
		day_num = 13;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;

	using StringBuffer_T = std::array<std::string_view, 30>;

	// compare chars at indices for every line in a buffer, effectively comparing columns
	bool columns_equal(StringBuffer_T& buffer, unsigned buffer_len, size_t column_no_1, size_t column_no_2) {
		for (size_t i = 0; i < buffer_len; i++) {
			auto& line = buffer[i];

			if (line[column_no_1] != line[column_no_2]) return false;
		}

		return true;
	}

	unsigned int line_differences(std::string_view& l1, std::string_view& l2) {
		unsigned differences = 0;

		for (int i = 0; i < l1.size(); i++) {
			if (l1[i] != l2[i]) differences++;
		}

		return differences;
	}

	unsigned int column_differences(StringBuffer_T& buffer, unsigned buffer_len, size_t column_no_1, size_t column_no_2) {
		unsigned differences = 0;

		for (size_t i = 0; i < buffer_len; i++) {
			auto& line = buffer[i];

			if (line[column_no_1] != line[column_no_2]) differences++;
		}

		return differences;
	}

	intmax_t process_buffer(StringBuffer_T& buffer, unsigned buffer_len, int buffer_id) {
		// find perfect reflection either in horizontal or in vertical division
		// exactly one per buffer
		size_t line_len = buffer[0].size();

		// horizontal
		for (int i = 0; i < buffer_len - 1; i++) {
			auto& line = buffer[i];
			auto& next = buffer[i + 1];

			if (line == next) {
				// lines match, potential reflection
				// backwards scan to confirm it, until we reach lower or upper boundary of buffer
				// or inequality found

				int offset = 1;
				bool confirmed = true;
				while (i - offset >= 0 && i + offset + 1 < buffer_len) {
					auto& p_line = buffer[i - offset];
					auto& p_next = buffer[i + 1 + offset];

					if (p_line != p_next) {
						confirmed = false;
						break;
					}
					offset++;
				}

				if (confirmed) return 100 * (i + 1);
			}
		}

		for (int i = 0; i < line_len - 1; i++) {
			if (columns_equal(buffer, buffer_len, i, i + 1)) {
				// columns match, potential reflection
				// backwards scan to confirm it, until we reach left or right boundary of buffer
				// or inequality found

				int offset = 1;
				bool confirmed = true;
				while (i - offset >= 0 && i + offset + 1 < line_len) {
					if (!columns_equal(buffer, buffer_len, i - offset, i + 1 + offset)) {
						confirmed = false;
						break;
					}
					offset++;
				}

				if (confirmed) return i + 1;
			}
		}
		return 0;
	}

	// p1 exited as soon as any ineqality found, this counts differences in between slices
	// doing so for p1 would be less optimal (though much less code)
	intmax_t process_buffer_p2(StringBuffer_T& buffer, unsigned buffer_len, int buffer_id, unsigned threshold) {
		for (unsigned int i = 0; i < buffer_len; i++) {
			unsigned int offset = std::min(i + 1, buffer_len - i - 1);

			unsigned slice_differences = 0;

			for (int j = 0; j < offset; j++) {
				slice_differences += line_differences(buffer[i - j], buffer[i + j + 1]);
	
				if (slice_differences > threshold) break;
			}

			if (slice_differences == threshold) return 100 * (i + 1);
		}

		unsigned line_len = buffer[0].size();

		for (unsigned int i = 0; i < line_len; i++) {
			unsigned int offset = std::min(i + 1, line_len - i - 1);

			unsigned slice_differences = 0;

			for (int j = 0; j < offset; j++) {
				slice_differences += column_differences(buffer, buffer_len, i - j, i + j + 1);
				if (slice_differences > threshold) break;
			}
			if (slice_differences == threshold) return i + 1;
		}

		return 0;
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

		StringBuffer_T buffer;
		unsigned buffer_len = 0;

		unsigned buffer_id = 0;
		for (const auto& line : solution_input) {
			if (line.empty()) {
				p1_result += process_buffer(buffer, buffer_len, buffer_id++);
				buffer_len = 0;
			}
			else {
				buffer[buffer_len++] = std::string_view(line);
			}
		}
		p1_result += process_buffer(buffer, buffer_len, buffer_id++);

		buffer_id = 0;
		buffer_len = 0;

		for (const auto& line : solution_input) {
			if (line.empty()) {
				p2_result += process_buffer_p2(buffer, buffer_len, buffer_id++, 1);
				buffer_len = 0;
			}
			else {
				buffer[buffer_len++] = std::string_view(line);
			}
		}
		p2_result += process_buffer_p2(buffer, buffer_len, buffer_id++, 1);

		// 29130 - 33438
		return { p1_result, p2_result };
	}
};


#endif // !AOC_DAY_13
