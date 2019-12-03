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

#include "screens/settingsScreen.hpp"

#include "utils/inifile.h"
#include "utils/keyboard.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);


extern int barColor;
extern int bgColor;
extern int textColor;
extern int musicMode;

void Settings::Draw(void) const
{
	Gui::DrawTop();
	Gui::DrawStringCentered(0, -1, 0.8f, textColor, "PKCount - Settings");
	Gui::DrawBottom();

	// Draw RGB Buttons.
	if (currentSetting != 3) {
		Gui::Draw_Rect(buttons[0].x, buttons[0].y, 95, 41, C2D_Color32(255, 0, 0, 255));
		Gui::Draw_Rect(buttons[1].x, buttons[1].y, 95, 41, C2D_Color32(0, 255, 0, 255));
		Gui::Draw_Rect(buttons[2].x, buttons[2].y, 95, 41, C2D_Color32(0, 0, 255, 255));
	}

	for (int i = 0; i < 4; i++) {
		if (currentSetting == i) {
			Gui::Draw_Rect(25 + i * 25, 5, 16, 16, C2D_Color32(140, 140, 140, 255));
		}
	}

	Gui::DrawString(29 + 0 * 25, 5, 0.5f, WHITE, "1", 400);
	Gui::DrawString(29 + 1 * 25, 5, 0.5f, WHITE, "2", 400);
	Gui::DrawString(29 + 2 * 25, 5, 0.5f, WHITE, "3", 400);
	Gui::DrawString(29 + 3 * 25, 5, 0.5f, WHITE, "4", 400);

	if (currentSetting == 0) {
		Gui::DrawStringCentered(0, 60, 0.7f, textColor, "Bar Color", 320);
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(barColor, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(barColor, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(barColor, 0).c_str(), 400);
	} else if (currentSetting == 1) {
		Gui::DrawStringCentered(0, 60, 0.7f, textColor, "Background Color", 320);
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(bgColor, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(bgColor, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(bgColor, 0).c_str(), 400);
	} else if (currentSetting == 2) {
		Gui::DrawStringCentered(0, 60, 0.7f, textColor, "TextColor", 320);
		Gui::DrawString(40, 98, 0.7f, WHITE, ColorHelper::getColorName(textColor, 2).c_str(), 400);
		Gui::DrawString(140, 98, 0.7f, WHITE, ColorHelper::getColorName(textColor, 1).c_str(), 400);
		Gui::DrawString(245, 98, 0.7f, WHITE, ColorHelper::getColorName(textColor, 0).c_str(), 400);
	}

	if (currentSetting == 3) {
		Gui::DrawStringCentered(0, 60, 0.7f, textColor, "Use of Music", 320);
		if (musicMode == 1) {
			Gui::Draw_Rect(buttons[1].x, buttons[1].y, 95, 41, C2D_Color32(0, 150, 0, 255));
			Gui::DrawString(140, 98, 0.7f, WHITE, "Yes", 400);
		} else if (musicMode == 0) {
			Gui::Draw_Rect(buttons[1].x, buttons[1].y, 95, 41, C2D_Color32(150, 0, 0, 255));
			Gui::DrawString(140, 98, 0.7f, WHITE, "No", 400);
		}
	}
}






void Settings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	int red;
	int green;
	int blue;

	if (hDown & KEY_L || hDown & KEY_LEFT) {
		if(currentSetting > 0)	currentSetting--;
	}

	if (hDown & KEY_R || hDown & KEY_RIGHT) {
		if(currentSetting < 3)	currentSetting++;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, buttons[0])) {
			int temp = Input::getUint(255, "Enter the Red RGB Value.");
			if(temp != -1) {
				red = temp;
				if (currentSetting == 0) {
					barColor = RGBA8(red, ColorHelper::getColorValue(barColor, 1), ColorHelper::getColorValue(barColor, 0), 255);
				} else if (currentSetting == 1) {
					bgColor = RGBA8(red, ColorHelper::getColorValue(bgColor, 1), ColorHelper::getColorValue(bgColor, 0), 255);
				} else if (currentSetting == 2) {
					textColor = RGBA8(red, ColorHelper::getColorValue(textColor, 1), ColorHelper::getColorValue(textColor, 0), 255);
				}
			}

		} else if (touching(touch, buttons[1])) {
			if (currentSetting != 3) {
				int temp = Input::getUint(255, "Enter the Green RGB Value.");
				if(temp != -1) {
					green = temp;
					if (currentSetting == 0) {
							barColor = RGBA8(ColorHelper::getColorValue(barColor, 2), green, ColorHelper::getColorValue(barColor, 0), 255);
					} else if (currentSetting == 1) {
						bgColor = RGBA8(ColorHelper::getColorValue(bgColor, 2), green, ColorHelper::getColorValue(bgColor, 0), 255);
						} else if (currentSetting == 2) {
						textColor = RGBA8(ColorHelper::getColorValue(textColor, 2), green, ColorHelper::getColorValue(textColor, 0), 255);
					}
				}
			} else {
				if (musicMode == 0) {
					musicMode = 1;
				} else if (musicMode == 1) {
					musicMode = 0;
				}
			}

		} else if (touching(touch, buttons[2])) {
			int temp = Input::getUint(255, "Enter the Blue RGB Value.");
			if(temp != -1) {
				blue = temp;
				if (currentSetting == 0) {
					barColor = RGBA8(ColorHelper::getColorValue(barColor, 2), ColorHelper::getColorValue(barColor, 1), blue, 255);
				} else if (currentSetting == 1) {
					bgColor = RGBA8(ColorHelper::getColorValue(bgColor, 2), ColorHelper::getColorValue(bgColor, 1), blue, 255);
				} else if (currentSetting == 2) {
					textColor = RGBA8(ColorHelper::getColorValue(textColor, 2), ColorHelper::getColorValue(textColor, 1), blue, 255);
				}
			}
		}
	}
}