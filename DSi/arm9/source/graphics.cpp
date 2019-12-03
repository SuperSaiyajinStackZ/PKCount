#include "graphics.hpp"
#include "lodepng.hpp"

#define WHITE 0xFFFF

std::vector<char> fontTiles;
std::vector<char> fontWidths;
std::vector<u16> fontMap;
u16 tileSize, tileWidth, tileHeight;

int getCharIndex(char16_t c) {
	// Try a binary search
	int left = 0;
	int mid = 0;
	int right = fontMap.size();

	while(left <= right) {
		mid = left + ((right - left) / 2);
		if(fontMap[mid] == c) {
			return mid;
			break;
		}

		if(fontMap[mid] < c) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}

	// If that doesn't find the char, do a linear search
	for(unsigned int i=0;i<fontMap.size();i++) {
		if(fontMap[i] == c)	return i;
	}
	return 0;
}

void initGraphics(void) {
	// Initialize video mode
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);

	// initialize all the VRAM banks
	vramSetPrimaryBanks(VRAM_A_MAIN_BG,
						VRAM_B_MAIN_SPRITE,
						VRAM_C_SUB_BG,
						VRAM_D_SUB_SPRITE);

	// Init oam with 1D mapping 128 byte boundary and no external palette support
	oamInit(&oamSub, SpriteMapping_Bmp_1D_128, false);
	oamInit(&oamMain, SpriteMapping_Bmp_1D_128, false);

	// Init for background
	REG_BG3CNT = BG_MAP_BASE(0) | BG_BMP16_256x256 | BG_PRIORITY(3);
	REG_BG3X = 0;
	REG_BG3Y = 0;
	REG_BG3PA = 1<<8;
	REG_BG3PB = 0;
	REG_BG3PC = 0;
	REG_BG3PD = 1<<8;

	REG_BG3CNT_SUB = BG_MAP_BASE(0) | BG_BMP16_256x256 | BG_PRIORITY(3);
	REG_BG3X_SUB = 0;
	REG_BG3Y_SUB = 0;
	REG_BG3PA_SUB = 1<<8;
	REG_BG3PB_SUB = 0;
	REG_BG3PC_SUB = 0;
	REG_BG3PD_SUB = 1<<8;

	// Set main background as target for sprite transparency
	REG_BLDCNT = 1<<11;
	REG_BLDCNT_SUB = 1<<11;
}

void loadFont(void) {
	FILE* font = fopen("nitro:/graphics/font.nftr", "rb");

	// Get file size
	fseek(font, 0, SEEK_END);
	u32 fileSize = ftell(font);

	// Load font info
	fseek(font, 0x30, SEEK_SET);
	u32 chunkSize;
	fread(&chunkSize, 4, 1, font);
	fseek(font, 0x34, SEEK_SET);
	tileWidth = fgetc(font);
	fseek(font, 0x35, SEEK_SET);
	tileHeight = fgetc(font);
	fseek(font, 0x36, SEEK_SET);
	fread(&tileSize, 2, 1, font);

	// Load character glyphs
	int tileAmount = ((chunkSize-0x10)/tileSize);
	fontTiles = std::vector<char>(tileSize*tileAmount);
	fseek(font, 0x3C, SEEK_SET);
	fread(fontTiles.data(), tileSize, tileAmount, font);

	// Fix top row
	for(int i=0;i<tileAmount;i++) {
		fontTiles[i*tileSize] = 0;
		fontTiles[i*tileSize+1] = 0;
		fontTiles[i*tileSize+2] = 0;
	}

	// Load character widths
	fseek(font, 0x24, SEEK_SET);
	u32 locHDWC;
	fread(&locHDWC, 4, 1, font);
	fseek(font, locHDWC-4, SEEK_SET);
	fread(&chunkSize, 4, 1, font);
	fseek(font, 8, SEEK_CUR);
	fontWidths = std::vector<char>(3*tileAmount);
	fread(fontWidths.data(), 3, tileAmount, font);

	// Load character maps
	fontMap = std::vector<u16>(tileAmount);
	fseek(font, 0x28, SEEK_SET);
	u32 locPAMC, mapType;
	fread(&locPAMC, 4, 1, font);

	while(locPAMC < fileSize) {
		u16 firstChar, lastChar;
		fseek(font, locPAMC, SEEK_SET);
		fread(&firstChar, 2, 1, font);
		fread(&lastChar, 2, 1, font);
		fread(&mapType, 4, 1, font);
		fread(&locPAMC, 4, 1, font);

		switch(mapType) {
			case 0: {
				u16 firstTile;
				fread(&firstTile, 2, 1, font);
				for(unsigned i=firstChar;i<=lastChar;i++) {
					fontMap[firstTile+(i-firstChar)] = i;
				}
				break;
			} case 1: {
				for(int i=firstChar;i<=lastChar;i++) {
					u16 tile;
					fread(&tile, 2, 1, font);
					fontMap[tile] = i;
				}
				break;
			} case 2: {
				u16 groupAmount;
				fread(&groupAmount, 2, 1, font);
				for(int i=0;i<groupAmount;i++) {
					u16 charNo, tileNo;
					fread(&charNo, 2, 1, font);
					fread(&tileNo, 2, 1, font);
					fontMap[tileNo] = charNo;
				}
				break;
			}
		}
	}
	fclose(font);
}

