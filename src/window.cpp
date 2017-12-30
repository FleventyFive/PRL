#include "window.hpp"

Window::Window(int _height, int _width, int _x, int _y): width(_width), height(_height), x(_x), y(_y) {
	std::unique_ptr<WINDOW, WindowDeleter> tmp(newwin(height, width, y, x), WindowDeleter());
	win = std::move(tmp);
}

void Window::fill(const char& c) {
	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			mvwaddch(win.get(), i, j, c);
		}
	}
}

void Window::clear() {
	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			mvwaddch(win.get(), i, j, ' ');
		}
	}
}

void Window::mvprint(const int& x, const int& y, const char* fmt, ...) {
	va_list args;
	wmove(win.get(), y, x);
	va_start(args, fmt);
	vw_printw(win.get(), fmt, args);
	va_end(args);
}

void Window::print(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vw_printw(win.get(), fmt, args);
	va_end(args);
}

void Window::clearBlock(const int& xFrom, const int& xTo, const int& yFrom, const int& yTo) {
	for(int i = yFrom; i < yTo; ++i) {
		for(int j = xFrom; j < xTo; ++j) {
			mvwaddch(win.get(), i, j, ' ');
		}
	}
}

std::string wrapByWord(const int& lineWidth, std::string text) {
	int start = 0, stop = 0;

	while(static_cast<std::size_t>(start) < text.size()) {
		stop = (static_cast<std::size_t>(start) + lineWidth) < text.size() ? start + lineWidth : text.size() - 1;
		if(static_cast<std::size_t>(stop) == text.size() - 1) {
			++stop;
		} else if(text[stop] == ' ') {
			text[stop] = '\n';
		} else {
			int tempStop = stop;
			while(text[tempStop] != ' ' && tempStop > start) {
				--tempStop;
			}
			if(text[tempStop] == ' ') {
				stop = tempStop;
				text[stop] = '\n';
			} else {
				text.insert(++stop, 1, '\n');
			}
		}
		start = stop;
	}

	return text;
}