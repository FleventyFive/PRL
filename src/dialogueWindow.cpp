#include "dialogueWindow.hpp"

void DialogueWindow::printMessages() {
	clear();
	for(std::size_t i = 0; i < dialogue.size(); ++i) {
		if(dialogue.size() - i > static_cast<std::size_t>(newLineCount)) {
			// wattron(get(), COLOR_PAIR(COLOR_BLACK));
			print(0, i, (dialogue[i]).c_str());
			// wattroff(get(), COLOR_PAIR(COLOR_BLACK));
		} else {
			// wattron(get(), COLOR_PAIR(COLOR_WHITE));
			print(0, i, (dialogue[i]).c_str());
			// wattroff(get(), COLOR_PAIR(COLOR_WHITE));
		}
	}
}


void DialogueWindow::addMessage(const char* fmt, ...) {
	// convert the input into a string
	char buff[256];

	va_list args;
	va_start(args, fmt);
	vsprintf(buff, fmt, args);
	va_end(args);

	std::string msg = buff;

	// Chop the message into strings as wide as the window
	// temporary algorithm
	if(msg.size() > static_cast<std::size_t>(getWidth())) {
		std::vector<std::string> segments;

		for(std::size_t i = 0; i < msg.size(); i += getWidth() - 1) {
			segments.push_back(msg.substr(i, getWidth() - 1));
		}

		for(const auto& segment : segments) {
			if(dialogue.size() == static_cast<std::size_t>(getHeight())) {
				dialogue.pop_front();
			}
			dialogue.push_back(segment);
		}
		newLineCount += segments.size();
	} else {
		if(dialogue.size() == static_cast<std::size_t>(getHeight())) {
			dialogue.pop_front();
		}
		dialogue.push_back(msg);
		++newLineCount;
	}
}
