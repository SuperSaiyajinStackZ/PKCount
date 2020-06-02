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

#include "creditsScreen.hpp"
#include "encounterScreen.hpp"
#include "instructions.hpp"
#include "screenSelection.hpp"
#include "settingsScreen.hpp"

#define MAX_SCREENS 4

extern int textColor;
extern int barColor;
extern bool exiting;

void ScreenSelection::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -1, 0.8f, textColor, "Select the screen you want to enter.");

	// Draw Boxes.
	Gui::Draw_Rect(30, 50, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));
	Gui::Draw_Rect(150, 50, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));
	Gui::Draw_Rect(270, 50, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));
	Gui::Draw_Rect(30, 140, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));
	Gui::Draw_Rect(150, 140, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));

	// Draw Selection.
	if (this->selectedScreen == 0) {
		Gui::Draw_Rect(30, 50, 100, 60, barColor);
	} else if (this->selectedScreen == 1) {
		Gui::Draw_Rect(150, 50, 100, 60, barColor);
	} else if (this->selectedScreen == 2) {
		Gui::Draw_Rect(270, 50, 100, 60, barColor);
	} else if (this->selectedScreen == 3) {
		Gui::Draw_Rect(30, 140, 100, 60, barColor);
	} else if (this->selectedScreen == 4) {
		Gui::Draw_Rect(150, 140, 100, 60, barColor);
	}

	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Encounter\n  Screen"))/2-70-50, 60, 0.6f, textColor, "Encounter\n  Screen");
	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Settings\n Screen"))/2-50+50, 60, 0.6f, textColor, "Settings\n Screen");
	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Credits\nScreen"))/2+70+50, 60, 0.6f, textColor, "Credits\nScreen");

	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Instructions\n   Screen"))/2-70-50, 150, 0.6f, textColor, "Instructions\n   Screen");
	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Exit App"))/2-50+50, 160, 0.6f, textColor, "Exit App");
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void ScreenSelection::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_A) {
		if (this->selectedScreen == 0) {
			Gui::setScreen(std::make_unique<Encounter>(), true, true);
		} else if (this->selectedScreen == 1) {
			Gui::setScreen(std::make_unique<Settings>(), true, true);
		} else if (this->selectedScreen == 2) {
			Gui::setScreen(std::make_unique<Credits>(), true, true);
		} else if (this->selectedScreen == 3) {
			 Msg::DrawWarnMsg("This Feature is not implemented yet.");
		} else if (this->selectedScreen == 4) {
			fadecolor = 0;
			fadeout = true;
			exiting = true;
		}
	}

	if (hDown & KEY_RIGHT || hDown & KEY_R) {
		if (this->selectedScreen < MAX_SCREENS)	this->selectedScreen++;
	}

	if (hDown & KEY_UP) {
		if (this->selectedScreen == 3) {
			this->selectedScreen = 0;
		} else if (this->selectedScreen == 4) {
			this->selectedScreen = 1;
		}
	}

	if (hDown & KEY_DOWN) {
		if (this->selectedScreen == 0) {
			this->selectedScreen = 3;
		} else if (this->selectedScreen == 1) {
			this->selectedScreen = 4;
		}
	}

	if (hDown & KEY_LEFT || hDown & KEY_L) {
		if (this->selectedScreen > 0)	this->selectedScreen--;
	}

	// Allow exiting with START here as well.
	if (hDown & KEY_START) {
		fadecolor = 0;
		fadeout = true;
		exiting = true;
	}
}