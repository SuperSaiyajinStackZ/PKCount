#include "colors.hpp"
#include "graphics.hpp"
#include "gui.hpp"

#include "utils/keyboard.h"

#include <string>
#include <stdio.h>
#include <ctype.h>

struct Key {
	std::string character;
	int x;
	int y;
};

bool caps = false, enter = false;
int xPos = 0, shift = 0;
ImageData keyboardData;
std::string string;
std::vector<u16> keyboard;

Key keysQWE[] = {
	{"1", 0, 0}, {"2", 20, 0}, {"3", 40, 0}, {"4", 60, 0}, {"5", 80, 0}, {"6", 100, 0}, {"7", 120, 0}, {"8", 140, 0}, {"9", 160, 0}, {"0", 180, 0}, {"-", 200, 0}, {"=", 220, 0},
	{"q", 10, 18}, {"w", 30, 18}, {"e", 50, 18}, {"r", 70, 18}, {"t", 90, 18}, {"y", 110, 18}, {"u", 130, 18}, {"i", 150, 18}, {"o", 170, 18}, {"p", 190, 18}, {"[", 210, 18}, {"]", 230, 18},
	{"a", 20, 36}, {"s", 40, 36}, {"d", 60, 36}, {"f", 80, 36}, {"g", 100, 36}, {"h", 120, 36}, {"j", 140, 36}, {"k", 160, 36}, {"l", 180, 36}, {";", 200, 36}, {"'", 220, 36},
	{"z", 30, 54}, {"x", 50, 54}, {"c", 70, 54}, {"v", 90, 54}, {"b", 110, 54}, {"n", 130, 54}, {"m", 150, 54}, {",", 170, 54}, {".", 190, 54}, {"/", 210, 54},
};
Key keysSpecialQWE[] = {
	{"bksp", 240, 0},	// Backspace
	{"caps", 0, 36},	// Caps Lock
	{"entr", 240, 36},	// Enter
	{"lsft", 0, 54},	// Left Shift
	{"rsft", 230, 54},	// Right Shift
	{"mode", 50, 72},	// Change mode
};

void clearVars(void) {
	string = "", caps = false, shift = false, enter = false;
}

void whileHeld(void) {
	while(keysHeld()) {
		swiWaitForVBlank();
		scanKeys();
	}
}

void drawKeyboard(std::string inputText) {
	keyboard.clear();
	keyboardData = loadPng("nitro:/graphics/keyboard.png", keyboard);
	xPos = 0;
	Gui::DrawTop();
	Gui::DrawBottom();
	printTextCentered(inputText, 0, 2, true);
	drawRectangle(0, 192-keyboardData.height-16, 256, keyboardData.height+16, DARKER_GRAY, false);
	drawImage(xPos, 192-keyboardData.height, keyboardData.width, keyboardData.height, keyboard, false);
}

