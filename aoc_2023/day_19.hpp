#ifndef AOC_DAY_19
#define AOC_DAY_19

#include "solution.hpp"

#include <unordered_map>

struct Condition_D19 {
	int index;
	int value;

	bool greater; // < or >
	bool final; // whether this is the last condition of the ruleset

	std::string_view target; // label, A or R

	friend std::ostream& operator<<(std::ostream& os, const Condition_D19& lhs) {
		if (lhs.final) {
			os << "[final] " << lhs.target;
		}
		else {
			os << lhs.index << (lhs.greater ? " > " : " < ") << lhs.value << " : " << lhs.target;
		}

		return os;
	}
};

std::ostream& operator<<(std::ostream& os, std::array<intmax_t, 4>& lhs) {
	os << "{" << "x=" << lhs[0] << ", " << "m=" << lhs[1] << ", " << "a=" << lhs[2] << ", " << "s=" << lhs[3] << "}";

	return os;
}

std::ostream& operator<<(std::ostream& os, std::array<std::pair<int, int>, 4>& lhs) {
	os << "{";

	for (const auto& [first, second] : lhs) {
		os << "(" << first << ", " << second << "), ";
	}

	os << "}";

	return os;
}

class Day19 : public Solution<Day19> {
public:
	Day19() {
		day_num = 19;

		p1_expected = 333263;
		p2_expected = 130745440937650;

		resolve_benchmark_tries = 10000;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;
	using Ruleset_T = std::unordered_map<std::string_view, std::vector<Condition_D19>>;
	using Workflow_T = std::array<intmax_t, 4>;
	using Range_T = std::array<std::pair<int, int>, 4>;

	bool test_workflow(Workflow_T&& workflow, Ruleset_T& ruleset) {
		static std::string start = "in";

		std::string_view start_view = std::string_view(start);
		std::string_view& label = start_view;

		do {
			if (label == "A") return true;
			if (label == "R") return false;

			const auto& conditions = ruleset.at(label);
			for (const auto& cond : conditions) {
				if (cond.final) {
					if (cond.target == "A") return true;
					else if (cond.target == "R") return false;
					else {
						label = cond.target;
						break;
					}
				}
				intmax_t w_value = workflow[cond.index];

				if (cond.greater) {
					if (w_value > cond.value) {
						label = cond.target;
						break;
					}
				}
				else if (w_value < cond.value) {
					label = cond.target;
					break;
				}
			}
		} while (1);

		return false;
	}

	bool is_invalid_range(Range_T& ranges) {
		for (const auto& range : ranges) {
			if (range.first > range.second) return true;
		}

		return false;
	}

	intmax_t process_workflow_p2(Range_T& ranges, Ruleset_T& ruleset, std::string_view label) {
		if (label == "R") {
			return 0;
		}
		else if (label == "A") {
			intmax_t combinations = 1;

			for (const auto& range : ranges) {
				combinations *= (intmax_t(range.second) - range.first);
			}

			return combinations;
		}

		const auto& conditions_vec = ruleset.at(label);
		intmax_t total = 0;

		for (const auto& cond : conditions_vec) {
			if (cond.final) {
				total += process_workflow_p2(ranges, ruleset, cond.target);
			}
			else {
				auto& [low, high] = ranges[cond.index];

				if (!cond.greater) {
					if (low < cond.value) {
						Range_T new_ranges = ranges;
						new_ranges[cond.index] = { low, cond.value };
						total += process_workflow_p2(new_ranges, ruleset, cond.target);
					}

					if (high <= cond.value) {
						break;
					}

					ranges[cond.index] = { cond.value, high };
				}
				else {
					int target_value = cond.value + 1;

					if (target_value < high) {
						Range_T new_ranges = ranges;
						new_ranges[cond.index] = { target_value, high };
						total += process_workflow_p2(new_ranges, ruleset, cond.target);
					}

					if (target_value <= low) {
						break;
					}

					ranges[cond.index] = { low, target_value };
				}
			}
		}

		return total;
	}

	inline int index_from_cond_c(const char& c) {
		switch (c) {
			case 'x': return 0;
			case 'm': return 1;
			case 'a': return 2;
			case 's': return 3;
		}
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

		bool reading_rules = true;

		Ruleset_T ruleset{};

		const char _default = '0';

		for (const auto& line : solution_input) {
			if (line.empty()) {
				reading_rules = false;

				continue;
			}

			if (reading_rules) {
				std::string_view line_view = std::string_view(line);
				size_t ruleset_open_at = line_view.find('{');

				std::string_view label = line_view.substr(0, ruleset_open_at);

				std::vector<Condition_D19> conditions{};

				size_t char_no = ruleset_open_at + 1;
				do {
					const char& cond_c = line_view[char_no++];
					const char& comp_c = line_view[char_no++];

					if (comp_c == '}') {
						conditions.emplace_back(_default, 0, false, true, line_view[char_no - 2] == 'A' ? std::string_view("A") : std::string_view("R"));
						break;
					}

					if (comp_c != '<' && comp_c != '>') {
						size_t final_label_end = line_view.find('}', char_no);
						std::string_view final_label = line_view.substr(char_no - 2, final_label_end - (char_no - 2));

						conditions.emplace_back(_default, 0, false, true, final_label);

						break;
					}

					unsigned value = 0;
					auto [ptr, ec] = std::from_chars(line.data() + char_no, line.data() + line.size(), value);

					if (ec != std::errc()) {
						std::cerr << "failed to parse value from line at " << char_no << "\n";
					}

					char_no = ptr - line.data() + 1; // we should be after : here

					size_t target_end = line_view.find(',', char_no);
					std::string_view target_label = line_view.substr(char_no, target_end - char_no);

					char_no = target_end + 1;

					conditions.emplace_back(
						index_from_cond_c(cond_c),
						value,
						comp_c == '>' ? true : false,
						false, 
						std::move(target_label)
					);
				} while (char_no < line.size());

				ruleset[label] = std::move(conditions);
			}
			else {
				size_t char_no = 3; // start from first char after {x=

				auto parse_next = [&char_no, &line]() {
					intmax_t value = 0;
					auto [ptr, ec] = std::from_chars(line.data() + char_no, line.data() + line.size(), value);

					if (ec != std::errc()) {
						std::cout << "failed to parse workflow num\n";
					}

					char_no = ptr - line.data() + 3;

					return value;
				};

				intmax_t x = parse_next();
				intmax_t m = parse_next();
				intmax_t a = parse_next();
				intmax_t s = parse_next();

				if (test_workflow({x, m, a, s}, ruleset)) {
					p1_result += (x + m + a + s);
				}
			}
		}

		// range checks are inclusive
		Range_T starting_ranges{
			{ {1, 4001}, {1, 4001}, {1, 4001}, {1, 4001} }
		};

		p2_result = process_workflow_p2(starting_ranges, ruleset, "in");

		// 333263 - 130745440937650
		return {p1_result, p2_result};
	}
};


#endif