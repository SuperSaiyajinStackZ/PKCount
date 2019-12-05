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

#include "screens/screenSelection.hpp"
#include "screens/startScreen.hpp"

extern int fadealpha;

void StartScreen::Draw(void) const
{
	Gui::set_screen(Top);
	Gui::sprite(sprites_dev_by_idx, 0, 0);
    Gui::DrawString(240, 200, 0.6, BLACK, "StackZ 2019-2020");
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect
	Gui::set_screen(Bottom);
    Gui::Draw_Rect(0, 0, 320, 240, WHITE);
	Gui::sprite(sprites_banner_idx, 35, 40);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect
}


void StartScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	gspWaitForVBlank();
	if(delay > 0) {
		delay--;
	} else {
		isStartup = !isStartup;
	}

    if (isStartup == false) {
		Gui::screenIsFade(std::make_unique<ScreenSelection>());
    }
}