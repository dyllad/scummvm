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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 */

#include "common/stdafx.h"
#include "graphics/font.h"

namespace Graphics {

// Built-in font
static const byte guifont[] = {
	// Header
	0, 0, 99, 1, 226, 8, 
	// Character width table
	4, 8, 6, 8, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	8, 2, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	4, 3, 7, 8, 7, 7, 8, 4, 5, 5, 8, 7, 4, 7, 3, 8, 
	7, 7, 7, 7, 8, 7, 7, 7, 7, 7, 3, 4, 7, 5, 7, 7, 
	8, 7, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 7, 8, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 8, 7, 7, 7, 5, 8, 5, 8, 8, 
	7, 7, 7, 6, 7, 7, 7, 7, 7, 5, 6, 7, 5, 8, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 8, 7, 7, 7, 5, 3, 5, 7, 8, 
	7, 7, 7, 7, 7, 7, 0, 6, 7, 7, 7, 5, 5, 5, 7, 0, 
	6, 8, 8, 7, 7, 7, 7, 7, 0, 7, 7, 0, 0, 0, 0, 0, 
	7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 7, 
	// Character table
	0,   0,   0,   0,   0,   0,   0,   0,   	// 0
	1,   3,   6,   12,  24,  62,  3,   0,   	// 1
	128, 192, 96,  48,  24,  124, 192, 0,   	// 2
	0,   3,   62,  24,  12,  6,   3,   1,   	// 3
	0,   192, 124, 24,  48,  96,  192, 128, 	// 4
	0,   0,   0,   0,   0,   0,   0,   0,   	// 5
	0,   0,   0,   0,   0,   0,   0,   0,   	// 6
	0,   0,   0,   0,   0,   0,   0,   0,   	// 7
	0,   0,   0,   0,   0,   0,   0,   0,   	// 8
	0,   0,   0,   0,   0,   0,   0,   0,   	// 9
	0,   0,   0,   0,   0,   0,   0,   0,   	// 10
	0,   0,   0,   0,   0,   0,   0,   0,   	// 11
	0,   0,   0,   0,   0,   0,   0,   0,   	// 12
	0,   0,   0,   0,   0,   0,   0,   0,   	// 13
	0,   0,   0,   0,   0,   0,   0,   0,   	// 14
	0,   0,   0,   0,   0,   0,   0,   0,   	// 15
	237, 74,  72,  0,   0,   0,   0,   0,   	// 16
	128, 128, 128, 0,   0,   0,   0,   0,   	// 17
	0,   0,   0,   0,   0,   0,   0,   0,   	// 18
	60,  66,  153, 161, 161, 153, 66,  60,  	// 19
	0,   0,   0,   0,   0,   0,   0,   0,   	// 20
	0,   0,   0,   0,   0,   0,   0,   0,   	// 21
	0,   0,   0,   0,   0,   0,   0,   0,   	// 22
	0,   0,   0,   0,   0,   0,   0,   0,   	// 23
	0,   0,   0,   0,   0,   0,   0,   0,   	// 24
	0,   0,   0,   0,   0,   0,   0,   0,   	// 25
	0,   0,   0,   0,   0,   0,   0,   0,   	// 26
	0,   0,   0,   0,   0,   0,   0,   0,   	// 27
	0,   0,   0,   0,   0,   0,   0,   0,   	// 28
	0,   0,   0,   0,   0,   0,   0,   0,   	// 29
	0,   0,   0,   0,   0,   0,   0,   0,   	// 30
	0,   0,   0,   0,   0,   0,   0,   0,   	// 31
	0,   0,   0,   0,   0,   0,   0,   0,   	// 32
	96,  96,  96,  96,  0,   0,   96,  0,   	// 33
	102, 102, 102, 0,   0,   0,   0,   0,   	// 34
	102, 102, 255, 102, 255, 102, 102, 0,   	// 35
	24,  62,  96,  60,  6,   124, 24,  0,   	// 36
	98,  102, 12,  24,  48,  102, 70,  0,   	// 37
	60,  102, 60,  56,  103, 102, 63,  0,   	// 38
	96,  48,  16,  0,   0,   0,   0,   0,   	// 39
	24,  48,  96,  96,  96,  48,  24,  0,   	// 40
	96,  48,  24,  24,  24,  48,  96,  0,   	// 41
	0,   102, 60,  255, 60,  102, 0,   0,   	// 42
	0,   24,  24,  126, 24,  24,  0,   0,   	// 43
	0,   0,   0,   0,   0,   48,  48,  96,  	// 44
	0,   0,   0,   126, 0,   0,   0,   0,   	// 45
	0,   0,   0,   0,   0,   96,  96,  0,   	// 46
	0,   3,   6,   12,  24,  48,  96,  0,   	// 47
	60,  102, 102, 102, 102, 102, 60,  0,   	// 48
	24,  24,  56,  24,  24,  24,  126, 0,   	// 49
	60,  102, 6,   12,  48,  96,  126, 0,   	// 50
	60,  102, 6,   28,  6,   102, 60,  0,   	// 51
	6,   14,  30,  102, 127, 6,   6,   0,   	// 52
	126, 96,  124, 6,   6,   102, 60,  0,   	// 53
	60,  102, 96,  124, 102, 102, 60,  0,   	// 54
	126, 102, 12,  24,  24,  24,  24,  0,   	// 55
	60,  102, 102, 60,  102, 102, 60,  0,   	// 56
	60,  102, 102, 62,  6,   102, 60,  0,   	// 57
	0,   0,   96,  0,   0,   96,  0,   0,   	// 58
	0,   0,   48,  0,   0,   48,  48,  96,  	// 59
	14,  24,  48,  96,  48,  24,  14,  0,   	// 60
	0,   0,   120, 0,   120, 0,   0,   0,   	// 61
	112, 24,  12,  6,   12,  24,  112, 0,   	// 62
	60,  102, 6,   12,  24,  0,   24,  0,   	// 63
	0,   0,   0,   255, 255, 0,   0,   0,   	// 64
	24,  60,  102, 126, 102, 102, 102, 0,   	// 65
	124, 102, 102, 124, 102, 102, 124, 0,   	// 66
	60,  102, 96,  96,  96,  102, 60,  0,   	// 67
	120, 108, 102, 102, 102, 108, 120, 0,   	// 68
	126, 96,  96,  120, 96,  96,  126, 0,   	// 69
	126, 96,  96,  120, 96,  96,  96,  0,   	// 70
	60,  102, 96,  110, 102, 102, 60,  0,   	// 71
	102, 102, 102, 126, 102, 102, 102, 0,   	// 72
	120, 48,  48,  48,  48,  48,  120, 0,   	// 73
	30,  12,  12,  12,  12,  108, 56,  0,   	// 74
	102, 108, 120, 112, 120, 108, 102, 0,   	// 75
	96,  96,  96,  96,  96,  96,  126, 0,   	// 76
	99,  119, 127, 107, 99,  99,  99,  0,   	// 77
	102, 118, 126, 126, 110, 102, 102, 0,   	// 78
	60,  102, 102, 102, 102, 102, 60,  0,   	// 79
	124, 102, 102, 124, 96,  96,  96,  0,   	// 80
	60,  102, 102, 102, 102, 60,  14,  0,   	// 81
	124, 102, 102, 124, 120, 108, 102, 0,   	// 82
	60,  102, 96,  60,  6,   102, 60,  0,   	// 83
	126, 24,  24,  24,  24,  24,  24,  0,   	// 84
	102, 102, 102, 102, 102, 102, 60,  0,   	// 85
	102, 102, 102, 102, 102, 60,  24,  0,   	// 86
	99,  99,  99,  107, 127, 119, 99,  0,   	// 87
	102, 102, 60,  24,  60,  102, 102, 0,   	// 88
	102, 102, 102, 60,  24,  24,  24,  0,   	// 89
	126, 6,   12,  24,  48,  96,  126, 0,   	// 90
	120, 96,  96,  96,  96,  96,  120, 0,   	// 91
	3,   6,   12,  24,  48,  96,  192, 0,   	// 92
	120, 24,  24,  24,  24,  24,  120, 0,   	// 93
	0,   0,   0,   0,   0,   219, 219, 0,   	// 94
	0,   0,   0,   0,   0,   0,   0,   255, 	// 95
	102, 102, 102, 0,   0,   0,   0,   0,   	// 96
	0,   0,   60,  6,   62,  102, 62,  0,   	// 97
	0,   96,  96,  124, 102, 102, 124, 0,   	// 98
	0,   0,   60,  96,  96,  96,  60,  0,   	// 99
	0,   6,   6,   62,  102, 102, 62,  0,   	// 100
	0,   0,   60,  102, 126, 96,  60,  0,   	// 101
	0,   14,  24,  62,  24,  24,  24,  0,   	// 102
	0,   0,   62,  102, 102, 62,  6,   124, 	// 103
	0,   96,  96,  124, 102, 102, 102, 0,   	// 104
	0,   48,  0,   112, 48,  48,  120, 0,   	// 105
	0,   12,  0,   12,  12,  12,  12,  120, 	// 106
	0,   96,  96,  108, 120, 108, 102, 0,   	// 107
	0,   112, 48,  48,  48,  48,  120, 0,   	// 108
	0,   0,   102, 127, 127, 107, 99,  0,   	// 109
	0,   0,   124, 102, 102, 102, 102, 0,   	// 110
	0,   0,   60,  102, 102, 102, 60,  0,   	// 111
	0,   0,   124, 102, 102, 124, 96,  96,  	// 112
	0,   0,   62,  102, 102, 62,  6,   6,   	// 113
	0,   0,   124, 102, 96,  96,  96,  0,   	// 114
	0,   0,   62,  96,  60,  6,   124, 0,   	// 115
	0,   24,  126, 24,  24,  24,  14,  0,   	// 116
	0,   0,   102, 102, 102, 102, 62,  0,   	// 117
	0,   0,   102, 102, 102, 60,  24,  0,   	// 118
	0,   0,   99,  107, 127, 62,  54,  0,   	// 119
	0,   0,   102, 60,  24,  60,  102, 0,   	// 120
	0,   0,   102, 102, 102, 62,  12,  120, 	// 121
	0,   0,   126, 12,  24,  48,  126, 0,   	// 122
	24,  48,  48,  96,  48,  48,  24,  0,   	// 123
	96,  96,  96,  0,   96,  96,  96,  0,   	// 124
	96,  48,  48,  24,  48,  48,  96,  0,   	// 125
	0,   0,   97,  153, 134, 0,   0,   0,   	// 126
	8,   12,  14,  255, 255, 14,  12,  8,   	// 127
	60,  102, 96,  96,  102, 60,  24,  56,  	// 128
	102, 0,   102, 102, 102, 102, 62,  0,   	// 129
	12,  24,  60,  102, 126, 96,  60,  0,   	// 130
	24,  36,  60,  6,   62,  102, 62,  0,   	// 131
	102, 0,   60,  6,   62,  102, 62,  0,   	// 132
	48,  24,  60,  6,   62,  102, 62,  0,   	// 133
	0,   0,   0,   0,   0,   0,   0,   0,   	// 134
	0,   60,  96,  96,  96,  60,  24,  56,  	// 135
	24,  36,  60,  102, 126, 96,  60,  0,   	// 136
	102, 0,   60,  102, 126, 96,  60,  0,   	// 137
	48,  24,  60,  102, 126, 96,  60,  0,   	// 138
	0,   216, 0,   112, 48,  48,  120, 0,   	// 139
	48,  72,  0,   112, 48,  48,  120, 0,   	// 140
	96,  48,  0,   112, 48,  48,  120, 0,   	// 141
	102, 24,  60,  102, 126, 102, 102, 0,   	// 142
	0,   0,   0,   0,   0,   0,   0,   0,   	// 143
	24,  48,  124, 96,  120, 96,  124, 0,   	// 144
	0,   0,   108, 26,  126, 216, 110, 0,   	// 145
	30,  40,  40,  126, 72,  136, 142, 0,   	// 146
	24,  36,  60,  102, 102, 102, 60,  0,   	// 147
	102, 0,   60,  102, 102, 102, 60,  0,   	// 148
	48,  24,  60,  102, 102, 102, 60,  0,   	// 149
	24,  36,  0,   102, 102, 102, 62,  0,   	// 150
	48,  24,  102, 102, 102, 102, 62,  0,   	// 151
	0,   0,   0,   0,   0,   0,   0,   0,   	// 152
	102, 60,  102, 102, 102, 102, 60,  0,   	// 153
	102, 0,   102, 102, 102, 102, 60,  0,   	// 154
	0,   0,   0,   0,   0,   0,   0,   0,   	// 155
	0,   0,   0,   0,   0,   0,   0,   0,   	// 156
	0,   0,   0,   0,   0,   0,   0,   0,   	// 157
	0,   0,   0,   0,   0,   0,   0,   0,   	// 158
	0,   0,   0,   0,   0,   0,   0,   0,   	// 159
	12,  24,  60,  6,   62,  102, 62,  0,   	// 160
	0,   0,   0,   0,   0,   0,   0,   0,   	// 161
	0,   0,   0,   0,   0,   0,   0,   0,   	// 162
	0,   0,   0,   0,   0,   0,   0,   0,   	// 163
	0,   0,   0,   0,   0,   0,   0,   0,   	// 164
	0,   0,   0,   0,   0,   0,   0,   0,   	// 165
	0,   0,   0,   0,   0,   0,   0,   0,   	// 166
	0,   0,   0,   0,   0,   0,   0,   0,   	// 167
	0,   0,   0,   0,   0,   0,   0,   0,   	// 168
	0,   0,   0,   0,   0,   0,   0,   0,   	// 169
	0,   0,   0,   0,   0,   0,   0,   0,   	// 170
	0,   0,   0,   0,   0,   0,   0,   0,   	// 171
	0,   0,   0,   0,   0,   0,   0,   0,   	// 172
	0,   0,   0,   0,   0,   0,   0,   0,   	// 173
	0,   0,   0,   0,   0,   0,   0,   0,   	// 174
	0,   0,   0,   0,   0,   0,   0,   0,   	// 175
	0,   0,   0,   0,   0,   0,   0,   0,   	// 176
	0,   0,   0,   0,   0,   0,   0,   0,   	// 177
	0,   0,   0,   0,   0,   0,   0,   0,   	// 178
	0,   0,   0,   0,   0,   0,   0,   0,   	// 179
	0,   0,   0,   0,   0,   0,   0,   0,   	// 180
	0,   0,   0,   0,   0,   0,   0,   0,   	// 181
	0,   0,   0,   0,   0,   0,   0,   0,   	// 182
	0,   0,   0,   0,   0,   0,   0,   0,   	// 183
	0,   0,   0,   0,   0,   0,   0,   0,   	// 184
	0,   0,   0,   0,   0,   0,   0,   0,   	// 185
	0,   0,   0,   0,   0,   0,   0,   0,   	// 186
	0,   0,   0,   0,   0,   0,   0,   0,   	// 187
	0,   0,   0,   0,   0,   0,   0,   0,   	// 188
	0,   0,   0,   0,   0,   0,   0,   0,   	// 189
	0,   0,   0,   0,   0,   0,   0,   0,   	// 190
	0,   0,   0,   0,   0,   0,   0,   0,   	// 191
	0,   0,   0,   0,   0,   0,   0,   0,   	// 192
	0,   0,   0,   0,   0,   0,   0,   0,   	// 193
	0,   0,   0,   0,   0,   0,   0,   0,   	// 194
	0,   0,   0,   0,   0,   0,   0,   0,   	// 195
	0,   0,   0,   0,   0,   0,   0,   0,   	// 196
	0,   0,   0,   0,   0,   0,   0,   0,   	// 197
	0,   0,   0,   0,   0,   0,   0,   0,   	// 198
	0,   0,   0,   0,   0,   0,   0,   0,   	// 199
	0,   0,   0,   0,   0,   0,   0,   0,   	// 200
	0,   0,   0,   0,   0,   0,   0,   0,   	// 201
	0,   0,   0,   0,   0,   0,   0,   0,   	// 202
	0,   0,   0,   0,   0,   0,   0,   0,   	// 203
	0,   0,   0,   0,   0,   0,   0,   0,   	// 204
	0,   0,   0,   0,   0,   0,   0,   0,   	// 205
	0,   0,   0,   0,   0,   0,   0,   0,   	// 206
	0,   0,   0,   0,   0,   0,   0,   0,   	// 207
	0,   0,   0,   0,   0,   0,   0,   0,   	// 208
	0,   0,   0,   0,   0,   0,   0,   0,   	// 209
	0,   0,   0,   0,   0,   0,   0,   0,   	// 210
	0,   0,   0,   0,   0,   0,   0,   0,   	// 211
	0,   0,   0,   0,   0,   0,   0,   0,   	// 212
	0,   0,   0,   0,   0,   0,   0,   0,   	// 213
	0,   0,   0,   0,   0,   0,   0,   0,   	// 214
	0,   0,   0,   0,   0,   0,   0,   0,   	// 215
	0,   0,   0,   0,   0,   0,   0,   0,   	// 216
	0,   0,   0,   0,   0,   0,   0,   0,   	// 217
	0,   0,   0,   0,   0,   0,   0,   0,   	// 218
	0,   0,   0,   0,   0,   0,   0,   0,   	// 219
	0,   0,   0,   0,   0,   0,   0,   0,   	// 220
	0,   0,   0,   0,   0,   0,   0,   0,   	// 221
	0,   0,   0,   0,   0,   0,   0,   0,   	// 222
	0,   0,   0,   0,   0,   0,   0,   0,   	// 223
	0,   0,   0,   0,   0,   0,   0,   0,   	// 224
	28,  54,  54,  124, 102, 102, 124, 64,  	// 225
	0,   0,   0					// ???
};

int ScummFont::getCharWidth(byte chr) const {
	return guifont[chr+6];
}

void ScummFont::drawChar(Surface *dst, byte chr, int tx, int ty, uint32 color) const {
	assert(dst != 0);
	byte *ptr = (byte *)dst->getBasePtr(tx, ty);

	const byte *tmp = guifont + 6 + guifont[4] + chr * 8;
	uint buffer = 0;
	uint mask = 0;

	for (int y = 0; y < 8; y++) {
		if (ty + y < 0 || ty + y >= dst->h)
			continue;
		for (int x = 0; x < 8; x++) {
			if (tx + x < 0 || tx + x >= dst->w)
				continue;
			unsigned char c;
			mask >>= 1;
			if (mask == 0) {
				buffer = *tmp++;
				mask = 0x80;
			}
			c = ((buffer & mask) != 0);
			if (c) {
				if (dst->bytesPerPixel == 1)
					ptr[x] = color;
				else if (dst->bytesPerPixel == 2)
					((uint16 *)ptr)[x] = color;
			}
		}
		ptr += dst->pitch;
	}
}

} // End of namespace Graphics
