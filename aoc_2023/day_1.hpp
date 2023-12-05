#ifndef AOC_DAY_1
#define AOC_DAY_1

#include "solution.hpp"
#include <array>

static const std::array<std::string, 9> words {
	"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};


class Day1 : public Solution<Day1> {
public:
	Day1() {
		day_num = 1;
	};

	using Result_T = std::pair<int, int>;

	inline bool is_digit(const char& c) {
		return c > 47 && c < 58;
	}

	inline bool is_starting_for_word(const char& c) {
		return c == 'o' || c == 't' || c == 'f' || c == 's' || c == 'e' || c == 'n';
	}

	inline std::tuple<size_t, size_t> array_section_size(const char& c) {
		// how many words for a given char, and at which index in array they start
		switch (c) {
			case 'o': return { 1, 0 };
			case 't': return { 2, 1 };
			case 'f': return { 2, 3 };
			case 's': return { 2, 5 };
			case 'e': return { 1, 7 };
			case 'n': return { 1, 8 };
			default:
				return { 0, 0 };
		}
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		unsigned int p1_value = 0;
		unsigned int p2_value = 0;

		for (const auto& line : solution_input) {
			unsigned int p1_first_digit = -1;
			unsigned int p1_last_digit = -1;

			unsigned int p2_first_digit = -1;
			unsigned int p2_last_digit = -1;

			for (int i = 0; i < line.size(); i++) {
				const char& c = line[i];

				if (is_digit(c)) {
					unsigned int parsed_digit = c - '0';

					if (p1_first_digit == -1) p1_first_digit = parsed_digit;
					if (p2_first_digit == -1) p2_first_digit = parsed_digit;

					p1_last_digit = parsed_digit;
					p2_last_digit = parsed_digit;

					continue;
				}

				size_t line_size = line.size();

				if (i <= line_size - 3 && is_starting_for_word(c)) {
					auto [section_size, index] = array_section_size(c);

					if (section_size == 1) {
						const std::string& word = words[index];
						size_t word_length = word.size();

						if ((i + word_length) <= line_size && std::string_view(line).substr(i, word_length) == word) {
							i += word_length - 2;

							if (p2_first_digit == -1) p2_first_digit = index + 1;
							p2_last_digit = index + 1;
						}
					}
					else {
						for (int word_index = index; word_index < index + section_size; word_index++) {
							const std::string& word = words[word_index];
							size_t word_length = word.size();

							if ((i + word_length) <= line_size && std::string_view(line).substr(i, word_length) == word) {
								i += word_length - 2;

								if (p2_first_digit == -1) p2_first_digit = word_index + 1;
								p2_last_digit = word_index + 1;
								break;
							}
						}
					}
				}
			}

			p1_value += (10 * p1_first_digit + p1_last_digit);
			p2_value += (10 * p2_first_digit + p2_last_digit);
		}

		// 54331 - 54518
		return { p1_value, p2_value };
	}
};


#endif // !AOC_DAY_1