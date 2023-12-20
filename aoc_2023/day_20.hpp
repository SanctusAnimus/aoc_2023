#ifndef AOC_DAY_20
#define AOC_DAY_20

#include "solution.hpp"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <bitset>

namespace D20
{
    std::vector<std::string_view> split_string(std::string_view s, std::string delimiter) {
        std::vector<std::string_view> result;
        size_t start = 0, end = -1;
        size_t inc = delimiter.size();
        do {
            end = s.find(delimiter, start);
            result.push_back(s.substr(start, end - start));
            start = end + inc;
        } while (end != -1);

        return result;

    }
    enum PulseStrength {
        Low, High
    };

    enum ModuleType {
        Base, Flip, Conjunction
    };

    struct ExecutionStep {
        std::string_view source;
        std::string_view destination;
        PulseStrength pulse;

        friend std::ostream& operator<<(std::ostream& os, ExecutionStep& lhs) {
            os << lhs.source << " => [" << lhs.pulse << "] => " << lhs.destination;
            return os;
        }
    };

    using Queue_T = std::queue<D20::ExecutionStep>;

    class BaseModule {
    public:
        BaseModule(std::string_view _label, std::vector<std::string_view>&& _targets) : label(_label), targets(_targets){};

        // update current state - switch for flip-flop, increment / decrement for conjunction, and propagate
        // may need return type (or ref arg) to fill queue with next steps
        virtual int UpdateState(PulseStrength pulse, Queue_T& queue, std::string_view& source) {
            // queue next steps
            return this->Enqueue(pulse, queue);
        };

        virtual ModuleType get_type() {
            return Base;
        }

        int Enqueue(PulseStrength pulse, Queue_T& queue) {
            for (const auto& target : targets) {
                queue.emplace(this->label, target, pulse);
            }

            return targets.size();
        }

        virtual void print() {
            std::cout << this->label << "\n-> ";
            for (const auto& t: targets) {
                std::cout << t << ", ";
            }
            std::cout << "\n";
        }

        std::string_view label; // label of this module
        std::vector<std::string_view> targets; // labels of targets
    };

    class FlipFlopModule : public BaseModule {
        // record current state, only emit if received low pulse, high if was off, low if was on, and switch
    public:
        FlipFlopModule(std::string_view _label, std::vector<std::string_view>&& _targets) : BaseModule(_label, std::move(_targets)) {}

        int UpdateState(PulseStrength pulse, Queue_T& queue, std::string_view& source) override {
            if (pulse == Low) {
                state = !state;
                pulse = state ? High : Low;
            }

            return this->Enqueue(pulse, queue);
        }

        ModuleType get_type() override {
            return Flip;
        }

        void print() override {
            std::cout << this->label << " [" << state << "]" << "\n-> ";
            for (const auto& t : targets) {
                std::cout << t << ", ";
            }
            std::cout << "\n";
        }

        bool state = false; // initially off
    };

    class ConjunctionModule : public BaseModule {
        // record all inputs, only emit if all states are high pulses
        // has low pulses by default
        // has to be updated externally
    public:
        ConjunctionModule(std::string_view _label, std::vector<std::string_view>&& _targets) : BaseModule(_label, std::move(_targets)) {}

        int UpdateState(PulseStrength pulse, Queue_T& queue, std::string_view& source) override {
            memory[source] = pulse == High ? true : false;

            bool all_high = std::all_of(memory.begin(), memory.end(), [](const auto& pair) { return pair.second; });

            // check if all values are true
            pulse = all_high ? Low : High;

            return this->Enqueue(pulse, queue);
        }

        ModuleType get_type() override {
            return Conjunction;
        }

        void print() override {
            std::cout << this->label << "\n-> ";
            for (const auto& t : targets) {
                std::cout << t << ", ";
            }
            std::cout << "\n";
            for (const auto& [src, state] : memory) {
                std::cout << "=>> " << src << " : " << state << "\n";
            }
        }

        std::unordered_map<std::string_view, bool> memory;
    };
}


class Day20 : public Solution<Day20> {
public:
    Day20() {
        day_num = 20;

        p1_expected = 899848294;
        p2_expected = 247454898168563;

        resolve_benchmark_tries = 200;
    };

    using Result_T = std::pair<intmax_t, intmax_t>;
    using ModuleCollection_T = std::unordered_map<std::string_view, std::shared_ptr<D20::BaseModule>>;

