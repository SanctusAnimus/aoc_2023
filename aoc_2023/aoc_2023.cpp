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
#include "day_13.hpp"
#include "day_14.hpp"
#include "day_15.hpp"
#include "day_16.hpp"
#include "day_17.hpp"
#include "day_18.hpp"
#include "day_19.hpp"


auto run_day(auto&& day_solution) {
    std::vector<std::string> input_data = day_solution.load_input();

    return day_solution.resolve(input_data);
}

void table_header() {
    std::cout << std::format(
        "                      | {}{:^20}{} | {}{:^20}{} | {}{:^12}{} | {}{:^12}{} |\n", 
        COLORED(BOLDCYAN, "P1"),
        COLORED(BOLDCYAN, "P2"), 
        // COLORED(BOLDCYAN, "Parse"), 
        COLORED(BOLDCYAN, "Mean"),
        COLORED(BOLDCYAN, "StdDev")
    );
}

void table_separator() {
    std::cout << std::string(22, '-') << "|";
    std::cout << std::string(22, '-') << "|";
    std::cout << std::string(22, '-') << "|";
    std::cout << std::string(14, '-') << "|";
    std::cout << std::string(14, '-') << "|" << '\n';
}

void run_all() {
    table_header();
    table_separator();

    std::cout << run_day(Day1());
    std::cout << run_day(Day2());
    std::cout << run_day(Day3());
    std::cout << run_day(Day4());
    std::cout << run_day(Day5());
    std::cout << run_day(Day6());
    std::cout << run_day(Day7());
    std::cout << run_day(Day8());
    std::cout << run_day(Day9());
    std::cout << run_day(Day10());
    std::cout << run_day(Day11());
    std::cout << run_day(Day12());
    std::cout << run_day(Day13());
    std::cout << run_day(Day14());
    std::cout << run_day(Day15());
    std::cout << run_day(Day16());
    std::cout << run_day(Day17());
    std::cout << run_day(Day18());
    std::cout << run_day(Day19());

    table_separator();
}

void run_current_day() {
    auto result = run_day(Day19());

    table_header();
    table_separator();
    std::cout << result;
    table_separator();
}


int main()
{
    // run_current_day();
    run_all();
    return 0;
}