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

#include "gui.hpp"

#include "screens/encounterScreen.hpp"

#include "utils/inifile.h"
#include "utils/keyboard.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern int textColor;
CIniFile encounterIni("sdmc:/3ds/PKCount/Encounter.ini");

Encounter::Encounter() {
	speciesName = "LatestEncounter";
	currentGeneration = "LatestEncounter";
	readCurrentEnc();
}


void Encounter::Draw(void) const
{
	Gui::DrawTop();
	Gui::DrawStringCentered(0, -1, 0.8f, textColor, "PKCount - Encounter Screen");
	Gui::DrawStringCentered(0, 50, 0.8f, textColor, "Current Species: " + speciesName);
	Gui::DrawStringCentered(0, 85, 0.8f, textColor, "Current Generation: " + currentGeneration);
	Gui::DrawStringCentered(0, 120, 0.8f, textColor, "Current Encounter: " + std::to_string(currentEncounter));
	Gui::DrawString(397-Gui::GetStringWidth(0.6f, V_STRING), 237-Gui::GetStringHeight(0.6f, V_STRING), 0.6f, WHITE, V_STRING);
	Gui::DrawBottom();
	Gui::DrawStringCentered(0, 0, 0.8f, textColor, "Hold B for Instructions.");
	Gui::sprite(sprites_plus_idx, mainButtons[0].x, mainButtons[0].y);
	Gui::sprite(sprites_minus_idx, mainButtons[1].x, mainButtons[1].y);
	Gui::sprite(sprites_reset_idx, mainButtons[2].x, mainButtons[2].y);
}


void Encounter::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_START) {
		if (Gui::promptMsg("Would you like to switch your Encounter?", "Yes", "No")) {
			// Save Current Encounter before switching.
			encounterIni.SetInt(currentGeneration, speciesName, currentEncounter);
			currentGeneration = Input::getString("Enter the current Generation.");
			speciesName = Input::getString("Enter the current Species name.");
			readCurrentEnc();
		}
	}

	if (hDown & KEY_X) {
		saveCurrentEnc();
	}
	if (hHeld & KEY_B) {
		Gui::HelperBox("<A>: Set Encounter manually.\n\n<START>: Save and switch Encounter.\n\n<X>: Save current Encounter.\n\n<SELECT>: Screen Selection.\n\n<Y>: Create New Encounter.\n\n<Left/Right/Touch>: increase / decrease Encounter.\n");
	}

	if (hDown & KEY_A) {
		std::string currentEncounter2 = Input::Numpad("Enter the current Encounter.");
		currentEncounter = std::stoi(currentEncounter2);
	}

	if (hDown & KEY_Y) {
		if (Gui::promptMsg("Would you like to create a new Encounter?", "Yes", "No")) {
			createNewEnc();
		}
	}

	if (hDown & KEY_RIGHT || hDown & KEY_R) {
		playCount();
		currentEncounter++;
	}
	if (hDown & KEY_LEFT || hDown & KEY_L) {
		playCount();
		if (currentEncounter > 0)	currentEncounter--;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			playCount();
			currentEncounter++;
		} else if (touching(touch, mainButtons[1])) {
			playCount();
			if (currentEncounter > 0)	currentEncounter--;
		} else if (touching(touch, mainButtons[2])) {
			playCount();
			currentEncounter = 0;
		}
	}
}



void Encounter::createNewEnc() {
	currentGeneration = Input::getString("Enter the current Generation.");
	speciesName = Input::getString("Enter the current Species name.");
	currentEncounter = encounterIni.GetInt(currentGeneration, speciesName, 0);
	encounterIni.SaveIniFile("sdmc:/3ds/PKCount/Encounter.ini");
}

void Encounter::readCurrentEnc() {
	currentEncounter = encounterIni.GetInt(currentGeneration, speciesName, currentEncounter);
}

void Encounter::saveCurrentEnc() {
	if (Gui::promptMsg("Would you like to save to the current Encounter\nor create a new Encounter?", "Current Encounter", "New Encounter")) {
		encounterIni.SetInt(currentGeneration, speciesName, currentEncounter);
	} else {
		currentGeneration = Input::getString("Enter the current Generation.");
		speciesName = Input::getString("Enter the current Species name.");
		encounterIni.SetInt(currentGeneration, speciesName, currentEncounter);
	}
	encounterIni.SaveIniFile("sdmc:/3ds/PKCount/Encounter.ini");
}