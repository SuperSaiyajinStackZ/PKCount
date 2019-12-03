#include "utils/sound.hpp"

#include <nds.h>
#include <maxmod9.h>

#include "soundbank.h"
#include "soundbank_bin.h"

mm_sound_effect Sound::count;

void Sound::init(void) {
	mmInitDefaultMem((mm_addr)soundbank_bin);
	
	mmLoadEffect(SFX_COUNT);

	mmSetModuleVolume(0x200);

	Sound::count = {
		{SFX_COUNT},			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		0xFF,	// volume
		0x80,	// panning
	};
}

void Sound::play(mm_sound_effect &sound) {
	mmEffectEx(&sound);
}