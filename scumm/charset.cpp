/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001-2003 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 */

#include "stdafx.h"
#include "charset.h"
#include "scumm.h"
#include "nut_renderer.h"

void CharsetRendererCommon::setCurID(byte id) {
	_vm->checkRange(_vm->_maxCharsets - 1, 0, _curId, "Printing with bad charset %d");

	_curId = id;

	_fontPtr = _vm->getResourceAddress(rtCharset, id);
	assert(_fontPtr);
	if (_vm->_features & GF_AFTER_V3) {
		_nbChars = _fontPtr[4];
		_fontPtr += 6 + _nbChars;
	} else 	if (_vm->_features & GF_AFTER_V4)
		_fontPtr += 17;
	else
		_fontPtr += 29;
}

// do spacing for variable width old-style font
int CharsetRendererClassic::getCharWidth(byte chr) {
	int spacing = 0;

	int offs = READ_LE_UINT32(_fontPtr + chr * 4 + 4);
	if (offs) {
		spacing = _fontPtr[offs];
		if (_fontPtr[offs + 2] >= 0x80) {
			spacing += _fontPtr[offs + 2] - 0x100;
		} else {
			spacing += _fontPtr[offs + 2];
		}
	}
	
	return spacing;
}

int CharsetRendererOld256::getCharWidth(byte chr) {
	int spacing = 0;

	spacing = *(_fontPtr - _nbChars + chr);

	// FIXME - this fixes the inventory icons in Zak256/Indy3
	//  see bug #613109.
	// chars 1,2: up arrow chars 3,4: down arrow
//	if ((_vm->_gameId == GID_ZAK256 || _vm->_gameId == GID_INDY3_256)
//			&& (chr >= 1 && chr <= 4))
//		spacing = 6;

	return spacing;
}

int CharsetRenderer::getStringWidth(int arg, byte *text) {
	int pos = 0;
	int width = 1;
	byte chr;
	int oldID = getCurID(); 

	while ((chr = text[pos++]) != 0) {
		if (chr == 0xD)
			break;
		if (chr == '@')
			continue;
		if (chr == 254 || chr == 255) {
			chr = text[pos++];
			if (chr == 3)	// 'WAIT'
				break;
			if (chr == 8) { // 'Verb on next line'
				if (arg == 1)
					break;
				while (text[pos] == ' ')
					text[pos++] = '@';
				continue;
			}
			if (chr == 10 || chr == 21 || chr == 12 || chr == 13) {
				pos += 2;
				continue;
			}
			if (chr == 9 || chr == 1 || chr == 2) // 'Newline'
				break;
			if (chr == 14) {
				int set = text[pos] | (text[pos + 1] << 8);
				pos += 2;
				setCurID(set);
				continue;
			}
		}
		width += getCharWidth(chr);
	}

	setCurID(oldID);

	return width;
}

void CharsetRenderer::addLinebreaks(int a, byte *str, int pos, int maxwidth) {
	int lastspace = -1;
	int curw = 1;
	byte chr;
	int oldID = getCurID(); 

	while ((chr = str[pos++]) != 0) {
		if (chr == '@')
			continue;
		if (chr == 254)
			chr = 255;
		if (chr == 255) {
			chr = str[pos++];
			if (chr == 3) // 'Wait'
				break;
			if (chr == 8) { // 'Verb on next line'
				if (a == 1) {
					curw = 1;
				} else {
					while (str[pos] == ' ')
						str[pos++] = '@';
				}
				continue;
			}
			if (chr == 10 || chr == 21 || chr == 12 || chr == 13) {
				pos += 2;
				continue;
			}
			if (chr == 1) { // 'Newline'
				curw = 1;
				continue;
			}
			if (chr == 2) // 'Don't terminate with \n'
				break;
			if (chr == 14) {
				int set = str[pos] | (str[pos + 1] << 8);
				pos += 2;
				setCurID(set);
				continue;
			}
		}

		if (chr == ' ')
			lastspace = pos - 1;

		curw += getCharWidth(chr);
		if (lastspace == -1)
			continue;
		if (curw > maxwidth) {
			str[lastspace] = 0xD;
			curw = 1;
			pos = lastspace + 1;
			lastspace = -1;
		}
	}

	setCurID(oldID);
}

void CharsetRendererOld256::setColor(byte color)
{
	_color = color;
	if (_vm->_features & GF_16COLOR) {
		_dropShadow = ((_color & 0xF0) != 0);
		_color &= 0x0f;
	} else
		_dropShadow = false;
}

