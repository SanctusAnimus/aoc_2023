#ifndef AOC_DAY_15
#define AOC_DAY_15

#include "solution.hpp"


class Day15 : public Solution<Day15> {
public:
	Day15() {
		day_num = 15;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;


	inline unsigned hash(std::string_view& s) {
		unsigned current_hash_value = 0;
		for (const auto& c : s) {
			current_hash_value = ((current_hash_value + int(c)) * 17) % 256;
		}

		return current_hash_value;
	}


	Result_T _get_solutions(SolutionInput_T solution_input) {
		const std::string& line = solution_input[0];

		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

		unsigned current_hash_value = 0;
		for (const auto& c : line) {
			if (c == ',') {
				p1_result += current_hash_value;
				current_hash_value = 0;
			}
			else {
				current_hash_value = ((current_hash_value + int(c)) * 17) % 256;
			}
		}
		p1_result += current_hash_value;

		// 256 boxes, each containing array of pairs, each pair is a label and focal strength
		using BoxContent_T = std::vector<std::pair<std::string_view, int>>;
		std::vector<BoxContent_T> boxes;
		boxes.resize(256, {});

		current_hash_value = 0;
		int instruction_start = 0;
		int instruction_current = 0;
		for (const auto& c : line) {
			if (c == '-') {
				std::string_view label = std::string_view(line).substr(instruction_start, instruction_current - instruction_start);

				unsigned hashed = hash(label);

				BoxContent_T& content = boxes[hashed];

				std::erase_if(content, [&label](std::pair<std::string_view, int>& lens) {return lens.first == label; });
			}
			else if (c == '=') {
				std::string_view label = std::string_view(line).substr(instruction_start, instruction_current - instruction_start);
				unsigned focal_strength = line[instruction_current + 1] - '0';

				unsigned hashed = hash(label);

				BoxContent_T& content = boxes[hashed];

				if (auto it = std::find_if(content.begin(), content.end(), [&label](std::pair<std::string_view, int>& lens) {return lens.first == label; }); it != std::end(content)) {
					it->second = focal_strength;
				}
				else {
					content.emplace_back( label, focal_strength );
				}
			}
			else if (c == ',') {
				current_hash_value = 0;

				instruction_start = instruction_current + 1;
			}

			instruction_current++;
		}

		unsigned i = 0;
		for (auto& box : boxes) {
			if (!box.empty()) {
				unsigned lens_index = 1;
				for (auto& lens : box) {

					p2_result += (i + 1) * lens_index * lens.second;

					lens_index++;
				}
				//std::cout << "\n";
			}
			i++;
		}

		// 507769 - 269747
		return { p1_result, p2_result };
	}
};



#endif // !AOC_DAY_15