ImageData loadPng(std::string path, std::vector<u16> &imageBuffer) {
	std::vector<unsigned char> image;
	unsigned width, height;
	lodepng::decode(image, width, height, path);
	for(unsigned i=0;i<image.size()/4;i++) {
		imageBuffer.push_back(ARGB16(image[(i*4)+3], image[i*4]>>3, image[(i*4)+1]>>3, image[(i*4)+2]>>3));
	}

	ImageData imageData;
	imageData.width = width;
	imageData.height = height;
	return imageData;
}

void drawImage(int x, int y, int w, int h, std::vector<u16> &imageBuffer, bool top) {
	for(int i=0;i<h;i++) {
		for(int j=0;j<w;j++) {
			if(imageBuffer[(i*w)+j]>>15 != 0) { // Do not render transparent pixel
				(top ? BG_GFX : BG_GFX_SUB)[(y+i)*256+j+x] = imageBuffer[(i*w)+j];
			}
		}
	}
}

void drawImageScaled(int x, int y, int w, int h, double scaleX, double scaleY, std::vector<u16> &imageBuffer, bool top) {
	if(scaleX == 1 && scaleY == 1)	drawImage(x, y, w, h, imageBuffer, top);
	else {
		for(int i=0;i<(h*scaleY);i++) {
			for(int j=0;j<(w*scaleX);j++) {
				if(imageBuffer[(((int)(i/scaleY))*w)+(j/scaleX)]>>15 != 0) { // Do not render transparent pixel
					(top ? BG_GFX : BG_GFX_SUB)[(y+i)*256+x+j] = imageBuffer[(((int)(i/scaleY))*w)+(j/scaleX)];
				}
			}
		}
	}
}

void drawRectangle(int x, int y, int w, int h, int color, bool top) { drawRectangle(x, y, w, h, color, color, top); }
void drawRectangle(int x, int y, int w, int h, int color1, int color2, bool top) {
	for(int i=0;i<h;i++) {
		dmaFillHalfWords((i%2 ? color1 : color2) | BIT(15), (top ? BG_GFX : BG_GFX_SUB)+((y+i)*256+x), w*2);
	}
}

void printText(std::string text, int xPos, int yPos, bool top, bool invert) { printTextTinted(StringUtils::UTF8toUTF16(text), WHITE, xPos, yPos, top, invert); }
void printText(std::u16string text, int xPos, int yPos, bool top, bool invert) { printTextTinted(text, WHITE, xPos, yPos, top, invert); }
void printTextCentered(std::string text, int xOffset, int yPos, bool top, bool invert) { printTextCenteredTinted(StringUtils::UTF8toUTF16(text), WHITE, xOffset, yPos, top, invert); }
void printTextCentered(std::u16string text, int xOffset, int yPos, bool top, bool invert) { printTextCenteredTinted(text, WHITE, xOffset, yPos, top, invert); }
void printTextCenteredTinted(std::string text, u16 color, int xOffset, int yPos, bool top, bool invert) { printTextCenteredTinted(StringUtils::UTF8toUTF16(text), color, xOffset, yPos, top, invert); }
void printTextCenteredTinted(std::u16string text, u16 color, int xOffset, int yPos, bool top, bool invert) {
	int i = 0;
	while(text.find('\n') != text.npos) {
		printTextTinted(text.substr(0, text.find('\n')), color, ((256-getTextWidth(text.substr(0, text.find('\n'))))/2)+xOffset, yPos+(i++*16), top, invert);
		text = text.substr(text.find('\n')+1);
	}
	printTextTinted(text.substr(0, text.find('\n')), color, ((256-getTextWidth(text.substr(0, text.find('\n'))))/2)+xOffset, yPos+(i*16), top, invert);
}
void printTextTinted(std::string text, u16 color, int xPos, int yPos, bool top, bool invert) { printTextTinted(StringUtils::UTF8toUTF16(text), color, xPos, yPos, top, invert); }

