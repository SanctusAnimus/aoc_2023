#ifndef AOC_DAY_5
#define AOC_DAY_5

#include "solution.hpp"
#include <compare>
#include <array>

struct MapRule {
	long long int range_start;
	long long int range_end;
	long long int range_shift;

	friend std::ostream& operator<<(std::ostream& os, const MapRule& record) {
		os << "MapRule <" << record.range_start << " : " << record.range_end << " : " << record.range_shift << ">\n";
		return os;
	}

	auto operator<=>(const MapRule& rhs) const {
		return range_start <=> rhs.range_start;
	};

	bool operator==(const MapRule& rhs) const {
		return range_start == rhs.range_start && range_end == rhs.range_end && range_shift == rhs.range_shift;
	}
};

class Day5 : public Solution<Day5> {
public:
	Day5() {
		day_num = 5;

		p1_expected = 525792406;
		p2_expected = 79004094;

		resolve_benchmark_tries = 10000;
	};

	using Result_T = std::pair<long long int, long long int>;

	void parse_seeds(const std::string& line, std::vector<long long int>& target, size_t offset = 0) {
		size_t line_size = line.size();
		const char* parse_ptr = line.data() + offset;

		do {
			long long int parsed = 0;
			auto [ptr, ec] = std::from_chars(parse_ptr, parse_ptr + line_size, parsed);

			if (ec == std::errc()) {
				target.push_back(parsed);
			}

			parse_ptr = ptr + 1;

		} while (parse_ptr - line.data() < line_size);
	}

	MapRule parse_map_line(const std::string& line) {
		size_t line_size = line.size();
		const char* parse_ptr = line.data();

		long long int dest_range_start = 0;
		long long int source_range_start = 0;
		long long int range_size = 0;

		auto [ptr_into_source, ec_1] = std::from_chars(parse_ptr, parse_ptr + line_size, dest_range_start);
		auto [ptr_into_range, ec_2] = std::from_chars(ptr_into_source + 1, ptr_into_source + line_size, source_range_start);
		auto [ptr, ec] = std::from_chars(ptr_into_range + 1, ptr_into_range + line_size, range_size);

		return {
			source_range_start,
			source_range_start + range_size,
			dest_range_start - source_range_start
		};
	}

	void p2_remap_seed_range(Result_T& seed_range, const std::array<MapRule, 60>& map_rules, size_t rules_size, std::vector<Result_T>& target) {
		bool any_new_ranges = false;

		auto& [seed_start, seed_len] = seed_range;

		for (int i = 0; i < rules_size; i++) {
			const auto& rule = map_rules[i];

			// if this seed stars within a rule range, transform seed range
			if (seed_start >= rule.range_start && seed_start < rule.range_end) {
				long long int result_start = seed_start + rule.range_shift;
				
				// if seed range fits entirely, place entire transformed in target
				if (rule.range_end >= seed_start + seed_len) {
					target.emplace_back(result_start, seed_len);
				}
				else {
					// otherwise split range in two, modifying original and putting cut part into target
					long long int new_seed_len = seed_start + seed_len - rule.range_shift;
					target.emplace_back(result_start, seed_len - new_seed_len);
					seed_len = new_seed_len;
					seed_start = rule.range_end;
				}
				any_new_ranges = true;
			}
		}

		// preserve original range if none of the rules made any modifications
		if (!any_new_ranges) {
			target.emplace_back(seed_start, seed_len);
		}
	}


	Result_T _get_solutions(const SolutionInput_T solution_input) {
		std::vector<long long int> seeds;
		seeds.reserve(20);
		
		parse_seeds(solution_input[0], seeds, 7);

		// transformed seed ranges per starting seed range
		std::vector<std::vector<Result_T>> p2_seed_ranges;
		p2_seed_ranges.resize(10, {});

		for (size_t i = 0; i < seeds.size(); i += 2) {
			p2_seed_ranges[i / 2].emplace_back(seeds[i], seeds[i + 1]);
		}

		// fixed-size rules buffer, that every conversion iteration override
		std::array<MapRule, 60> map_rules;

		const size_t input_size = solution_input.size();
		size_t current_line = 3; // skip 3 lines into start of seed-to-soil
		
		// 7 conversions in total (hardcoding this is kinda silly but what gives)
		for (int i = 0; i < 7; i++) {
			size_t current_map_size = 0;
			do {
				const std::string& line = solution_input[current_line++];
				if (line.empty()) break;

				map_rules[current_map_size++] = parse_map_line(line);

				if (current_line >= input_size) break;
			} while (1);

			std::sort(map_rules.begin(), map_rules.begin() + current_map_size);

			// p1
			for (int seed_idx = 0; seed_idx < seeds.size(); seed_idx++) {
				long long int current_seed_value = seeds[seed_idx];

				for (int record_idx = 0; record_idx < current_map_size; record_idx++) {
					const auto& record = map_rules[record_idx];

					if (current_seed_value >= record.range_start && current_seed_value < record.range_end) {
						seeds[seed_idx] = current_seed_value + record.range_shift;
					}
				}
			}

			// p2
			for (auto& seed_ranges : p2_seed_ranges) {
				std::vector<Result_T> new_ranges;

				for (auto& seed_range : seed_ranges) {
					p2_remap_seed_range(seed_range, map_rules, current_map_size, new_ranges);
				}

				seed_ranges.swap(new_ranges);
			}

			current_line++;
		}

		// p2 is the min start across all sub-ranges of all seed ranges
		long long int p2_result = INT64_MAX;

		for (const auto& ranges : p2_seed_ranges) {
			for (const auto& range : ranges) {
				if (range.first < p2_result) p2_result = range.first;
			}
		}

		long long int p1_result = *std::min_element(seeds.begin(), seeds.end());

		// 525792406 - 79004094
		return { p1_result , p2_result };
	}
};

#endif