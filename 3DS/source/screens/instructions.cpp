/*
*   This file is part of PKCount
*   Copyright (C) 2019-2020 SuperSaiyajinStackZ
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

#include "instructions.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern int textColor;

void Instructions::DrawInst(void) const {
	if (currentInstruction == 1) {
		Gui::DrawStringCentered(0, -1, 0.8f, textColor, "Instructions - EncounterScreen");
		Gui::DrawString(10, 30, 0.5f, textColor, "This is the MainScreen of this app. With this you can\nCount your Encounters for your Shiny Pokémon\nAnd save it afterward to your SD Card.\n\nTHIS IS A WORK IN PROGRESS!");
	}
}


void Instructions::Draw(void) const {
	GFX::DrawTop();
	DrawInst();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void Instructions::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}
}