void CharsetRendererOld256::printChar(int chr) {
	// Indy3 / Zak256 / Loom
	VirtScreen *vs;
	byte *char_ptr, *dest_ptr, *mask_ptr;
	unsigned int buffer = 0, bit = 0, x = 0, y = 0;
	bool useMask;
	int w, h;

	if (!_dropShadow) {
		w = h = 8;
	} else {
		w = h = 9;
	}
	
	_vm->checkRange(_vm->_maxCharsets - 1, 0, _curId, "Printing with bad charset %d");

	if ((vs = _vm->findVirtScreen(_top)) == NULL)
		return;

	if (chr == '@')
		return;

	if (_firstChar) {
		_strLeft = _left;
		_strTop = _top;
		_strRight = _left;
		_strBottom = _top;
		_firstChar = false;
	}

	char_ptr = _fontPtr + chr * 8;
	dest_ptr = vs->screenPtr + vs->xstart + (_top - vs->topline) * _vm->_realWidth + _left;
	mask_ptr = _vm->getResourceAddress(rtBuffer, 9) + _vm->_screenStartStrip + (_top - vs->topline) * _vm->gdi._numStrips + _left / 8;
	useMask = (vs->number == 0 && !_ignoreCharsetMask);

	_vm->updateDirtyRect(vs->number, _left, _left + w, _top - vs->topline, _top - vs->topline + h, 0);
	if (vs->number == 0)
		_hasMask = true;

	for (y = 0; y < 8; y++) {
		byte maskmask = revBitMask[_left & 7];
		int maskpos = 0;

		for (x = 0; x < 8; x++) {
			if ((bit >>= 1) == 0) {
				buffer = *char_ptr++;
				bit = 0x80;
			}
			if (buffer & bit) {
				byte *dst = dest_ptr + y * _vm->_realWidth + x;

				if (_dropShadow)
					*(dst + _vm->_realWidth + 1) = 0;
				*dst = _color;

				if (useMask) {
					mask_ptr[maskpos] |= maskmask;
					if (_dropShadow) {
						int spos = maskpos + _vm->gdi._numStrips;
						byte sbit = maskmask >> 1;

						if (sbit == 0) {
							sbit = 0x80;
							spos++;
						}
						mask_ptr[spos] |= sbit;
					}
				}
			}
			maskmask >>= 1;
			if (maskmask == 0) {
				maskmask = 0x80;
				maskpos++;
			}
		}
		mask_ptr += _vm->gdi._numStrips;
	}

	// FIXME
	if (_strLeft > _left)
		_strLeft = _left;

	_left += getCharWidth(chr);

	if (_strRight < _left) {
		_strRight = _left;
		if (_dropShadow)
			_strRight++;
	}

	if (_strBottom < _top + h)
		_strBottom = _top + h;
}

void CharsetRendererClassic::printChar(int chr) {
	int width, height;
	int offsX, offsY;
	int d;
	VirtScreen *vs;

	_vm->checkRange(_vm->_maxCharsets - 1, 1, _curId, "Printing with bad charset %d");
	
	if ((vs = _vm->findVirtScreen(_top)) == NULL)
		return;

	if (chr == '@')
		return;

	_bpp = *_fontPtr;
	_vm->_charsetColorMap[1] = _color;

	uint32 charOffs = READ_LE_UINT32(_fontPtr + chr * 4 + 4);

	if (!charOffs)
		return;

	assert(charOffs < 0x10000);

	_charPtr = _fontPtr + charOffs;

	width = _charPtr[0];
	height = _charPtr[1];
	if (_firstChar) {
		_strLeft = 0;
		_strTop = 0;
		_strRight = 0;
		_strBottom = 0;
	}

	if (_disableOffsX) {
		offsX = 0;
	} else {
		d = _charPtr[2];
		if (d >= 0x80)
			d -= 0x100;
		offsX = d;
	}

	d = _charPtr[3];
	if (d >= 0x80)
		d -= 0x100;
	offsY = d;

	_top += offsY;
	_left += offsX;

	if (_left + width > _right + 1 || _left < 0) {
		_left += width;
		_top -= offsY;
		return;
	}

	_disableOffsX = false;

	if (_firstChar) {
		_strLeft = _left;
		_strTop = _top;
		_strRight = _left;
		_strBottom = _top;
		_firstChar = false;
	}

	if (_left < _strLeft)
		_strLeft = _left;

	if (_top < _strTop)
		_strTop = _top;

	int drawTop = _top - vs->topline;
	if (drawTop < 0)
		drawTop = 0;
	int bottom = drawTop + height + offsY;

	_vm->updateDirtyRect(vs->number, _left, _left + width, drawTop, bottom, 0);

	if (vs->number != 0)
		_blitAlso = false;
	if (vs->number == 0 && !_blitAlso)
		_hasMask = true;

	_charPtr += 4;

	byte *mask = _vm->getResourceAddress(rtBuffer, 9)
		+ drawTop * _vm->gdi._numStrips + _left / 8 + _vm->_screenStartStrip;

	byte *dst = vs->screenPtr + vs->xstart + drawTop * _vm->_realWidth + _left;

	if (_blitAlso) {
		byte *back = dst;
		dst = _vm->getResourceAddress(rtBuffer, vs->number + 5)
			+ vs->xstart + drawTop * _vm->_realWidth + _left;

		drawBits(vs, dst, mask, drawTop, width, height);

		_vm->blit(back, dst, width, height);
	} else {
		drawBits(vs, dst, mask, drawTop, width, height);
	}
	
	_left += width;
	if (_left > _strRight)
		_strRight = _left;

	if (_top + height > _strBottom)
		_strBottom = _top + height;

	_top -= offsY;
}