    void reset_state(ModuleCollection_T& modules) {
        for (auto& [label, _mod] : modules) {
            switch (_mod->get_type()) {
                case D20::Flip:
                {
                    std::shared_ptr<D20::FlipFlopModule> conj_mod = std::dynamic_pointer_cast<D20::FlipFlopModule>(_mod);
                    conj_mod->state = false;
                    break;
                }
                case D20::Conjunction:
                {
                    std::shared_ptr<D20::ConjunctionModule> conj_mod = std::dynamic_pointer_cast<D20::ConjunctionModule>(_mod);

                    for (auto& [key, _] : conj_mod->memory) {
                        conj_mod->memory[key] = false;
                    }
                    break;
                }
            }
        }
    }

    void run_wires(ModuleCollection_T& modules, intmax_t& low_pulses, intmax_t& high_pulses, auto&& on_step) {
        static std::string_view start = "button";

        D20::Queue_T exec_queue{};

        exec_queue.emplace("button", "roadcaster", D20::Low);

        for (; !exec_queue.empty(); exec_queue.pop()) {
            auto& next = exec_queue.front();

            on_step(next);

            if (!modules.contains(next.destination)) continue;

            const auto& mod = modules.at(next.destination);

            if (next.pulse == D20::High && mod->get_type() == D20::Flip) continue;

            mod->UpdateState(next.pulse, exec_queue, next.source);
        }
    }

    Result_T _get_solutions(SolutionInput_T solution_input) {
        intmax_t p1_result = 0;
        intmax_t p2_result = 0;

        ModuleCollection_T modules{};
        
        std::string_view rx_in;
        bool found_first_rx_in = false;

        // primary parsing pass
        // convert into modules, find first rx input label
        for (const auto& line : solution_input) {
            const char& module_type = line[0];

            std::string_view line_view = line;
            size_t label_end = line_view.find(' ', 2);

            std::string_view label = line_view.substr(1, label_end - 1);

            std::vector<std::string_view> targets = D20::split_string(line_view.substr(label_end + 4, line.size() - label_end), ", ");

            if (!found_first_rx_in) {
                for (const auto& v : targets) {
                    if (v == "rx") {
                        rx_in = label;
                        found_first_rx_in = true;
                    }
                }
            }

            switch (module_type) {
                case '%': // flip-flop
                {
                    modules.emplace(label, new D20::FlipFlopModule(label, std::move(targets)));
                    break;
                }
                case '&': // conjunction
                {
                    modules.emplace(label, new D20::ConjunctionModule(label, std::move(targets)));
                    break;
                }
                case 'b': // broadcaster
                {
                    modules.emplace(label, new D20::BaseModule(label, std::move(targets)));
                    break;
                }
            }
        }
        // second pass
        // rediscover inputs for conjunction modules
        for (auto& [label, _mod] : modules) {
            for (const auto& t : _mod->targets) {
                if (!modules.contains(t)) continue;

                const auto& target_mod = modules.at(t);
                if (target_mod->get_type() == D20::Conjunction) {
                    std::shared_ptr<D20::ConjunctionModule> conj_mod = std::dynamic_pointer_cast<D20::ConjunctionModule>(target_mod);
                    conj_mod->memory[label] = false;
                }
            }
        }

        // p1
        static const int start_pulses = 1000;
        intmax_t low_pulses = 0, high_pulses = 0;
        
        auto on_step_p1 = [&low_pulses, &high_pulses](D20::ExecutionStep& next) {
            if (next.pulse == D20::High) high_pulses += 1;
            else low_pulses += 1;
            };

        for (int i = 0; i < start_pulses; i++) {
            run_wires(modules, low_pulses, high_pulses, on_step_p1);
        }

        p1_result = low_pulses * high_pulses;

        // p2
        reset_state(modules);

        intmax_t step = 1;
        std::vector<intmax_t> p2_presses;

        std::shared_ptr<D20::ConjunctionModule> rx_in_mod = std::dynamic_pointer_cast<D20::ConjunctionModule>(modules.at(rx_in));

        do {
            auto on_step_p2 = [&p2_presses, &rx_in, &step](D20::ExecutionStep& next) {
                if (next.pulse == D20::High && next.destination == rx_in) p2_presses.push_back(step);
                };

            run_wires(modules, low_pulses, high_pulses, on_step_p2);

            step++;

            if (p2_presses.size() == rx_in_mod->memory.size()) {
                p2_result = 1;

                for (const auto& val : p2_presses) {
                    p2_result = std::lcm(p2_result, val);
                }

                break;
            }
        } while (1);


        return { p1_result, p2_result };
    }
};

#endif