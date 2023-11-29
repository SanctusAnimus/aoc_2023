#ifndef AOC_DAY_1
#define AOC_DAY_1

#include "solution.hpp"
#include <array>


class Day1 : public Solution {
public:
	Day1() {
		day_num = 1;
	};

private:
	SolutionReturn_T _get_solutions(std::vector<std::string>& solution_input) {
		//std::cout << "day 1 getting solutions\n";

		std::vector<int> elf_values;
		int current_elf_value = 0;

		//std::cout << solution_input.size();

		for (const auto& m_value : solution_input) {
			//std::cout << "reading " << m_value << "\n";
			if (m_value.empty() || m_value == "\n") {
				//std::cout << "empty line encountered\n";
				elf_values.push_back(current_elf_value);

				current_elf_value = 0;
			}

			int parsed_value = 0;
			auto [ptr, ec] = std::from_chars(m_value.data(), m_value.data() + m_value.size(), parsed_value);
			current_elf_value += parsed_value;

			//std::cout << parsed_value << "\n";
		}

		if (elf_values.size() == 0) {
			return std::make_tuple("1", "2");
		}

		int max_value = *std::max_element(elf_values.begin(), elf_values.end());
		std::ranges::sort(elf_values, std::greater<>());
		int sum_values = elf_values[0] + elf_values[1] + elf_values[2];

		return std::make_tuple(std::to_string(max_value), std::to_string(sum_values));

		//return std::make_tuple(std::to_string(m / 23.0), std::to_string(m));
	}
};


#endif // !AOC_DAY_1