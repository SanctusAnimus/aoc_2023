#ifndef AOC_DAY_10
#define AOC_DAY_10

#include "solution.hpp"
#include <algorithm>
#include <iterator>
#include <unordered_set>

enum Direction_D10 {
	Left = 1, Right = 2, Up = 4, Down = 8
};


struct LoopNode_D10 {
	int x = -1;
	int y = -1;

	Direction_D10 direction = Direction_D10::Left;

	bool operator==(LoopNode_D10& rhs) {
		return x == rhs.x && y == rhs.y;
	}

	int silly_hash() const {
		return 100000 * y + x;
	}

	friend std::ostream& operator<<(std::ostream& os, const LoopNode_D10& node) {
		os << "<LN: " << node.x << ":" << node.y << " : " << node.direction << ">";

		return os;
	}
};

bool point_in_poly(LoopNode_D10 point, std::vector<LoopNode_D10>& points) {
	int nvert = points.size();
	bool c = false;

	for (int i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((points[i].y >= point.y) != (points[j].y >= point.y)) &&
			(point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
			)
			c = !c;
	}

	return c;
}


class Day10 : public Solution<Day10> {
public:
	Day10() {
		day_num = 10;
	};

	using Result_T = std::pair<intmax_t, intmax_t>;

	size_t input_line_size = 0;

	int get_starting_axis(SolutionInput_T input, int start_x, int start_y) {
		int axis = 0;

		if (start_x > 0) {
			switch (input[start_y][start_x - 1]) {
				case 'L':
				case '-':
				case 'F':
					axis += Direction_D10::Left;
					break;
			}
		}

		if (start_x < input_line_size - 1) {
			switch (input[start_y][start_x + 1]) {
				case 'J':
				case '-':
				case '7':
					axis += Direction_D10::Right;
					break;
			}
		}

		if (start_y > 0) {
			switch (input[start_y - 1][start_x]) {
				case '7':
				case '|':
				case 'F':
					axis += Direction_D10::Up;
					break;
			}
		}

		if (start_y < input.size() - 1) {
			switch (input[start_y + 1][start_x]) {
				case 'J':
				case '|':
				case 'L':
					axis += Direction_D10::Down;
					break;
			}
		}

		return axis;
	}

	inline char axis_to_letter(int axis) {
		switch (axis) {
			case Direction_D10::Left + Direction_D10::Right:
				return '-';
			case Direction_D10::Up + Direction_D10::Down:
				return '|';
			case Direction_D10::Left + Direction_D10::Up:
				return 'J';
			case Direction_D10::Left + Direction_D10::Down:
				return '7';
			case Direction_D10::Right + Direction_D10::Up:
				return 'L';
			case Direction_D10::Right + Direction_D10::Down:
				return 'F';
		}

		return '.';
	}

	std::pair<LoopNode_D10, LoopNode_D10> build_loop_head(const char& starting_letter, int start_x, int start_y) {
		switch (starting_letter) {
			case '|':
				return { {start_x, start_y - 1, Up}, {start_x, start_y + 1, Down} };
			case '-':
				return { {start_x - 1, start_y, Left}, {start_x + 1, start_y, Right} };
			case 'L':
				return { {start_x, start_y - 1, Up}, {start_x + 1, start_y, Right} };
			case 'J':
				return { {start_x - 1, start_y, Left}, {start_x, start_y - 1, Up} };
			case '7':
				return { {start_x - 1, start_y, Left}, {start_x, start_y + 1, Down} };
			case 'F':
				return { {start_x + 1, start_y, Right}, {start_x, start_y + 1, Down} };
		}

		return { {-1, -1, Up}, {-1, -1, Up} };
	}

	Direction_D10 get_new_direction(const char& c, Direction_D10& previous_direction) {
		switch (c) {
			case '|':
				return previous_direction & Up ? Up : Down;
			case '-':
				return previous_direction & Right ? Right : Left;
			case 'L':
				return previous_direction & Down ? Right : Up;
			case 'J':
				return previous_direction & Down ? Left : Up;
			case '7':
				return previous_direction & Right ? Down : Left;
			case 'F':
				return previous_direction & Left ? Down : Right;
		}
	}

	LoopNode_D10 move_head(SolutionInput_T input, LoopNode_D10& head) {
		const char& c = input[head.y][head.x];
		Direction_D10 new_direction = get_new_direction(c, head.direction);

		switch (new_direction) {
			case Left: 
				return { head.x - 1, head.y, Left };
			case Right:
				return { head.x + 1, head.y, Right };
			case Up:
				return { head.x, head.y - 1, Up };
			case Down:
				return { head.x, head.y + 1, Down };
		}
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		intmax_t p1_result = 1;
		intmax_t p2_result = 0;

		input_line_size = solution_input[0].size();
		int start_x = 0;
		int start_y = 0;

		for (const auto& line : solution_input) {
			size_t pos = line.find('S');

			if (pos != std::string::npos) {
				//std::cout << "S found at " << pos << ":" << start_y << "\n";
				start_x = pos;
				break;
			}
			start_y++;
		}

		int axis = get_starting_axis(solution_input, start_x, start_y);

		auto [start_1, start_2] = build_loop_head(axis_to_letter(axis), start_x, start_y);

		// loop parts starting from 1 and 2 respectively
		std::vector<LoopNode_D10> path_1{ {start_x, start_y}, start_1 };
		std::vector<LoopNode_D10> path_2{ start_2 };

		do {
			start_1 = move_head(solution_input, start_1);
			start_2 = move_head(solution_input, start_2);

			path_1.push_back(start_1);

			if (start_1 != start_2) {
				path_2.push_back(start_2);
			}

			p1_result++;
		} while (start_1 != start_2);

		// merge path 2 into path 1 backwards, so they form complete polygon
		path_1.insert(path_1.end(), std::make_move_iterator(path_2.rbegin()), std::make_move_iterator(path_2.rend()));

		std::unordered_set<int> known_nodes;

		int min_x = 99999, max_x = 0, min_y = 99999, max_y = 0;

		// exclude edges, and narrow search range
		for (const auto& node : path_1) {
			known_nodes.insert(node.silly_hash());

			if (node.x < min_x) min_x = node.x;
			else if (node.x > max_x) max_x = node.x;

			if (node.y < min_y) min_y = node.y;
			else if (node.y > max_y) max_y = node.y;
		}

		// this is SLOW (42ms on my machine)
		// might want to add faster solution later
		for (int y = min_y; y < max_y; y++) {
			for (int x = min_x; x < max_x; x++) {
				if (!known_nodes.contains(100000 * y + x)) {
					bool r = point_in_poly({ x, y }, path_1);
					if (r) p2_result++;
				}
			}
		}
	
		// 6838 - 451
		return { p1_result, p2_result };
	}
};



#endif // !AOC_DAY_10
