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

#ifndef GUI_HPP
#define GUI_HPP

#include "colors.hpp"
#include "sprites.h"

#include "screens/screen.hpp"

namespace Gui
{
	Result init(void);
	void exit(void);

	void setDraw(C3D_RenderTarget * screen);
	C3D_RenderTarget* target(gfxScreen_t t);

	// Clear Text.
	void clearTextBufs(void);

	// Draw Sprites from the Sheets.
	void sprite(int key, int x, int y, float ScaleX = 1.0, float ScaleY = 1.0);

	// Misc.
	bool Draw_Rect(float x, float y, float w, float h, u32 color);
	bool promptMsg(std::string promptMsg, std::string AOption, std::string BOption);
	void HelperBox(std::string Msg);

	// New Text / String Functions.
	void DrawStringCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth = 400);
	void DrawString(float x, float y, float size, u32 color, std::string Text, int maxWidth = 400);
	void GetStringSize(float size, float *width, float *height, std::string Text);
	float GetStringWidth(float size, std::string Text);
	float GetStringHeight(float size, std::string Text);

	// GUI.
	void DrawTop(void);
	void DrawBottom(void);
}

#endif