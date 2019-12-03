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

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern int textColor;

void Credits::Draw(void) const
{
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, textColor, "PKCount - Credits");
	Gui::sprite(sprites_stackZ_idx, 0, 45);
	Gui::DrawString(160, 65, 0.6f, textColor, "Hello there! I'm StackZ.\nI'm the Developer of this App. \nI hope you enjoy it!\nSee ya, StackZ.");
	Gui::DrawBottom();
}


void Credits::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
}