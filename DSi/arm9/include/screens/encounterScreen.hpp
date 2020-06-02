/*
*   This file is part of PKCount
*   Copyright (C) 2019-2020 Stack-Team
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

#ifndef ENCOUNTERSCREEN_HPP
#define ENCOUNTERSCREEN_HPP

#include "screenCommon.hpp"
#include "structs.hpp"

#include <vector>

class Encounter : public Screen {
public:
	void Draw(void) const override;
	void Logic(u16 hDown, touchPosition touch) override;
	Encounter();
private:
	void DrawCredits(void) const;
	int screenMode = 0;
	mutable bool screenDrawn = false;
	int currentEncounter = 0;
	void readCurrentEnc();
	void saveCurrentEnc();
	void createNewEnc();

	// Current Encounter name.
	std::string speciesName = "";
	std::string currentGeneration = "";

	std::vector<Structs::ButtonPos> mainButtons = {		
		{60, 30, 64, 64}, // Plus.
		{150, 30, 64, 64}, // Minus.
		{180, 105, 64, 64}, // Reset.
		{0, 172, 256, 20} // Bar.
	};
};

#endif