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

#ifndef KYRA_WSAMOVIE_H
#define KYRA_WSAMOVIE_H

namespace Audio {
class AppendableAudioStream;
class SoundHandle;
} // end of namespace Audio

namespace Kyra {
class KyraEngine;
class Screen_v2;

class Movie {
public:
	Movie(KyraEngine *vm) : _vm(vm), _opened(false),  _x(-1), _y(-1), _drawPage(-1) {}
	virtual ~Movie() {}

	virtual bool opened() { return _opened; }

	virtual int open(const char *filename, int offscreen, uint8 *palette) = 0;
	virtual void close() = 0;

	virtual int frames() = 0;

	virtual void displayFrame(int frameNum, ...) = 0;

	virtual void setX(int x) { _x = x; }
	virtual void setY(int y) { _y = y; }
	virtual void setDrawPage(int page) { _drawPage = page; }
protected:
	KyraEngine *_vm;
	bool _opened;

	int _x, _y;
	int _drawPage;
};

class WSAMovie_v1 : public Movie {
public:
	WSAMovie_v1(KyraEngine *vm);
	virtual ~WSAMovie_v1();

	virtual int open(const char *filename, int offscreen, uint8 *palette);
	virtual void close();

	virtual int frames() { return _opened ? _numFrames : -1; }

	virtual void displayFrame(int frameNum, ...);

	enum WSAFlags {
		WF_OFFSCREEN_DECODE = 0x10,
		WF_NO_FIRST_FRAME = 0x40,
		WF_HAS_PALETTE = 0x100,
		WF_XOR = 0x200
	};

protected:
	virtual void processFrame(int frameNum, uint8 *dst);

	uint16 _currentFrame;
	uint16 _numFrames;
	uint16 _width;
	uint16 _height;
	uint16 _flags;
	uint8 *_deltaBuffer;
	uint32 _deltaBufferSize;
	uint8 *_offscreenBuffer;
	uint32 *_frameOffsTable;
	uint8 *_frameData;
};

class WSAMovieAmiga : public WSAMovie_v1 {
public:
	WSAMovieAmiga(KyraEngine *vm);
	int open(const char *filename, int offscreen, uint8 *palette);
	void close();

	void displayFrame(int frameNum, ...);
private:
	void processFrame(int frameNum, uint8 *dst);

	uint8 *_buffer;
};

class WSAMovie_v2 : public WSAMovie_v1 {
public:
	WSAMovie_v2(KyraEngine *vm, Screen_v2 *scren);

	int open(const char *filename, int unk1, uint8 *palette);

	virtual void displayFrame(int frameNum, ...);

	void setX(int x) { _x = x + _xAdd; }
	void setY(int y) { _y = y + _yAdd; }

	int xAdd() const { return _xAdd; }
	int yAdd() const { return _yAdd; }

	int width() const { return _width; }
	int height() const { return _height; }

	void setWidth(int w) { _width = w; }
	void setHeight(int h) { _height = h; }
protected:
	Screen_v2 *_screen;

	int16 _xAdd;
	int16 _yAdd;
};

} // end of namespace Kyra

#endif


