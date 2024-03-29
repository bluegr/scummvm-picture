/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://www.switchlink.se/svn/picture/script.h $
 * $Id: script.h 43 2008-09-23 10:57:39Z johndoe $
 *
 */

#ifndef PICTURE_SOUND_H
#define PICTURE_SOUND_H

#include "audio/mixer.h"	// for Audio::SoundHandle

#include "picture/picture.h"

namespace Picture {

// 0x1219

enum SoundChannelType {
	kChannelTypeEmpty		= 0,
	kChannelTypeBackground	= -1,
	kChannelTypeSfx			= -2,
	kChannelTypeSpeech		= -3
};

struct SoundChannel {
	int16 resIndex;
	int16 type;
	Audio::SoundHandle handle;
};

const int kMaxChannels = 4;

class Sound {
public:
	Sound(PictureEngine *vm);
	~Sound();

	void playSpeech(int16 resIndex);
	void playSound(int16 resIndex, int16 type, int16 volume);
	void playSoundAtPos(int16 resIndex, int16 x, int16 y);
	void updateSpeech();
	void stopSpeech();
	void stopAll();

protected:
	PictureEngine *_vm;

	SoundChannel channels[kMaxChannels];

	void internalPlaySound(int16 resIndex, int16 type, int16 volume, int16 panning);

};


} // End of namespace Picture

#endif /* PICTURE_SOUND_H */
