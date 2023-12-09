#ifndef AOC_DAY_7
#define AOC_DAY_7

#include "solution.hpp"
#include <array>
#include <unordered_map>

// akqjt98765432

enum HandKind {
	HighCard = 1, OnePair, TwoPair, ThreeOfKind, FullHouse, FourOfKind, FiveOfKind
};


struct Hand {
	HandKind kind;
	unsigned bet;
	std::array<unsigned, 5> cards;

	bool operator<(Hand& rhs) {
		auto kind_difference = kind - rhs.kind;

		if (kind_difference != 0) return kind_difference < 0;

		for (int i = 0; i < 5; i++) {
			int card_power_difference = cards[i] - rhs.cards[i];

			if (card_power_difference != 0) return card_power_difference < 0;
		}

		return true;
	}

	friend std::ostream& operator<<(std::ostream& os, const Hand& rhs) {
		for (const auto& card : rhs.cards) {
			switch (card) {
				case 14:
					os << 'A';
					break;
				case 13:
					os << 'K';
					break;
				case 12:
					os << 'Q';
					break;
				case 11:
					os << 'J';
					break;
				case 10:
					os << 'T';
					break;
				case 1:
					os << 'j';
					break;
				default:
					os << char(card + '0');
			};
		}

		os << " : " << rhs.bet << ">";

		return os;
	}
};


class Day7 : public Solution<Day7> {
public:
	Day7() {
		day_num = 7;
	};

	using Result_T = std::pair<int, int>;


	inline int get_card_power(const char& c) {
		switch (c) {
			case 'A':
				return 14;
			case 'K':
				return 13;
			case 'Q':
				return 12;
			case 'J':
				return 11;
			case 'T':
				return 10;
			default:
				return c - '0';
		};

		return 0;
	}

	HandKind hand_kind_from_repeats(std::array<unsigned, 5>& repeats) {
		if (repeats[0] == 5) return FiveOfKind;
		if (repeats[0] == 4) return FourOfKind;
		if (repeats[0] == 3 and repeats[1] == 2) return FullHouse;
		if (repeats[0] == 3) return ThreeOfKind;
		if (repeats[0] == 2 and repeats[1] == 2) return TwoPair;
		if (repeats[0] == 2) return OnePair;

		return HandKind::HighCard;
	}

	HandKind get_hand_kind(Hand& hand) {
		std::unordered_map<unsigned, unsigned> times;

		for (const int& card_power : hand.cards) {
			times[card_power]++;
		}
		
		auto view_it = std::views::values(times);
		std::array<unsigned, 5> repeats{0};

		unsigned i = 0;
		for (const auto& el : view_it) {
			repeats[i++] = el;
		}

		std::sort(repeats.begin(), repeats.end(), std::greater<>());

		//std::cout << "\t\tgetting hand kind: " << repeats[0] << " " << repeats[1] << '\n';

		return hand_kind_from_repeats(repeats);
	}

	void into_joker_card(Hand& hand) {
		// mutate hand by changing hand kind in accordance to joker rules (it being wildcard)
		// as well as replacing power of it with 1 everywhere in occurs for comparisons

		std::unordered_map<unsigned, unsigned> times;

		unsigned joker_times = 0;

		for (unsigned int& card_power : hand.cards) {
			if (card_power == 11) {
				card_power = 1;
				joker_times++;
				continue;
			}

			times[card_power]++;
		}

		auto view_it = std::views::values(times);
		std::array<unsigned, 5> repeats{ 0 };

		unsigned i = 0;
		for (const auto& el : view_it) {
			repeats[i++] = el;
		}

		std::sort(repeats.begin(), repeats.end(), std::greater<>());
		
		repeats[0] += joker_times;

		hand.kind = hand_kind_from_repeats(repeats);
	}

	Result_T _get_solutions(SolutionInput_T solution_input) {
		unsigned p1_result = 0;
		unsigned p2_result = 0;

		std::vector<Hand> hands;
		hands.reserve(1000);

		for (const auto& line : solution_input) {
			// first 5 chars are cards, then whitespace, then bet
			unsigned bet = 0;
			std::from_chars(line.data() + 6, line.data() + line.size(), bet);

			Hand hand{ HandKind::HighCard, bet};

			for (int i = 0; i < 5; i++) {
				hand.cards[i] = get_card_power(line[i]);
			}

			hand.kind = get_hand_kind(hand);

			hands.push_back(hand);
		}

		std::sort(hands.begin(), hands.end());

		unsigned rank = 1;
		for (auto& hand : hands) {
			p1_result += (hand.bet * rank++);

			into_joker_card(hand);
		}

		// re-sort for p2 after turning into joker cards
		std::sort(hands.begin(), hands.end());

		rank = 1;
		for (const auto& hand : hands) {
			p2_result += (hand.bet * rank++);
		}

		// 248179786 - 247885995
		return { p1_result, p2_result };
	}
};


#endif // !AOC_DAY_7