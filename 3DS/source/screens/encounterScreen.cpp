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

#include "common.hpp"
#include "encounterScreen.hpp"
#include "keyboard.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern int textColor;

Encounter::Encounter() {
	this->speciesName = "LatestEncounter";
	this->currentGeneration = "LatestEncounter";
	this->getEncounterFile().LoadIniFile("sdmc:/3ds/PKCount/Encounter.ini");
	this->readCurrentEnc();
}


void Encounter::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -1, 0.8f, textColor, "PKCount - Encounter Screen");
	Gui::DrawStringCentered(0, 50, 0.8f, textColor, "Current Species: " + speciesName);
	Gui::DrawStringCentered(0, 85, 0.8f, textColor, "Current Generation: " + currentGeneration);
	Gui::DrawStringCentered(0, 120, 0.8f, textColor, "Current Encounter: " + std::to_string(currentEncounter));
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	Gui::DrawStringCentered(0, 0, 0.8f, textColor, "Hold <Select> for Instructions.");
	GFX::DrawSprite(sprites_plus_idx, mainButtons[0].x, mainButtons[0].y);
	GFX::DrawSprite(sprites_minus_idx, mainButtons[1].x, mainButtons[1].y);
	GFX::DrawSprite(sprites_reset_idx, mainButtons[2].x, mainButtons[2].y);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void Encounter::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_START) {
		if (Msg::promptMsg("Would you like to switch your Encounter?", "Yes", "No")) {
			// Save Current Encounter before switching.
			this->getEncounterFile().SetInt(this->currentGeneration, this->speciesName, this->currentEncounter);
			this->currentGeneration = Input::getString("Enter the current Generation.");
			this->speciesName = Input::getString("Enter the current Species name.");
			this->readCurrentEnc();
		}
	}

	if (hDown & KEY_X) {
		this->saveCurrentEnc();
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox("<A>: Set Encounter manually.\n\n<START>: Save and switch Encounter.\n\n<X>: Save current Encounter.\n\n<B>: Screen Selection.\n\n<Y>: Create New Encounter.\n\n<Left/Right/Touch>: increase / decrease Encounter.\n");
	}

	if (hDown & KEY_A) {
		std::string currentEncounter2 = Input::Numpad("Enter the current Encounter.");
		this->currentEncounter = std::stoi(currentEncounter2);
	}

	if (hDown & KEY_Y) {
		if (Msg::promptMsg("Would you like to create a new Encounter?", "Yes", "No")) {
			this->createNewEnc();
		}
	}

	if (hDown & KEY_RIGHT || hDown & KEY_R) {
		playCount();
		this->currentEncounter++;
	}

	if (hDown & KEY_LEFT || hDown & KEY_L) {
		playCount();
		if (this->currentEncounter > 0)	this->currentEncounter--;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			playCount();
			this->currentEncounter++;
		} else if (touching(touch, mainButtons[1])) {
			playCount();
			if (this->currentEncounter > 0)	this->currentEncounter--;
		} else if (touching(touch, mainButtons[2])) {
			playCount();
			this->currentEncounter = 0;
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}
}



void Encounter::createNewEnc() {
	this->currentGeneration = Input::getString("Enter the current Generation.");
	this->speciesName = Input::getString("Enter the current Species name.");
	this->currentEncounter = encounterIni.GetInt(this->currentGeneration, this->speciesName, 0);
	this->getEncounterFile().SaveIniFile("sdmc:/3ds/PKCount/Encounter.ini");
}

void Encounter::readCurrentEnc() {
	this->currentEncounter = encounterIni.GetInt(this->currentGeneration, this->speciesName, this->currentEncounter);
}

void Encounter::saveCurrentEnc() {
	if (Msg::promptMsg("Would you like to save to the current Encounter\nor create a new Encounter?", "Current Encounter", "New Encounter")) {
		this->getEncounterFile().SetInt(this->currentGeneration, this->speciesName, this->currentEncounter);
	} else {
		this->currentGeneration = Input::getString("Enter the current Generation.");
		this->speciesName = Input::getString("Enter the current Species name.");
		this->getEncounterFile().SetInt(this->currentGeneration, this->speciesName, this->currentEncounter);
	}

	this->getEncounterFile().SaveIniFile("sdmc:/3ds/PKCount/Encounter.ini");
}