void printTextTinted(std::u16string text, u16 color, int xPos, int yPos, bool top, bool invert) {
	int x=xPos;
	u16 pallet[4] = {
		0,
		(u16)(color & (invert ? 0xBDEF : 0xFBDE)),
		(u16)(color & (invert ? 0xFBDE : 0xBDEF)),
		0
	};
	for(unsigned c=0;c<text.size();c++) {
		if(text[c] == 0xA) {
			x = xPos;
			yPos += tileHeight;
			continue;
		}

		int t = getCharIndex(text[c]);
		std::vector<u16> image;
		for(int i=0;i<tileSize;i++) {
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>6 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>4 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>2 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)] & 3]);
		}

		x += fontWidths[t*3];
		if(x > 256) {
			x = xPos+fontWidths[t*3];
			yPos += tileHeight;
		}
		drawImage(x, yPos, tileWidth, tileHeight, image, top);
		x += fontWidths[(t*3)+1];
	}
}

void printTextMaxW(std::string text, double w, double scaleY, int xPos, int yPos, bool top, bool invert) { printTextTintedScaled(StringUtils::UTF8toUTF16(text), std::min(1.0, w/getTextWidth(text)), scaleY, WHITE, xPos, yPos, top, invert); }
void printTextMaxW(std::u16string text, double w, double scaleY, int xPos, int yPos, bool top, bool invert) { printTextTintedScaled(text, std::min(1.0, w/getTextWidth(text)), scaleY, WHITE, xPos, yPos, top, invert); }
void printTextCenteredMaxW(std::string text, double w, double scaleY, int xOffset, int yPos, bool top, bool invert) { printTextCenteredTintedMaxW(StringUtils::UTF8toUTF16(text), w, scaleY, WHITE, xOffset, yPos, top, invert); }
void printTextCenteredMaxW(std::u16string text, double w, double scaleY, int xOffset, int yPos, bool top, bool invert) { printTextCenteredTintedMaxW(text, w, scaleY, WHITE, xOffset, yPos, top, invert); }
void printTextCenteredTintedMaxW(std::string text, double w, double scaleY, u16 color, int xOffset, int yPos, bool top, bool invert) { printTextCenteredTintedMaxW(StringUtils::UTF8toUTF16(text), w, scaleY, color, xOffset, yPos, top, invert); }
void printTextCenteredTintedMaxW(std::u16string text, double w, double scaleY, u16 color, int xOffset, int yPos, bool top, bool invert) {
	int i = 0;
	while(text.find('\n') != text.npos) {
		printTextTintedScaled(text.substr(0, text.find('\n')), std::min(1.0, w/getTextWidth(text.substr(0, text.find('\n')))), scaleY, color, ((256-getTextWidthMaxW(text.substr(0, text.find('\n')), w))/2)+xOffset, yPos+(i++*(16*scaleY)), top, invert);
		text = text.substr(text.find('\n')+1);
	}
	printTextTintedScaled(text.substr(0, text.find('\n')), std::min(1.0, w/getTextWidth(text.substr(0, text.find('\n')))), scaleY, color, ((256-getTextWidthMaxW(text.substr(0, text.find('\n')), w))/2)+xOffset, yPos+(i*(16*scaleY)), top, invert);
}
void printTextTintedMaxW(std::string text, double w, double scaleY, u16 color, int xPos, int yPos, bool top, bool invert) { printTextTintedScaled(StringUtils::UTF8toUTF16(text), std::min(1.0, w/getTextWidth(text)), scaleY, color, xPos, yPos, top, invert); }
void printTextTintedMaxW(std::u16string text, double w,  double scaleY, u16 color, int xPos, int yPos, bool top, bool invert) { printTextTintedScaled(text, std::min(1.0, w/getTextWidth(text)), scaleY, color, xPos, yPos, top, invert); }

