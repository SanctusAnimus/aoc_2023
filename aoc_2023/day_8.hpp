#ifndef AOC_DAY_8
#define AOC_DAY_8


#include "solution.hpp"

#include <array>
#include <unordered_map>
#include <numeric>
#include <cstdint>


constexpr auto lcm(intmax_t x, auto... xs) {
	return ((x = std::lcm(x, xs)), ...);
}


class Day8 : public Solution<Day8> {
public:
	Day8() {
		day_num = 8;
		instructions_count = 0;
	};

	using Result_T = std::pair<int, intmax_t>;

	unsigned instructions_count = 0;
	std::array<bool, 300> instructions_buffer;
	std::string_view first_node = "AAA";
	std::unordered_map<std::string_view, std::pair<std::string_view, std::string_view>> node_paths;

	unsigned traverse_until(std::string_view& starting_node_name, auto&& condition) {
		unsigned step = 0;
		unsigned index = 0;
		std::string_view node_name = starting_node_name;

		do {
			std::pair<std::string_view, std::string_view>& p = node_paths.at(node_name);
			bool instruction = instructions_buffer[index++];

			switch (instruction) {
				case 0:
					node_name = p.first;
					break;
				case 1:
					node_name = p.second;
			}

			step++;

			if (index >= instructions_count) {
				index = 0;
			}

			if (condition(node_name)) break;
		} while (1);

		return step;
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		unsigned p1_result = 0;
		intmax_t p2_result = 1;
		instructions_count = 0;

		// first line is the instruction set
		// skip 1, and all next lines fill the map

		//std::cout << "starting instruction buffer at " << instructions_count << "\n";
		for (const auto& c : solution_input[0]) {
			switch (c) {
				case 'L':
					instructions_buffer[instructions_count++] = false;
					break;
				case 'R':
					instructions_buffer[instructions_count++] = true;
					break;
			}
		}

		std::array<std::string_view, 10> p2_nodes;
		unsigned p2_node_id = 0;

		unsigned node_id = 0;
		size_t input_size = solution_input.size();

		// start from line 2 to skip instructions and whitespace
		for (unsigned current_line = 2; current_line < input_size; current_line++) {
			std::string_view l = solution_input[current_line];

			std::string_view node_name = l.substr(0, 3);
			std::string_view l_node = l.substr(7, 3);
			std::string_view r_node = l.substr(12, 3);

			node_paths[node_name] = { l_node, r_node };

			if (node_name[2] == 'A') p2_nodes[p2_node_id++] = node_name;
		}

		// p1
		p1_result = traverse_until(first_node, [](const std::string_view& node_name) { return node_name == "ZZZ"; });

		// p2
		for (int i = 0; i < p2_node_id; i++) {
			unsigned distance = traverse_until(p2_nodes[i], [](const std::string_view& node_name) {return node_name[2] == 'Z'; });

			p2_result = std::lcm(p2_result, distance);
		}

		//std::cout << lcm(14893, 20513, 22199, 19951, 17141, 12083) << "\n";

		// 20513 - 15995167053923
		node_paths.clear();
		return { p1_result, p2_result };
	}

};


#endif // !AOC_DAY_8