void CharsetRendererClassic::drawBits(VirtScreen *vs, byte *dst, byte *mask, int drawTop, int width, int height) {
	byte maskmask;
	int y, x;
	int maskpos;
	int color;
	byte numbits, bits;
	bool useMask = (vs->number == 0 && !_ignoreCharsetMask);

	bits = *_charPtr++;
	numbits = 8;

	y = 0;

	for (y = 0; y < height && y + drawTop < vs->height; y++) {
		maskmask = revBitMask[_left & 7];
		maskpos = 0;

		for (x = 0; x < width; x++) {
			int myMask = (0xFF << (8 - _bpp)) & 0xFF;
			int myColor = (bits & myMask) >> (8 - _bpp);
			color = (bits >> (8 - _bpp)) & 0xFF;
			assert(color == myColor);
			
			if (color) {
				if (useMask) {
					mask[maskpos] |= maskmask;
				}
				*dst = _vm->_charsetColorMap[color];
			}
			dst++;
			bits <<= _bpp;
			numbits -= _bpp;
			if (numbits == 0) {
				bits = *_charPtr++;
				numbits = 8;
			}
			maskmask >>= 1;
			if (maskmask == 0) {
				maskmask = 0x80;
				maskpos++;
			}
		}
		dst += _vm->_realWidth - width;
		mask += _vm->gdi._numStrips;
	}
}

CharsetRendererNut::CharsetRendererNut(Scumm *vm)
	 : CharsetRenderer(vm) {
	_current = 0;

	for (int i = 0; i < 5; i++) {
		char fontname[256];
		sprintf(fontname, "font%d.nut", i);
		_fr[i] = new NutRenderer(_vm);
		if (!(_fr[i]->loadFont(fontname, _vm->getGameDataPath()))) {
			delete _fr[i];
			_fr[i] = NULL;
		}
	}
}

CharsetRendererNut::~CharsetRendererNut() {
	for (int i = 0; i < 5; i++)
		delete _fr[i];
}

void CharsetRendererNut::setCurID(byte id) {
	assert(id < 5);
	_curId = id;
	_current = _fr[id];
	assert(_current);
}

int CharsetRendererNut::getCharWidth(byte chr) {
	assert(_current);
	return _current->getCharWidth(chr);
}

int CharsetRendererNut::getFontHeight() {
	// FIXME / TODO: how to implement this properly???
	assert(_current);
	return _current->getCharHeight('|');
}

void CharsetRendererNut::printChar(int chr) {
	assert(_current);
	if (chr == '@')
		return;

	if (_firstChar) {
		_strLeft = _left;
		_strTop = _top;
		_strRight = _left;
		_strBottom = _top;
		_firstChar = false;
	}

	int width = _current->getCharWidth(chr);
	int height = _current->getCharHeight(chr);

	_hasMask = true;
	_current->drawChar((char)chr, _left, _top, _color, !_ignoreCharsetMask);
	_vm->updateDirtyRect(0, _left, _left + width, _top, _top + height, 0);

	_left += width;
	if (_left > _strRight)
		_strRight = _left;

	if (_top + height > _strBottom)
		_strBottom = _top + height;
}

