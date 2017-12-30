#pragma once

#include <iostream>
#include <random>

class Die {
private:
	std::mt19937 eng{std::random_device{}()};

	unsigned int rolls, sides;
public:
	unsigned int roll() {
		unsigned int roll = 0;
		for(unsigned int i = 0; i < rolls; ++i) {
			roll += std::uniform_int_distribution<unsigned int>{1, sides}(eng);
		}
		return roll;
	}

	Die(unsigned int _rolls, unsigned int _sides): rolls(_rolls), sides(_sides) { }
};
