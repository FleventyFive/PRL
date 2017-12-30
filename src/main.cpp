#include <iostream>

#include <ncurses.h>

#include "game.hpp"

#define VERSION_NUMBER "0.2.7"

int main() {
	std::cout << "Last compiled : " << __DATE__ << ", " << __TIME__ << '\n';
	std::cout << "Version - " << VERSION_NUMBER << '\n';
	std::cout << "Developed by Mark Calhoun: https://github.com/FleventyFive\n";

	// Init Ncurses
	initscr();
	cbreak();
	curs_set(0);
	refresh();
	noecho();
	assert(start_color() == OK);
	use_default_colors();

	for(int i = 0; i < COLORS; ++i) {
		init_pair(i, i, -1);
	}

	if(can_change_color() && COLORS >= 256) {
		// init_color(COLOR_BLACK, 0, 0, 0);
		init_color(COLOR_RED, 1000, 0, 0);
		init_color(COLOR_GREEN, 0, 0, 1000);
		init_color(COLOR_YELLOW, 1000, 1000, 0);
		// init_color(COLOR_BLUE, 0, 0, 1000);
		init_color(COLOR_MAGENTA, 1000, 0, 1000);
		init_color(COLOR_CYAN, 0, 1000, 1000);
		init_color(COLOR_WHITE, 1000, 1000, 1000);
	}
	
	Game game;

	use_default_colors();

	// End Ncurses
	endwin();
	return 0;
}
