#include <iostream>

#include "day_1.hpp"
#include "day_2.hpp"
#include "day_3.hpp"
#include "day_4.hpp"
#include "day_5.hpp"
#include "day_6.hpp"


auto run_day(auto&& day_solution) {
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
    std::cout << std::string(109, '-') << '\n';
    std::cout << "Tries per day: " << BOLDMAGENTA << 10000 << RESET << "\n";
}

void run_all() {
    table_header();

    auto r_d1 = run_day(Day1());
    auto r_d2 = run_day(Day2());
    auto r_d3 = run_day(Day3());
    auto r_d4 = run_day(Day4());
    auto r_d5 = run_day(Day5());
    auto r_d6 = run_day(Day6());

    std::cout << r_d1;
    std::cout << r_d2;
    std::cout << r_d3;
    std::cout << r_d4;
    std::cout << r_d5;
    std::cout << r_d6;

    table_footer();
}

void run_current_day() {
    auto result = run_day(Day6());

    table_header();
    std::cout << result;
    table_footer();
}


int main()
{
    run_current_day();
    // run_all();
    return 0;
}