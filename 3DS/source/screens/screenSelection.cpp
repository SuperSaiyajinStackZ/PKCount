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

#include "screens/creditsScreen.hpp"
#include "screens/encounterScreen.hpp"
#include "screens/instructions.hpp"
#include "screens/screenSelection.hpp"
#include "screens/settingsScreen.hpp"

#define MAX_SCREENS 4

extern int textColor;
extern int barColor;
extern bool exiting;
extern int fadealpha;

void ScreenSelection::Draw(void) const
{
	Gui::DrawTop();
	Gui::DrawStringCentered(0, -1, 0.8f, textColor, "Select the screen you want to enter.");

	// Draw Boxes.
	Gui::Draw_Rect(30, 50, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));
	Gui::Draw_Rect(150, 50, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));
	Gui::Draw_Rect(270, 50, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));
	Gui::Draw_Rect(30, 140, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));
	Gui::Draw_Rect(150, 140, 100, 60, barColor & C2D_Color32(255, 255, 255, 120));

	// Draw Selection.
	if (selectedScreen == 0) {
		Gui::Draw_Rect(30, 50, 100, 60, barColor);
	} else if (selectedScreen == 1) {
		Gui::Draw_Rect(150, 50, 100, 60, barColor);
	} else if (selectedScreen == 2) {
		Gui::Draw_Rect(270, 50, 100, 60, barColor);
	} else if (selectedScreen == 3) {
		Gui::Draw_Rect(30, 140, 100, 60, barColor);
	} else if (selectedScreen == 4) {
		Gui::Draw_Rect(150, 140, 100, 60, barColor);
	}

	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Encounter\n  Screen"))/2-70-50, 60, 0.6f, textColor, "Encounter\n  Screen");
	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Settings\n Screen"))/2-50+50, 60, 0.6f, textColor, "Settings\n Screen");
	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Credits\nScreen"))/2+70+50, 60, 0.6f, textColor, "Credits\nScreen");

	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Instructions\n   Screen"))/2-70-50, 150, 0.6f, textColor, "Instructions\n   Screen");
	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Exit App"))/2-50+50, 160, 0.6f, textColor, "Exit App");
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect
	Gui::DrawBottom();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect
}


void ScreenSelection::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_A) {
		if (selectedScreen == 0) {
			Screen::set(std::make_unique<Encounter>());
		} else if (selectedScreen == 1) {
			Screen::set(std::make_unique<Settings>());
		} else if (selectedScreen == 2) {
			Screen::set(std::make_unique<Credits>());
 		} else if (selectedScreen == 3) {
			Screen::set(std::make_unique<Instructions>());
		} else if (selectedScreen == 4) {
			exiting = true;
		}
	}

	if (hDown & KEY_RIGHT || hDown & KEY_R) {
		if (selectedScreen < MAX_SCREENS)	selectedScreen++;
	}

	if (hDown & KEY_UP) {
		if (selectedScreen == 3) {
			selectedScreen = 0;
		} else if (selectedScreen == 4) {
			selectedScreen = 1;
		}
	}

	if (hDown & KEY_DOWN) {
		if (selectedScreen == 0) {
			selectedScreen = 3;
		} else if (selectedScreen == 1) {
			selectedScreen = 4;
		}
	}

	if (hDown & KEY_LEFT || hDown & KEY_L) {
		if (selectedScreen > 0)	selectedScreen--;
	}
}