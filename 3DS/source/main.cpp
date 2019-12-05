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

#include "screens/encounterScreen.hpp"
#include "screens/screenCommon.hpp"
#include "screens/screenSelection.hpp"
#include "screens/startScreen.hpp"

#include "utils/inifile.h"
#include "utils/sound.h"
#include "utils/structs.hpp"

#include <3ds.h>
#include <dirent.h>
#include <unistd.h>

bool dspfirmfound = false;
bool musicExist = false;
bool musicPlays = false;
bool exiting = false;
// Fade stuff.
int fadealpha = 255;
bool fadein = true;


// Settings stuff.
CIniFile settingsIni("sdmc:/3ds/PKCount/Settings.ini");
int barColor;
int bgColor;
int textColor;
int musicMode;
int startMode;

void loadSettings() {
	barColor = settingsIni.GetInt("Settings", "BarColor", RGBA8(0, 120, 255, 255));
	bgColor = settingsIni.GetInt("Settings", "BGColor", RGBA8(120, 120, 120, 255));
	textColor = settingsIni.GetInt("Settings", "TextColor", WHITE);
	musicMode = settingsIni.GetInt("Settings", "MusicMode", 1);
	startMode = settingsIni.GetInt("Settings", "StartMode", 1);
}

void saveSettings() {
	settingsIni.SetInt("Settings", "BarColor", barColor);
	settingsIni.SetInt("Settings", "BGColor", bgColor);
	settingsIni.SetInt("Settings", "TextColor", textColor);
	settingsIni.SetInt("Settings", "MusicMode", musicMode);
	settingsIni.SetInt("Settings", "StartMode", startMode);
	settingsIni.SaveIniFile("sdmc:/3ds/PKCount/Settings.ini");
}


//Music and sound effects.
sound *sfx_count = NULL;
sound *music = NULL;

void loadSoundEffects(void) {
	if( access( "sdmc:/3ds/PKCount/music.wav", F_OK ) != -1 ) {
		music = new sound("sdmc:/3ds/PKCount/music.wav", 1, true);
		musicExist = true;
	}
	sfx_count = new sound("romfs:/count.wav", 2, false);
}

touchPosition touch;

bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}

int main()
{
	gfxInitDefault();
	romfsInit();
	sdmcInit();
	Gui::init();
	cfguInit();
	loadSettings();
	if (startMode == 0) {
		Gui::setScreen(std::make_unique<ScreenSelection>());
	} else {
		Gui::setScreen(std::make_unique<StartScreen>());
	}

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users


	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);
	mkdir("sdmc:/3ds/PKCount", 0777); // main Path.

 	if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
		loadSoundEffects();
	 }

	// Loop as long as the status is not exit
	while (aptMainLoop() && !exiting)
	{
		hidScanInput();
		u32 hHeld = hidKeysHeld();
		u32 hDown = hidKeysDown();
		hidTouchRead(&touch);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, BLACK);
		C2D_TargetClear(Bottom, BLACK);
		Gui::clearTextBufs();
		Gui::mainLoop(hDown, hHeld, touch);
		C3D_FrameEnd(0);

		if (dspfirmfound == true && musicExist == true && musicMode == 1) {
			music->play();
			musicPlays = true;
		}

		if (dspfirmfound == true && musicExist == true && musicMode == 0 && musicPlays == true) {
			music->stop();
			musicPlays = false;
		}

		if (hDown & KEY_B) {
			Gui::setScreen(std::make_unique<ScreenSelection>());
		}


		if (fadein == true) {
			fadealpha -= 6;
			if (fadealpha < 0) {
				fadealpha = 0;
				fadein = false;
			}
		}
	}

	delete sfx_count;

	if (musicPlays == true) {
		music->stop();
	}

	if (dspfirmfound == true && musicExist == true) {
		ndspExit();
	}

	delete music;
	saveSettings();
	Gui::exit();
	gfxExit();
	cfguExit();
	romfsExit();
	sdmcExit();
	return 0;
}