void processTouchQWE(touchPosition touch, uint maxLength) {
	if(string.length() < maxLength) {
		// Check if a regular key was pressed
		for(uint i=0;i<(sizeof(keysQWE)/sizeof(keysQWE[0]));i++) {
			if((touch.px > keysQWE[i].x-2 && touch.px < keysQWE[i].x+18) && (touch.py > keysQWE[i].y+(192-keyboardData.height)-2 && touch.py < keysQWE[i].y+18+(192-keyboardData.height))) {
				drawRectangle(keysQWE[i].x, keysQWE[i].y+(192-keyboardData.height), 16, 16, DARK_GRAY, false);
				char c = keysQWE[i].character[0];
				string += (shift || caps ? toupper(c) : c);
				shift = false;
				printText(string, 0, 192-keyboardData.height-16, false);
				break;
			}
		}
		// Check if space was pressed
		Key key = {" ", 70, 72};
		if((touch.px > key.x-2 && touch.px < key.x+100) && (touch.py > key.y+(192-keyboardData.height)-2 && touch.py < key.y+18+(192-keyboardData.height))) {
			drawRectangle(key.x, key.y+(192-keyboardData.height), 96, 16, DARK_GRAY, false);
			string += key.character;
			shift = false;
			printText(string, 0, 192-keyboardData.height-16, false);
		}
	}
	// Check if a special key was pressed
	for(uint i=0;i<(sizeof(keysSpecialQWE)/sizeof(keysSpecialQWE[0]));i++) {
		if((touch.px > keysSpecialQWE[i].x-2 && touch.px < keysSpecialQWE[i].x+18) && (touch.py > keysSpecialQWE[i].y+(192-keyboardData.height)-2 && touch.py < keysSpecialQWE[i].y+18+(192-keyboardData.height))) {
			if(keysSpecialQWE[i].character == "bksp") {
				drawRectangle(keysSpecialQWE[i].x, keysSpecialQWE[i].y+(192-keyboardData.height), 16, 16, DARK_GRAY, false);
				string = string.substr(0, string.length()-1);
				drawRectangle(0, 192-keyboardData.height-16, 256, 16, DARKER_GRAY, false);
				printText(string, 0, 192-keyboardData.height-16, false);
			} else if(keysSpecialQWE[i].character == "caps") {
				caps = !caps;
				if(caps) drawRectangle(keysSpecialQWE[i].x, keysSpecialQWE[i].y+(192-keyboardData.height), 16, 16, GRAY, false);
				whileHeld();
			} else if(keysSpecialQWE[i].character == "entr") {
				drawRectangle(keysSpecialQWE[i].x, keysSpecialQWE[i].y+(192-keyboardData.height), 16, 16, GRAY, false);
				whileHeld();
				enter = true;
			} else if(keysSpecialQWE[i].character == "lsft") {
				if(shift)	shift = 0;
				else {
					drawRectangle(keysSpecialQWE[i].x, keysSpecialQWE[i].y+(192-keyboardData.height), 26, 16, GRAY, false);
					whileHeld();
					shift = 1;
				}
			} else if(keysSpecialQWE[i].character == "rsft") {
				if(shift)	shift = 0;
				else {
					drawRectangle(keysSpecialQWE[i].x, keysSpecialQWE[i].y+(192-keyboardData.height), 26, 16, GRAY, false);
					whileHeld();
					shift = 2;
				}
			}
			break;
		}
	}
}

std::string Input::getLine(std::string inputText) { return Input::getLine(-1, inputText); }

std::string Input::getLine(uint maxLength, std::string inputText) {
	clearVars();
	drawKeyboard(inputText);
	int held, pressed, cursorBlink = 30;
	touchPosition touch;
	while(1) {
		do {
			swiWaitForVBlank();
			scanKeys();
			held = keysDownRepeat();
			pressed = keysDown();
			if(cursorBlink == 30) {
				drawRectangle(0, 192-keyboardData.height-16, 256, 16, DARKER_GRAY, false);
				printText(string+"_", 0, 192-keyboardData.height-16, false);
			} else if(cursorBlink == 0) {
				drawRectangle(0, 192-keyboardData.height-16, 256, 16, DARKER_GRAY, false);
				printText(string, 0, 192-keyboardData.height-16, false);
			} else if(cursorBlink == -30) {
				cursorBlink = 31;
			}
			cursorBlink--;
		} while(!held);
			if(caps) drawRectangle(keysSpecialQWE[1].x, keysSpecialQWE[1].y+(192-keyboardData.height), 16, 16, GRAY, false);

		if(pressed & KEY_TOUCH) {
			touchRead(&touch);
			processTouchQWE(touch, maxLength);
			
			// Redraw keyboard to cover up highlight
			drawKeyboard(inputText);
			// Print string
			drawRectangle(0, 192-keyboardData.height-16, 256, 16, DARKER_GRAY, false);
			printText(string + (cursorBlink ? "_" : ""), 0, 192-keyboardData.height-16, false);

			// If caps lock / shift are on, highlight the key
			if(caps)	drawRectangle(keysSpecialQWE[1].x, keysSpecialQWE[1].y+(192-keyboardData.height), 16, 16, GRAY, false);
			if(shift)	drawRectangle(keysSpecialQWE[2+shift].x, keysSpecialQWE[2+shift].y+(192-keyboardData.height), 26, 16, GRAY, false);
		} else if(held & KEY_B) {
			drawRectangle(keysSpecialQWE[0].x, keysSpecialQWE[0].y+(192-keyboardData.height), 16, 16, DARK_GRAY, false);
			string = string.substr(0, string.length()-1);
			drawRectangle(0, 192-keyboardData.height-16, 256, 16, DARKER_GRAY, false);
			printText(string, 0, 192-keyboardData.height-16, false);
		}
		if(held & KEY_START || enter) {
			break;
		}
	}
	return string;
}