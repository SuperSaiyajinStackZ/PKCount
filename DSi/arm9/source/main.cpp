/*
*   This file is part of PKCount
*   Copyright (C) 2019-2020 StackZ
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "colors.hpp"
#include "graphics.hpp"
#include "gui.hpp"

#include "screens/screenCommon.hpp"
#include "screens/encounterScreen.hpp"

#include "utils/nitrofs.h"
#include "utils/sound.hpp"
#include "utils/structs.hpp"

#include <fat.h>

bool exiting = false;

bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}

touchPosition touch;

int main(int argc, char **argv) {
	initGraphics();
	keysSetRepeat(25,5);
	defaultExceptionHandler();
	scanKeys();

	drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, true);
	drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, false);

	// Init filesystem
	if(!fatInitDefault()) {
		// Prints error if fatInitDefault() fails
		consoleDemoInit();
		printf("fatInitDefault() failed...");
		while(1) swiWaitForVBlank();
	}

	// Make directories
	mkdir("/_nds", 0777);
	mkdir("/_nds/PKCount", 0777);
	// Try to init NitroFS from argv provided to the game when it was launched
	if(!nitroFSInit(argv[0])) {
		// If that fails, try to init NitroFS on 'PKCount.nds'
		if(!nitroFSInit("PKCount.nds")) {
			if(!nitroFSInit("/_nds/PKCount/PKCount.nds")) {
				// Prints error if nitroFSInit() fails
				consoleDemoInit();
				printf("nitroFSInit() failed...\n\n");
				while(1) swiWaitForVBlank();
			}
		}
	}
	loadFont();
	printTextCentered("Loading...", 0, 32, false);

	Sound::init();
	loadGraphics();
	Gui::setScreen(std::make_unique<Encounter>());
	u16 hDown = 0;

	while(!exiting) {
		scanKeys();
		swiWaitForVBlank();
		hDown = keysDown();
		touchRead(&touch);
		Gui::mainLoop(hDown, touch);
	}
	return 0;
}