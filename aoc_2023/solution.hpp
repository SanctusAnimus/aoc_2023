#ifndef SOLUTION_HPP
#define SOLUTION_HPP
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>
#include <chrono>
#include <format>
#include <ranges>
#include <algorithm>
#include <numeric>

#include "tuple_hash.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define COLORED(color, ...) color, __VA_ARGS__, RESET

namespace chrono = std::chrono;
namespace rv = std::ranges::views;

using SolutionInput_T = std::vector<std::string>&;
using Duration_T = chrono::duration<double, std::milli>;

enum AssertResult {
	OK, Fail, Unchecked
};

double variance_from(const std::vector<double>& samples, double mean, int size);

template<typename T>
class Solution {
public:
	struct DayResult {
		int day_num = -1;
		int resolve_benchmark_tries = 0;

		T::Result_T answer;

		double time_in_parsing = 0.0;
		double time_resolve_mean = 0.0;
		double time_resolve_total = 0.0;
		double time_resolve_stddev = 0.0;
		double time_resolve_variance = 0.0;

		AssertResult p1_check = Unchecked;
		AssertResult p2_check = Unchecked;

		friend std::ostream& operator<<(std::ostream& os, const DayResult& node) {
			std::string p1_color = node.p1_check == Fail ? RED : node.p1_check == OK ? GREEN : YELLOW;
			std::string p2_color = node.p2_check == Fail ? RED : node.p2_check == OK ? GREEN : YELLOW;

			std::string mean_color = node.time_resolve_mean <= 1.0 ? WHITE : node.time_resolve_mean <= 5.0 ? YELLOW : RED;

			os << std::format(
				"{}Day {:>2}{} ({}{:<6}{} tries) | {}{:>20}{} | {}{:>20}{} | {}{:>10.5f}{}ms | {:>10.5f}ms |\n",
				COLORED(GREEN, node.day_num), 
				COLORED(MAGENTA, node.resolve_benchmark_tries),
				COLORED(p1_color, node.answer.first), 
				COLORED(p2_color, node.answer.second), 
				// node.time_in_parsing, 
				COLORED(mean_color, node.time_resolve_mean),
				node.time_resolve_stddev
				// node.time_resolve_variance
			);

			return os;
		}
	};

	std::string get_file_path() const { return std::format("data_source/day_{}.txt", day_num); };

	std::vector<std::string> load_input() {
		auto parse_start = chrono::high_resolution_clock::now();

		const auto file_path = get_file_path();
		std::ifstream file(file_path);

		if (!file.is_open()) {
			std::cout << "file is not open!\n";
		}

		std::vector<std::string> parsed;

		std::string line;
		while (std::getline(file, line)) {
			parsed.push_back(line);
		}

		auto parse_end = chrono::high_resolution_clock::now();

		time_in_parsing = Duration_T(parse_end - parse_start).count();

		return parsed;
	};

	// benchmarking wrapper around solutions
	auto resolve(SolutionInput_T solution_input) {
		DayResult res = { day_num, resolve_benchmark_tries };

		std::vector<double> resolve_durations;
		resolve_durations.reserve(resolve_benchmark_tries);

		// this is just for benchmarking
		for (int i = 0; i < resolve_benchmark_tries; i++) {
			std::vector<std::string> t = solution_input;
			auto resolve_start = chrono::high_resolution_clock::now();
			static_cast<T*>(this)->_get_solutions(t);
			auto resolve_end = chrono::high_resolution_clock::now();

			Duration_T time_resolving = resolve_end - resolve_start;
			resolve_durations.push_back(time_resolving.count());
		}
		
		double time_took_total = std::accumulate(resolve_durations.begin(), resolve_durations.end(), 0.);
		double time_mean = time_took_total / resolve_benchmark_tries;
		double variance = variance_from(resolve_durations, time_mean, resolve_benchmark_tries);
		double stddev = sqrt(variance);

		res.answer = static_cast<T*>(this)->_get_solutions(solution_input);

		res.time_in_parsing = time_in_parsing;
		res.time_resolve_mean = time_mean;
		res.time_resolve_total = time_took_total;
		res.time_resolve_stddev = stddev;
		res.time_resolve_variance = variance;

		if (p1_expected != INT64_MIN) res.p1_check = (res.answer.first == p1_expected) ? OK : Fail;
		if (p2_expected != INT64_MIN) res.p2_check = (res.answer.second == p2_expected) ? OK : Fail;

		return res;
	};

	int day_num = -1;
	double time_in_parsing = -1.;
	int resolve_benchmark_tries = 100;

	intmax_t p1_expected = INT64_MIN;
	intmax_t p2_expected = INT64_MIN;
};

#endif // !SOLUTION_HPP