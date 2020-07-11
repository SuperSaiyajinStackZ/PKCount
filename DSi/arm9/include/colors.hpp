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

#ifndef COLORS_HPP
#define COLORS_HPP

// Grayscale
inline int WHITE = 0xFFFF;
inline int LIGHT_GRAY = 0xE739;
inline int GRAY = 0xCE73;
inline int DARK_GRAY = 0x98C6;
inline int DARKER_GRAY = 0x94A5;
inline int DARKERER_GRAY = 0x8842;
inline int BLACK = 0x8000;

// BGR
namespace BGR {
	inline int DARK_BLUE = 0x800C;
	inline int LIGHT_BLUE = 0x8019;
	inline int TEAL = 0x833f;
}

// RGB
namespace RGB {
	inline int BLUE = 0xFC00;
}

#endif
