/*
*   This file is part of PKCount
*   Copyright (C) 2019-2020 Stack-Team
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

#include "encounterScreen.hpp"
#include "gui.hpp"
#include "inifile.h"
#include "keyboard.h"

#include <string>

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool exiting;
CIniFile encounterIni("sd:/_nds/PKCount/Encounter.ini");

Encounter::Encounter() {
	if (Gui::promptMsg("Would you like to read the latest Encounter\nOr a specific Encounter?", "Latest Encounter", "Specific Encounter")) {
		speciesName = "LatestEncounter";
		currentGeneration = "LatestEncounter";
		readCurrentEnc();
	} else {
		currentGeneration = Input::getLine("Enter the current Generation.");
		speciesName = Input::getLine("Enter the current Species name.");
		readCurrentEnc();
	}
}

void Encounter::DrawCredits(void) const {
	Gui::DrawTop();
	printText("PKCount by StackZ", 80, 2, true);
	drawImage(0, 70, stackZData.width, stackZData.height, stackZ, true);
	printTextScaled("Hello there! I'm StackZ.\nI'm the Developer of this App.\nI hope you enjoy it!\nPress on the bar again\nto switch back to the MainScreen.\nSee ya, StackZ.", 0.9, 0.9, 100, 50, true);
	Gui::DrawBottom();
}



void Encounter::Draw(void) const {
	if (screenDrawn) return;
	if (screenMode == 0) {
		Gui::DrawTop();
		printTextCentered("PKCount by Stack-Team", 0, 2, true);

		printTextCentered("Current Species: " + speciesName, 0, 50, true);
		printTextCentered("Current Generation: " + currentGeneration, 0, 85, true);
		printTextCentered("Current Encounter: " + std::to_string(currentEncounter), 0, 120, true);
		printText(V_STRING, 160, 175, true);
		Gui::DrawBottom();
		drawImage(mainButtons[0].x, mainButtons[0].y, plusIconData.width, plusIconData.height, plusIcon, false);
		drawImage(mainButtons[1].x, mainButtons[1].y, minusIconData.width, minusIconData.height, minusIcon, false);
		drawImage(mainButtons[2].x, mainButtons[2].y, resetIconData.width, resetIconData.height, resetIcon, false);
	} else if (screenMode == 1) {
		DrawCredits();
	}

	screenDrawn = true;
}


void Encounter::Logic(u16 hDown, touchPosition touch) {
	if (screenMode == 0) {
		if (hDown & KEY_START) {
			encounterIni.SetInt("LatestEncounter", "LatestEncounter", currentEncounter);
			encounterIni.SaveIniFile("sd:/_nds/PKCount/Encounter.ini");
			exiting = true;
		}

		if (keysDownRepeat() & KEY_B) {
			Gui::DrawBox("<START>: Exit and Save Encounter.\n\n<X>: Save current Encounter.\n\n<SELECT>: Switch Encounter.\n\n<Y>: Create New Encounter.\n\n<Left/Right/Touch>: increase / decrease Encounter.\n\n<Touch Bar>: Show Credits.");
		}


		if (hDown & KEY_X) {
			saveCurrentEnc();
			screenDrawn = false;
		}

		if (hDown & KEY_SELECT) {
			currentGeneration = Input::getLine("Enter the current Generation.");
			speciesName = Input::getLine("Enter the current Species name.");
			readCurrentEnc();
			screenDrawn = false;
		}

		if (hDown & KEY_Y) {
			if (Gui::promptMsg("Would you like to create a new Encounter?", "Yes", "No")) {
				createNewEnc();
			}

			screenDrawn = false;
		}

		if (hDown & KEY_RIGHT || hDown & KEY_R) {
			Sound::play(Sound::count);
			currentEncounter++;
			screenDrawn = false;
		}

		if (hDown & KEY_LEFT || hDown & KEY_L) {
			Sound::play(Sound::count);
			if (currentEncounter > 0)	currentEncounter--;
			screenDrawn = false;
		}


		if (hDown & KEY_TOUCH) {
			if (touching(touch, mainButtons[0])) {
				Sound::play(Sound::count);
				currentEncounter++;
				screenDrawn = false;
			} else if (touching(touch, mainButtons[1])) {
				Sound::play(Sound::count);
				if (currentEncounter > 0)	currentEncounter--;
				screenDrawn = false;
			} else if (touching(touch, mainButtons[2])) {
				Sound::play(Sound::count);
				currentEncounter = 0;
				screenDrawn = false;
			} else if (touching(touch, mainButtons[3])) {
				screenMode = 1;
				screenDrawn = false;
			}
		}

		// Credits Screen.
	} else if (screenMode == 1) {
		if (hDown & KEY_TOUCH) {
			if (touching(touch, mainButtons[3])) {
				screenMode = 0;
				screenDrawn = false;
			}
		}
	}
}

void Encounter::createNewEnc() {
	currentGeneration = Input::getLine("Enter the current Generation.");
	speciesName = Input::getLine("Enter the current Species name.");
	currentEncounter = encounterIni.GetInt(currentGeneration, speciesName, 0);
	encounterIni.SaveIniFile("sd:/_nds/PKCount/Encounter.ini");
}

void Encounter::readCurrentEnc() {
	currentEncounter = encounterIni.GetInt(currentGeneration, speciesName, currentEncounter);
}


void Encounter::saveCurrentEnc() {
	if (Gui::promptMsg("Would you like to save to the current\nEncounter\nor create a new Encounter?", "Current Encounter", "New Encounter")) {
		encounterIni.SetInt(currentGeneration, speciesName, currentEncounter);
	} else {
		currentGeneration = Input::getLine("Enter the current Generation.");
		speciesName = Input::getLine("Enter the current Species name.");
		encounterIni.SetInt(currentGeneration, speciesName, currentEncounter);
	}

	encounterIni.SaveIniFile("sdmc:/_nds/PKCount/Encounter.ini");
}