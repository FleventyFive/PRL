#include "window.hpp"

Window::Window(int _height, int _width, int _x, int _y): width(_width), height(_height), x(_x), y(_y) {

}

void Window::fill(const char* c) {
	for(int i = 0; i < width; ++i) {
		for(int j = 0; j < height; ++j) {
			terminal_print(i + x, j + y, c);
		}
	}
}

void Window::clear() {
	for(int i = 0; i < width; ++i) {
		for(int j = 0; j < height; ++j) {
			terminal_print(i + x, j + y, " ");
		}
	}
}

void Window::print(const int& _x, const int& _y, const char* fmt, ...) {
	char buff[256];

	va_list args;
	va_start(args, fmt);
	vsprintf(buff, fmt, args);
	va_end(args);

	std::string msg = buff;
	
	terminal_print(x + _x, y + _y, wrapByWordToWindow(width, height, msg).c_str());
}

void Window::clearBlock(const int& xFrom, const int& xTo, const int& yFrom, const int& yTo) {
	for(int i = xFrom; i < xTo; ++i) {
		for(int j = yFrom; j < yTo; ++j) {
			terminal_print(i, j, " ");
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

std::string wrapByWordToWindow(const int& lineWidth, const int& lineHeight, std::string text) {
	int start = 0, stop = 0, height = 0;

	while(static_cast<std::size_t>(start) < text.size()) {
		stop = (static_cast<std::size_t>(start) + lineWidth) < text.size() ? start + lineWidth : text.size() - 1;
		if(static_cast<std::size_t>(stop) == text.size() - 1) {
			++stop;
		} else if(text[stop] == ' ') {
			text[stop] = '\n';
			++height;
		} else {
			int tempStop = stop;
			while(text[tempStop] != ' ' && tempStop > start) {
				--tempStop;
			}
			if(text[tempStop] == ' ') {
				stop = tempStop;
				text[stop] = '\n';
				++height;
			} else {
				text.insert(++stop, 1, '\n');
				++height;
			}
		}
		if(height == lineHeight) {
			text = text.substr(0, stop);
			break;
		}
		start = stop;
	}

	return text;
}
