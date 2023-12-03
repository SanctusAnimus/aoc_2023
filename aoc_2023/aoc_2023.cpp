#include <iostream>

#include "day_1.hpp"
#include "day_2.hpp"
#include "day_3.hpp"


DayResult run_day(Solution&& day_solution) {
    std::vector<std::string> input_data = day_solution.load_input();

    return day_solution.resolve(input_data);
}

void table_header() {
    std::cout << std::format(
        "       | {}{:^20}{} | {}{:^20}{} | {}{:^12}{} | {}{:^12}{} | {}{:^12}{} | {}{:^12}{} |\n", 
        COLORED(BOLDCYAN, "P1"),
        COLORED(BOLDCYAN, "P2"), 
        COLORED(BOLDCYAN, "Parse"), 
        COLORED(BOLDCYAN, "Mean"),
        COLORED(BOLDCYAN, "StdDev"), 
        COLORED(BOLDCYAN, "Variance")
    );
}

void table_footer() {
    std::cout << std::string(129, '-') << '\n';
    std::cout << "Tries per day: " << BOLDMAGENTA << Solution::resolve_benchmark_tries << RESET << "\n";
}

void run_all() {
    std::vector<DayResult> results = {
        run_day(Day1()),
        run_day(Day2()),
        run_day(Day3()),
    };

    table_header();

    for (auto& result : results) {
        std::cout << result;
    }

    table_footer();
}

void run_current_day() {
    auto result = run_day(Day3());

    table_header();
    std::cout << result;
    table_footer();
}


int main()
{
    run_current_day();
    return 0;
}