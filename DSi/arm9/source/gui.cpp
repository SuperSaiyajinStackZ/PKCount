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
#include "gui.hpp"

#include "screens/screenCommon.hpp"

#include <stack>


std::stack<std::unique_ptr<Screen>> screens;

void Gui::mainLoop(u16 hDown, touchPosition touch) {
	screens.top()->Draw();
	screens.top()->Logic(hDown, touch);
}

void Gui::setScreen(std::unique_ptr<Screen> screen)
{
	screens.push(std::move(screen));
}

void Gui::screenBack()
{
	screens.pop();
}


// Display a Message, which needs to be confirmed with A/B.
bool Gui::promptMsg(std::string promptMsg, std::string AOption, std::string BOption)
{
	Gui::DrawTop();
	printTextCentered(promptMsg, 0, 80, true);
	printTextCenteredMaxW("<A>: " + AOption + "   <B>: " + BOption, 250, 1, 0, 140, true);
	Gui::DrawBottom();
	while(1)
	{
		swiWaitForVBlank();
		scanKeys();
		if(keysDown() & KEY_A) {
			return true;
		} else if(keysDown() & KEY_B) {
			return false;
		}
	}
}

// Basic GUI Stuff.

void Gui::DrawTop(void)
{
	drawRectangle(0, 20, 256, 152, DARKER_GRAY, true);
	drawRectangle(0, 0, 256, 20, RGB::BLUE, true);
	drawRectangle(0, 172, 256, 20, RGB::BLUE, true);
}

void Gui::DrawBottom(void)
{
	drawRectangle(0, 20, 256, 152, DARKER_GRAY, false);
	drawRectangle(0, 0, 256, 20, RGB::BLUE, false);
	drawRectangle(0, 172, 256, 20, RGB::BLUE, false);
}

void Gui::DrawBox(std::string input) {
	Gui::DrawTop();
	Gui::DrawBottom();
	printText(input, 0, 25, true);
}