void printTextScaled(std::string text, double scaleX, double scaleY, int xPos, int yPos, bool top, bool invert) { printTextTintedScaled(StringUtils::UTF8toUTF16(text), scaleX, scaleY, WHITE, xPos, yPos, top, invert); }
void printTextScaled(std::u16string text, double scaleX, double scaleY, int xPos, int yPos, bool top, bool invert) { printTextTintedScaled(text, scaleX, scaleY, WHITE, xPos, yPos, top, invert); }
void printTextCenteredScaled(std::string text, double scaleX, double scaleY, int xOffset, int yPos, bool top, bool invert) { printTextCenteredTintedScaled(StringUtils::UTF8toUTF16(text), scaleX, scaleY, WHITE, xOffset, yPos, top, invert); }
void printTextCenteredScaled(std::u16string text, double scaleX, double scaleY, int xOffset, int yPos, bool top, bool invert) { printTextCenteredTintedScaled(text, scaleX, scaleY, WHITE, xOffset, yPos, top, invert); }
void printTextCenteredTintedScaled(std::string text, double scaleX, double scaleY, u16 color, int xOffset, int yPos, bool top, bool invert) { printTextCenteredTintedScaled(StringUtils::UTF8toUTF16(text), scaleX, scaleY, color, xOffset, yPos, top, invert); }
void printTextCenteredTintedScaled(std::u16string text, double scaleX, double scaleY, u16 color, int xOffset, int yPos, bool top, bool invert) {
	int i = 0;
	while(text.find('\n') != text.npos) {
		printTextTintedScaled(text.substr(0, text.find('\n')), scaleX, scaleY, color, ((256-getTextWidth(text.substr(0, text.find('\n'))))/2)+xOffset, yPos+(i++*(16*scaleY)), top, invert);
		text = text.substr(text.find('\n')+1);
	}
	printTextTintedScaled(text.substr(0, text.find('\n')), scaleX, scaleY, color, ((256-getTextWidth(text.substr(0, text.find('\n'))))/2)+xOffset, yPos+(i*(16*scaleY)), top, invert);
}
void printTextTintedScaled(std::string text, double scaleX, double scaleY, u16 color, int xPos, int yPos, bool top, bool invert) { printTextTintedScaled(StringUtils::UTF8toUTF16(text), scaleX, scaleY, color, xPos, yPos, top, invert); }

void printTextTintedScaled(std::u16string text, double scaleX, double scaleY, u16 color, int xPos, int yPos, bool top, bool invert) {
	if(scaleX == 1 && scaleY == 1) {
		printTextTinted(text, color, xPos, yPos, top, invert);
		return;
	}

	int x=xPos;
	u16 pallet[4] = {
		0,
		(u16)(color & (invert ? 0xBDEF : 0xFBDE)),
		(u16)(color & (invert ? 0xFBDE : 0xBDEF)),
		0
	};
	for(unsigned c=0;c<text.size();c++) {
		if(text[c] == 0xA) {
			x = xPos;
			yPos += tileHeight;
			continue;
		}

		int t = getCharIndex(text[c]);
		std::vector<u16> image;
		for(int i=0;i<tileSize;i++) {
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>6 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>4 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>2 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)] & 3]);
		}

		x += fontWidths[t*3];
		if(x > 256) {
			x = xPos+fontWidths[t*3];
			yPos += tileHeight;
		}
		drawImageScaled(x, yPos, tileWidth, tileHeight, scaleX, scaleY, image, top);
		x += fontWidths[(t*3)+1]*scaleX;
	}
}

int getTextWidthMaxW(std::string text, int w) { return std::min(w, getTextWidth(StringUtils::UTF8toUTF16(text))); }
int getTextWidthMaxW(std::u16string text, int w) { return std::min(w, getTextWidth(text)); }
int getTextWidthScaled(std::string text, double scale) { return getTextWidth(StringUtils::UTF8toUTF16(text))*scale; }
int getTextWidthScaled(std::u16string text, double scale) { return getTextWidth(text)*scale; }
int getTextWidth(std::string text) { return getTextWidth(StringUtils::UTF8toUTF16(text)); }
int getTextWidth(std::u16string text) {
	int textWidth = 0;
	for(unsigned c=0;c<text.size();c++) {
		textWidth += fontWidths[(getCharIndex(text[c])*3)+2];
	}
	return textWidth;
}