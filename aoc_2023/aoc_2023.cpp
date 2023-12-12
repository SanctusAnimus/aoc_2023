#include <iostream>

#include "day_1.hpp"
#include "day_2.hpp"
#include "day_3.hpp"
#include "day_4.hpp"
#include "day_5.hpp"
#include "day_6.hpp"
#include "day_7.hpp"
#include "day_8.hpp"
#include "day_9.hpp"
#include "day_10.hpp"
#include "day_11.hpp"
#include "day_12.hpp"


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

    auto r_d01 = run_day(Day1());
    auto r_d02 = run_day(Day2());
    auto r_d03 = run_day(Day3());
    auto r_d04 = run_day(Day4());
    auto r_d05 = run_day(Day5());
    auto r_d06 = run_day(Day6());
    auto r_d07 = run_day(Day7());
    auto r_d08 = run_day(Day8());
    auto r_d09 = run_day(Day9());
    auto r_d10 = run_day(Day10());
    auto r_d11 = run_day(Day11());

    std::cout << r_d01;
    std::cout << r_d02;
    std::cout << r_d03;
    std::cout << r_d04;
    std::cout << r_d05;
    std::cout << r_d07;
    std::cout << r_d08;
    std::cout << r_d09;
    std::cout << r_d11;

    table_footer();
}

void run_current_day() {
    auto result = run_day(Day12());

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