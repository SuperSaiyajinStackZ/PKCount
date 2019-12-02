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

#include <string>

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool exiting;
CIniFile encounterIni("sdmc:/3ds/PKCount/Encounter.ini");

Encounter::Encounter() {
	if (Gui::promptMsg("Would you like to read the latest Encounter\nOr a specific Encounter?", "Latest Encounter", "Specific Encounter")) {
		speciesName = "LatestEncounter";
		currentGeneration = "LatestEncounter";
		readCurrentEnc();
	} else {
		currentGeneration = getString(50, "Enter the current Generation.");
		speciesName = getString(50, "Enter the current Species name.");
		readCurrentEnc();
	}
}

void Encounter::DrawCredits(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, C2D_Color32(255, 255, 255, 255), "PKCount by StackZ");
	Gui::sprite(sprites_stackZ_idx, 0, 45);
	Gui::DrawString(140, 65, 0.6f, WHITE, "Hello there! I'm StackZ.\nI'm the Developer of this App. \nI hope you enjoy it!\nPress on the bar again,\nto switch back to the Main Screen.\nSee ya, StackZ.");
	Gui::DrawBottom();
}



void Encounter::Draw(void) const
{
	if (screenMode == 0) {
		Gui::DrawTop();
		Gui::DrawStringCentered(0, 0, 0.8f, C2D_Color32(255, 255, 255, 255), "PKCount by StackZ");
		Gui::DrawStringCentered(0, 50, 0.8f, C2D_Color32(255, 255, 255, 255), "Current Species: " + speciesName);
		Gui::DrawStringCentered(0, 85, 0.8f, C2D_Color32(255, 255, 255, 255), "Current Generation: " + currentGeneration);
		Gui::DrawStringCentered(0, 120, 0.8f, C2D_Color32(255, 255, 255, 255), "Current Encounter: " + std::to_string(currentEncounter));
		Gui::DrawString(397-Gui::GetStringWidth(0.6f, V_STRING), 237-Gui::GetStringHeight(0.6f, V_STRING), 0.6f, WHITE, V_STRING);
		Gui::DrawBottom();
		Gui::DrawStringCentered(0, 0, 0.8f, C2D_Color32(255, 255, 255, 255), "Hold B for Instructions.");
		Gui::sprite(sprites_plus_idx, mainButtons[0].x, mainButtons[0].y);
		Gui::sprite(sprites_minus_idx, mainButtons[1].x, mainButtons[1].y);
		Gui::sprite(sprites_reset_idx, mainButtons[2].x, mainButtons[2].y);
	} else if (screenMode == 1) {
		DrawCredits();
	}
}


void Encounter::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (screenMode == 0) {
		if (hDown & KEY_START) {
			encounterIni.SetInt("LatestEncounter", "LatestEncounter", currentEncounter);
			encounterIni.SaveIniFile("sdmc:/3ds/PKCount/Encounter.ini");
			exiting = true;
		}
		if (hDown & KEY_X) {
			saveCurrentEnc();
		}

		if (hHeld & KEY_B) {
			Gui::HelperBox("<START>: Exit and Save Encounter.\n\n<X>: Save current Encounter.\n\n<SELECT>: Switch Encounter.\n\n<Y>: Create New Encounter.\n\n<Left/Right/Touch>: increase / decrease Encounter.\n\n<Touch Bar>: Show Credits.");
		}

		if (hDown & KEY_SELECT) {
			currentGeneration = getString(50, "Enter the current Generation.");
			speciesName = getString(50, "Enter the current Species name.");
			readCurrentEnc();
		}

		if (hDown & KEY_Y) {
			if (Gui::promptMsg("Would you like to create a new Encounter?", "Yes", "No")) {
				createNewEnc();
			}
		}

		if (hDown & KEY_RIGHT || hDown & KEY_R) {
			currentEncounter++;
		}

		if (hDown & KEY_LEFT || hDown & KEY_L) {
			if (currentEncounter > 0)	currentEncounter--;
		}


		if (hDown & KEY_TOUCH) {
			if (touching(touch, mainButtons[0])) {
				currentEncounter++;
			} else if (touching(touch, mainButtons[1])) {
				if (currentEncounter > 0)	currentEncounter--;
			} else if (touching(touch, mainButtons[2])) {
				currentEncounter = 0;
			} else if (touching(touch, mainButtons[3])) {
				screenMode = 1;
			}
		}

		// Credits Screen.
	} else if (screenMode == 1) {
		if (hDown & KEY_TOUCH) {
			if (touching(touch, mainButtons[3])) {
				screenMode = 0;
			}
		}
	}
}

std::string Encounter::getString(uint maxLength, const char *hint)
{
	std::string normalString;
	C3D_FrameEnd(0);
	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NORMAL, 2, maxLength);
	swkbdSetHintText(&state, hint);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, SWKBD_FILTER_PROFANITY, 0);
	char input[maxLength + 1]	= {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[maxLength]		= '\0';
	if (ret == SWKBD_BUTTON_CONFIRM)
	{
		normalString = input;
		return normalString;
	} else {
		return "";
	}
}

void Encounter::createNewEnc() {
	currentGeneration = getString(50, "Enter the current Generation.");
	speciesName = getString(50, "Enter the current Species name.");
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
		currentGeneration = getString(50, "Enter the current Generation.");
		speciesName = getString(50, "Enter the current Species name.");
		encounterIni.SetInt(currentGeneration, speciesName, currentEncounter);
	}
	encounterIni.SaveIniFile("sdmc:/3ds/PKCount/Encounter.ini");
}