#pragma once

#include <cstdarg>
#include <iostream>
#include <memory>
#include <string>

#include <ncurses.h>

class Window {
private:
	int width, height, x, y;

	struct WindowDeleter {
		void operator()(WINDOW* ptr) const { delwin(ptr); }

		WindowDeleter() = default;
	};

	std::unique_ptr<WINDOW, WindowDeleter> win;
public:
	void fill(const char& c);
	void printChar(const int& x, const int& y, const char& c) { mvwaddch(win.get(), y, x, c); }
	void mvprint(const int& x, const int& y, const char* fmt, ...);
	void print(const char* fmt, ...);
	void refresh() noexcept { wrefresh(win.get()); }
	void moveCursor(const int& x, const int& y) noexcept { wmove(win.get(), y, x); }
	void clear();
	void clearBlock(const int& xFrom, const int& xTo, const int& yFrom, const int& yTo);

	WINDOW* get() { return win.get(); }

	constexpr int getHeight() noexcept { return height; }
	constexpr int getWidth() noexcept { return width; }

	Window(int _height, int _width, int _x, int _y);
	~Window() = default;
};

// Helper function for wrapping when string is too long for the window
std::string wrapByWord(const int& lineWidth, std::string text);