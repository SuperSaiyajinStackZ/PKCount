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

#ifndef _PKCOUNT_COLORS_HPP
#define _PKCOUNT_COLORS_HPP

#include <citro2d.h>
#include <sstream>
#include <string>

#define RGBA8(r, g, b, a) ((((r)&0xFF)<<0) | (((g)&0xFF)<<8) | (((b)&0xFF)<<16) | (((a)&0xFF)<<24))

#define BLACK 			C2D_Color32(0, 0, 0, 255)
#define WHITE 			C2D_Color32(255, 255, 255, 255)
#define RED				C2D_Color32(255, 0, 0, 255)
#define GREEN			C2D_Color32(0, 255, 0, 255)
#define BLUE			C2D_Color32(0, 0, 255, 255)

namespace ColorHelper {
	int getColorValue(int color, int bgr);
	std::string getColorName(int color, int bgr);
}

typedef u32 Color;

#endif