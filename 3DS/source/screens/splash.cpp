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

#include "screenSelection.hpp"
#include "splash.hpp"

void Splash::Draw(void) const {
	Gui::ScreenDraw(Top);
	GFX::DrawSprite(sprites_dev_by_idx, 0, 0);
	Gui::DrawString(395-Gui::GetStringWidth(0.50, "2019-2020"), 218, 0.50, WHITE, "2019-2020");
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	Gui::ScreenDraw(Bottom);
	GFX::DrawSprite(sprites_universal_core_idx, 0, 0);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void Splash::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	this->delay++;
	// If the delay is larger than 130 -> Switch screen.
	if (this->delay > 130) {
		Gui::setScreen(std::make_unique<ScreenSelection>(), true, false);
	}
}