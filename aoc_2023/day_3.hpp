#ifndef AOC_DAY_3
#define AOC_DAY_3

#include "solution.hpp"

#include <set>
#include <ranges>
#include <algorithm>

namespace rv = std::ranges::views;

class Day3 : public Solution<Day3> {
public:
	Day3() {
		day_num = 3;
	};

	using Result_T = std::pair<int, int>;

	inline std::tuple<unsigned int, size_t> next_number(std::string_view line, size_t start, size_t size) {
		size_t end = line.find_first_not_of("0123456789", start);

		if (end == std::string::npos) end = size;

		std::string_view num_as_string = line.substr(start, end - start);
		
		unsigned int parsed = 0;
		auto [ptr, ec] = std::from_chars(num_as_string.data(), num_as_string.data() + num_as_string.size(), parsed);

		return { parsed , end - start };
	}

	struct NumberRecord {
		unsigned int number;
		int start_x;
		int end_x;
	};

	struct SymbolRecord {
		const char& symbol;
		int x;
	};

	inline bool p1_symbol_touches_number_from_row(const std::vector<SymbolRecord>& sr_v, const NumberRecord& num) {
		for (const auto& sr : sr_v) {
			if (sr.x >= num.start_x - 1 && sr.x <= num.end_x + 1) {
				return true;
			}
		}

		return false;
	}

	void p2_calculate_row_for_gear(const std::vector<NumberRecord>& nr_v, const SymbolRecord& symbol, unsigned int& connections, unsigned int& product) {
		for (const auto& nr : nr_v) {
			if (
				nr.end_x >= (symbol.x - 1) && nr.start_x <= (symbol.x - 1) || (nr.start_x <= (symbol.x + 1) && nr.end_x >= symbol.x)
			)
			{
				//std::cout << "valid connection!\n";
				connections++;
				product *= nr.number;
			}
		}
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		unsigned int p1_result = 0;
		unsigned int p2_result = 0;

		size_t input_length = solution_input.size();

		std::vector<std::vector<NumberRecord>> numbers;
		std::vector<std::vector<SymbolRecord>> symbols;

		numbers.resize(input_length);
		symbols.resize(input_length);

		// input parsing into structs
		size_t line_no = 0;
		for (const auto& line : solution_input) {
			size_t offset = 0;
			size_t line_size = line.size();
			size_t char_no = line.find_first_not_of('.');

			numbers.push_back({});
			symbols.push_back({});

			while (char_no < line_size) {
				const char& c = line[char_no];
				if (c == '\n') continue;

				if (isdigit(c)) {
					const auto [parsed_num, num_size] = next_number(line, char_no, line_size);
					numbers[line_no].emplace_back(parsed_num, char_no, char_no + num_size - 1);

					char_no += num_size;
					char_no = line.find_first_not_of('.', char_no);
					if (char_no == std::string::npos) break;
				}
				else if (c == '.') {
					char_no = line.find_first_not_of('.', char_no + 1);
				}
				else {
					symbols[line_no].emplace_back(c, char_no);
					char_no = line.find_first_not_of('.', char_no + 1);
				}
			}

			line_no++;
		}

		auto is_gear = [](const SymbolRecord& sr) { return sr.symbol == '*'; };

		for (size_t m_line_no = 0; m_line_no < input_length; m_line_no++) {
			const auto& numbers_records = numbers[m_line_no];
			const auto& symbol_records = symbols[m_line_no];

			// p1
			for (const auto& num_record : numbers_records) {
				bool has_connections = false;

				if (m_line_no > 0) {
					if (p1_symbol_touches_number_from_row(symbols[m_line_no - 1], num_record)) has_connections = true;
				}

				if (p1_symbol_touches_number_from_row(symbols[m_line_no], num_record)) has_connections = true;

				if (m_line_no < input_length - 1) {
					if (p1_symbol_touches_number_from_row(symbols[m_line_no + 1], num_record)) has_connections = true;
				}

				if (has_connections) {
					p1_result += num_record.number;
				}
			}

			// p2
			for (const auto& gear : symbol_records | std::views::filter(is_gear)) {
				unsigned int gear_connections = 0;
				unsigned int gear_product = 1;

				if (m_line_no > 0) {
					p2_calculate_row_for_gear(numbers[m_line_no - 1], gear, gear_connections, gear_product);
				}

				p2_calculate_row_for_gear(numbers_records, gear, gear_connections, gear_product);

				if (m_line_no < input_length - 1) {
					p2_calculate_row_for_gear(numbers[m_line_no + 1], gear, gear_connections, gear_product);
				}

				if (gear_connections == 2) {
					p2_result += gear_product;
				}
			}
		}

		// 527144 - 81463996
		return { p1_result, p2_result };
	}
};


#endif