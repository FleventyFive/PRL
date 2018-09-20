#include <iostream>

#include "game.hpp"

#include "BearLibTerminal/BearLibTerminal.h"

#define VERSION_NUMBER "0.2.7"

int main() {
	std::cout << "Last compiled : " << __DATE__ << ", " << __TIME__ << '\n';
	std::cout << "Version - " << VERSION_NUMBER << '\n';
	std::cout << "Developed by Mark Calhoun: https://github.com/FleventyFive\n";

	// Init BearLibTerminal
	terminal_open();

	// terminal_set("ini.game.tile-size = 32; window: title='Roguelike', size=91x20; log: file=bt.log, level=trace; font: UbuntuMono-R.ttf, size=12; italic font: UbuntuMono-RI.ttf, size=12;");

	Game game;

	// End BearLibTerminal
	terminal_close();

	return 0;
}
