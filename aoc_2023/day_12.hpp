#ifndef AOC_DAY_12
#define AOC_DAY_12

#include <cstdint>
#include "solution.hpp"

#include <tuple>
#include <map>
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& groups) {
    for (const auto& v : groups) {
        os << v << " ";
    }

    return os;
}

class Day12 : public Solution<Day12> {
public:
	Day12() {
		day_num = 12;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;

    using Record_T = std::tuple<intmax_t, intmax_t, intmax_t>;
    using Cache_T = std::unordered_map<Record_T, intmax_t>;

    intmax_t combinations(const std::string_view& springs, const std::vector<int>& groups, intmax_t i, intmax_t j, intmax_t r, Cache_T& cache) {
        // splitting recursion, intermediate results are cached in unordered map
        Record_T t = { i, j, r };
        if (auto search = cache.find(t); search != cache.end()) {
            return search->second;
        }

        if (j == groups.size()) {
            intmax_t result = intmax_t(std::count(springs.begin() + i, springs.end(), '#') == 0);
            cache.emplace(t, result);
            return result;
        }

        if (i == springs.size()) {
            intmax_t result = intmax_t(j == (groups.size() - 1) && r == groups[j]);
            cache.emplace(t, result);
            return result;
        }

        intmax_t n = 0;

        if (springs[i] != '.') {
            n += combinations(springs, groups, i + 1, j, r + 1, cache);
        }
        if (springs[i] != '#') {
            if (r == groups[j]) {
                n += combinations(springs, groups, i + 1, j + 1, 0, cache);
            }
            else if (r == 0) {
                n += combinations(springs, groups, i + 1, j, 0, cache);
            }
        }

        cache.emplace(t, n);

        return n;
    }

    inline std::string repeat_times(std::string_view& s, unsigned times) {
        std::ostringstream os;
        os << s;
        for (int i = 0; i < times - 1; i++)
            os << "?" << s;
        return os.str();
    }

	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 0;
		intmax_t p2_result = 0;

        size_t line_no = 0;
        for (const auto& line : solution_input) {
            size_t whitespace_at = line.find(' ');
            std::string_view springs = std::string_view(line).substr(0, whitespace_at);

            std::vector<int> groups;

            size_t char_no = whitespace_at + 1;
            unsigned parsed = 0;
            do {
                const char& c = line[char_no++];
                if (isdigit(c)) {
                    parsed = parsed * 10 + c - '0';
                }
                else {
                    groups.push_back(parsed);
                    parsed = 0;
                }

            } while (char_no < line.size());

            if (parsed != 0) groups.push_back(parsed);

            Cache_T cache;
            p1_result += combinations(springs, groups, 0, 0, 0, cache);

            std::string p2_springs = repeat_times(springs, 5);
            
            // mutate group in-place
            size_t groups_size = groups.size();

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < groups_size; j++) {
                    groups.push_back(groups[j]);
                }
            }

            Cache_T p2_cache;
            intmax_t res = combinations(p2_springs, groups, 0, 0, 0, p2_cache);
            p2_result += res;
        }

        // 7236 - 11607695322318
		return { p1_result, p2_result };
	}
};

#endif