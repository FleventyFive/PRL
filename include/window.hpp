#pragma once

#include <cstdarg>
#include <iostream>
#include <memory>
#include <string>

#include "BearLibTerminal/BearLibTerminal.h"

class Window {
private:
	int width, height, x, y;
public:
	void fill(const char* c);
	// void printChar(const int& _x, const int& _y, const char* c) { terminal_print(x + _x, y + _y, c); }
	void print(const int& _x, const int& _y, const char* fmt, ...);
	void clear();
	void clearBlock(const int& xFrom, const int& xTo, const int& yFrom, const int& yTo);

	constexpr int getHeight() noexcept { return height; }
	constexpr int getWidth() noexcept { return width; }

	Window(int _height, int _width, int _x, int _y);
	~Window() = default;
};

// Helper function for wrapping when string is too long for the window
std::string wrapByWord(const int& lineWidth, std::string text);
std::string wrapByWordToWindow(const int& lineWidth, const int& lineHeight, std::string text);