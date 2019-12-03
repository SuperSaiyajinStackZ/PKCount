#ifndef SOUND_HPP
#define SOUND_HPP
#include <maxmod9.h>

namespace Sound {
	extern mm_sound_effect count;
	void init(void);
	void play(mm_sound_effect &sound);
}


#endif
