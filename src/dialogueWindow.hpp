#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <deque>

#include <cstdarg>
#include <cmath>

#include "window.hpp"

class DialogueWindow: public Window {
private:
  std::deque<std::string> dialogue;

  int newLineCount = 0;
public:
  void printMessages();
  void addMessage(const char* fmt, ...);
  constexpr void resetCounter() noexcept { newLineCount = 0; }

  DialogueWindow(int height, int width, int x, int y): Window(height, width, x, y), dialogue(height) {  }
};
