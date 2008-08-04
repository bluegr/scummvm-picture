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
 * $URL$
 * $Id$
 *
 */

#include "common/events.h"
#include "common/keyboard.h"
#include "common/file.h"
#include "common/savefile.h"
#include "common/config-manager.h"

#include "base/plugins.h"
#include "base/version.h"

#include "sound/mixer.h"

#include "picture/picture.h"
#include "picture/animation.h"
#include "picture/screen.h"

namespace Picture {

AnimationPlayer::AnimationPlayer(PictureEngine *vm) : _vm(vm) {
    _animBuffer = new byte[262144];
}

AnimationPlayer::~AnimationPlayer() {
	delete[] _animBuffer;
}

void AnimationPlayer::start(uint resIndex) {
	debug(1, "AnimationPlayer::start(%d)", resIndex);

	_resIndex = resIndex;

	_vm->_arc->openResource(_resIndex);
	_height = _vm->_arc->readUint16LE();
	_width = _vm->_arc->readUint16LE();
	_frameCount = _vm->_arc->readUint16LE();
	_vm->_arc->read(_palette, 768);
	_curFrameSize = _vm->_arc->readUint32LE();
	_nextFrameOffset = _curFrameSize + 782;
	_vm->_arc->read(_animBuffer, _curFrameSize);
	_nextFrameSize = _vm->_arc->readUint32LE();
	_vm->_arc->closeResource();

	debug(1, "AnimationPlayer::start() width = %d; height = %d; frameCount = %d", _width, _height, _frameCount);

    unpackFrame();

	_keepFrameCounter = 0;
	_frameNumber = 0;
	// TODO mov screenFlag01, 0FFFFh
	// TODO mov animDrawFrameFlag, 0FFFFh
	
	_firstNextFrameOffset = _nextFrameOffset;
	_firstCurFrameSize = _curFrameSize;
	_firstNextFrameSize = _nextFrameSize;

}

void AnimationPlayer::nextFrame() {
	debug(1, "AnimationPlayer::nextFrame()");

	if (_frameNumber == _frameCount) {
		_nextFrameOffset = _firstNextFrameOffset;
		_curFrameSize = _firstCurFrameSize;
		_nextFrameSize = _firstNextFrameSize;
		_frameNumber = 1;
	} else {
		_frameNumber++;
	}
	
	debug(1, "AnimationPlayer::nextFrame() frameNumber = %d", _frameNumber);

	if (_keepFrameCounter > 0) {
	    _keepFrameCounter--;
	    return;
	}
	
	_vm->_arc->openResource(_resIndex);
	_vm->_arc->seek(_nextFrameOffset, SEEK_CUR);
	_curFrameSize = _nextFrameSize;
	
	if (_curFrameSize == 0)
	    _curFrameSize = 1;
	
	_vm->_arc->read(_animBuffer, _curFrameSize);
	_nextFrameSize = _vm->_arc->readUint32LE();
	_nextFrameOffset += _curFrameSize + 4;
	
	if (_curFrameSize > 1) {
        unpackFrame();
        // TODO mov animDrawFrameFlag, 0FFFFh
	} else {
	    _keepFrameCounter = _animBuffer[0] - 1;
	    // TODO mov animDrawFrameFlag, 0
	}

	_vm->_arc->closeResource();


}

int16 AnimationPlayer::getStatus() {
	debug(1, "AnimationPlayer::getStatus()");
	int16 status = -1;
	if (_frameNumber == _frameCount)
	    status = 0;
	else if (_frameNumber == _frameCount - 1)
	    status = 1;
	debug(1, "AnimationPlayer::getStatus() status = %d", status);
	return status;
}

void AnimationPlayer::unpackFrame() {
    _vm->_screen->unpackRle(_animBuffer, _vm->_screen->_frontScreen, _width, _height);
    _vm->_screen->unpackRle(_animBuffer, _vm->_screen->_backScreen, _width, _height);
}

} // End of namespace Picture
