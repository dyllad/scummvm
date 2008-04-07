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

/*! \file
 * Script interpreter file
 */

#include "common/endian.h"

#include "cine/cine.h"
#include "cine/bg_list.h"
#include "cine/object.h"
#include "cine/sound.h"
#include "cine/various.h"
#include "cine/script.h"

namespace Cine {

uint16 compareVars(int16 a, int16 b);
void palRotate(byte a, byte b, byte c);
void removeSeq(uint16 param1, uint16 param2, uint16 param3);
uint16 isSeqRunning(uint16 param1, uint16 param2, uint16 param3);
void addGfxElementA0(int16 param1, int16 param2);
void removeGfxElementA0(int16 idx, int16 param);

const Opcode FWScript::_opcodeTable[] = {
	/* 00 */
	{ &FWScript::o1_modifyObjectParam, "bbw" },
	{ &FWScript::o1_getObjectParam, "bbb" },
	{ &FWScript::o1_addObjectParam, "bbw" },
	{ &FWScript::o1_subObjectParam, "bbw" },
	/* 04 */
	{ &FWScript::o1_add2ObjectParam, "bbw" },
	{ &FWScript::o1_sub2ObjectParam, "bbw" },
	{ &FWScript::o1_compareObjectParam, "bbw" },
	{ &FWScript::o1_setupObject, "bwwww" },
	/* 08 */
	{ &FWScript::o1_checkCollision, "bwwww" },
	{ &FWScript::o1_loadVar, "bc" },
	{ &FWScript::o1_addVar, "bc" },
	{ &FWScript::o1_subVar, "bc" },
	/* 0C */
	{ &FWScript::o1_mulVar, "bc" },
	{ &FWScript::o1_divVar, "bc" },
	{ &FWScript::o1_compareVar, "bc" },
	{ &FWScript::o1_modifyObjectParam2, "bbb" },
	/* 10 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o1_loadMask0, "b" },
	/* 14 */
	{ &FWScript::o1_unloadMask0, "b" },
	{ &FWScript::o1_addToBgList, "b" },
	{ &FWScript::o1_loadMask1, "b" },
	{ &FWScript::o1_unloadMask1, "b" },
	/* 18 */
	{ &FWScript::o1_loadMask4, "b" },
	{ &FWScript::o1_unloadMask4, "b" },
	{ &FWScript::o1_addSpriteFilledToBgList, "b" },
	{ &FWScript::o1_op1B, "" },
	/* 1C */
	{ 0, 0 },
	{ &FWScript::o1_label, "l" },
	{ &FWScript::o1_goto, "b" },
	{ &FWScript::o1_gotoIfSup, "b" },
	/* 20 */
	{ &FWScript::o1_gotoIfSupEqu, "b" },
	{ &FWScript::o1_gotoIfInf, "b" },
	{ &FWScript::o1_gotoIfInfEqu, "b" },
	{ &FWScript::o1_gotoIfEqu, "b" },
	/* 24 */
	{ &FWScript::o1_gotoIfDiff, "b" },
	{ &FWScript::o1_removeLabel, "b" },
	{ &FWScript::o1_loop, "bb" },
	{ 0, 0 },
	/* 28 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 2C */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 30 */
	{ 0, 0 },
	{ &FWScript::o1_startGlobalScript, "b" },
	{ &FWScript::o1_endGlobalScript, "b" },
	{ 0, 0 },
	/* 34 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 38 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o1_loadAnim, "s" },
	/* 3C */
	{ &FWScript::o1_loadBg, "s" },
	{ &FWScript::o1_loadCt, "s" },
	{ 0, 0 },
	{ &FWScript::o1_loadPart, "s" },
	/* 40 */
	{ &FWScript::o1_closePart, "" },
	{ &FWScript::o1_loadNewPrcName, "bs" },
	{ &FWScript::o1_requestCheckPendingDataLoad, "" },
	{ 0, 0 },
	/* 44 */
	{ 0, 0 },
	{ &FWScript::o1_blitAndFade, "" },
	{ &FWScript::o1_fadeToBlack, "" },
	{ &FWScript::o1_transformPaletteRange, "bbwww" },
	/* 48 */
	{ 0, 0 },
	{ &FWScript::o1_setDefaultMenuColor2, "b" },
	{ &FWScript::o1_palRotate, "bbb" },
	{ 0, 0 },
	/* 4C */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o1_break, "" },
	/* 50 */
	{ &FWScript::o1_endScript, "x" },
	{ &FWScript::o1_message, "bwwww" },
	{ &FWScript::o1_loadGlobalVar, "bc" },
	{ &FWScript::o1_compareGlobalVar, "bc" },
	/* 54 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 58 */
	{ 0, 0 },
	{ &FWScript::o1_declareFunctionName, "s" },
	{ &FWScript::o1_freePartRange, "bb" },
	{ &FWScript::o1_unloadAllMasks, "" },
	// 5C */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 60 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o1_setScreenDimensions, "wwww" },
	/* 64 */
	{ &FWScript::o1_displayBackground, "" },
	{ &FWScript::o1_initializeZoneData, "" },
	{ &FWScript::o1_setZoneDataEntry, "bw" },
	{ &FWScript::o1_getZoneDataEntry, "bb" },
	/* 68 */
	{ &FWScript::o1_setDefaultMenuColor, "b" },
	{ &FWScript::o1_allowPlayerInput, "" },
	{ &FWScript::o1_disallowPlayerInput, "" },
	{ &FWScript::o1_changeDataDisk, "b" },
	/* 6C */
	{ 0, 0 },
	{ &FWScript::o1_loadMusic, "s" },
	{ &FWScript::o1_playMusic, "" },
	{ &FWScript::o1_fadeOutMusic, "" },
	/* 70 */
	{ &FWScript::o1_stopSample, "" },
	{ &FWScript::o1_op71, "bw" },
	{ &FWScript::o1_op72, "wbw" },
	{ &FWScript::o1_op73, "wbw" },
	/* 74 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o1_playSample, "bbwbww" },
	/* 78 */
	{ &FWScript::o1_playSample, "bbwbww" },
	{ &FWScript::o1_disableSystemMenu, "b" },
	{ &FWScript::o1_loadMask5, "b" },
	{ &FWScript::o1_unloadMask5, "b" }
};
const unsigned int FWScript::_numOpcodes = ARRAYSIZE(FWScript::_opcodeTable);


const Opcode OSScript::_opcodeTable[] = {
	/* 00 */
	{ &FWScript::o1_modifyObjectParam, "bbw" },
	{ &FWScript::o1_getObjectParam, "bbb" },
	{ &FWScript::o1_addObjectParam, "bbw" },
	{ &FWScript::o1_subObjectParam, "bbw" },
	/* 04 */
	{ &FWScript::o1_add2ObjectParam, "bbw" },
	{ &FWScript::o1_sub2ObjectParam, "bbw" },
	{ &FWScript::o1_compareObjectParam, "bbw" },
	{ &FWScript::o1_setupObject, "bwwww" },
	/* 08 */
	{ &FWScript::o1_checkCollision, "bwwww" },
	{ &FWScript::o1_loadVar, "bc" },
	{ &FWScript::o1_addVar, "bc" },
	{ &FWScript::o1_subVar, "bc" },
	/* 0C */
	{ &FWScript::o1_mulVar, "bc" },
	{ &FWScript::o1_divVar, "bc" },
	{ &FWScript::o1_compareVar, "bc" },
	{ &FWScript::o1_modifyObjectParam2, "bbb" },
	/* 10 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o1_loadMask0, "b" },
	/* 14 */
	{ &FWScript::o1_unloadMask0, "b" },
	{ &FWScript::o1_addToBgList, "b" },
	{ &FWScript::o1_loadMask1, "b" },
	{ &FWScript::o1_unloadMask1, "b" },
	/* 18 */
	{ &FWScript::o1_loadMask4, "b" },
	{ &FWScript::o1_unloadMask4, "b" },
	{ &FWScript::o1_addSpriteFilledToBgList, "b" },
	{ &FWScript::o1_op1B, "" },
	/* 1C */
	{ 0, 0 },
	{ &FWScript::o1_label, "l" },
	{ &FWScript::o1_goto, "b" },
	{ &FWScript::o1_gotoIfSup, "b" },
	/* 20 */
	{ &FWScript::o1_gotoIfSupEqu, "b" },
	{ &FWScript::o1_gotoIfInf, "b" },
	{ &FWScript::o1_gotoIfInfEqu, "b" },
	{ &FWScript::o1_gotoIfEqu, "b" },
	/* 24 */
	{ &FWScript::o1_gotoIfDiff, "b" },
	{ &FWScript::o1_removeLabel, "b" },
	{ &FWScript::o1_loop, "bb" },
	{ 0, 0 },
	/* 28 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 2C */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 30 */
	{ 0, 0 },
	{ &FWScript::o1_startGlobalScript, "b" },
	{ &FWScript::o1_endGlobalScript, "b" },
	{ 0, 0 },
	/* 34 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 38 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o1_loadAnim, "s" },
	/* 3C */
	{ &FWScript::o1_loadBg, "s" },
	{ &FWScript::o1_loadCt, "s" },
	{ 0, 0 },
	{ &FWScript::o2_loadPart, "s" },
	/* 40 */
	{ 0, 0 }, /* o1_closePart, triggered by some scripts (STARTA.PRC 4 for ex.) */
	{ &FWScript::o1_loadNewPrcName, "bs" },
	{ &FWScript::o1_requestCheckPendingDataLoad, "" },
	{ 0, 0 },
	/* 44 */
	{ 0, 0 },
	{ &FWScript::o1_blitAndFade, "" },
	{ &FWScript::o1_fadeToBlack, "" },
	{ &FWScript::o1_transformPaletteRange, "bbwww" },
	/* 48 */
	{ 0, 0 },
	{ &FWScript::o1_setDefaultMenuColor2, "b" },
	{ &FWScript::o1_palRotate, "bbb" },
	{ 0, 0 },
	/* 4C */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o1_break, "" },
	/* 50 */
	{ &FWScript::o1_endScript, "x" },
	{ &FWScript::o1_message, "bwwww" },
	{ &FWScript::o1_loadGlobalVar, "bc" },
	{ &FWScript::o1_compareGlobalVar, "bc" },
	/* 54 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 58 */
	{ 0, 0 },
	{ &FWScript::o1_declareFunctionName, "s" },
	{ &FWScript::o1_freePartRange, "bb" },
	{ &FWScript::o1_unloadAllMasks, "" },
	// 5C */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* 60 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o1_setScreenDimensions, "wwww" },
	/* 64 */
	{ &FWScript::o1_displayBackground, "" },
	{ &FWScript::o1_initializeZoneData, "" },
	{ &FWScript::o1_setZoneDataEntry, "bw" },
	{ &FWScript::o1_getZoneDataEntry, "bb" },
	/* 68 */
	{ &FWScript::o1_setDefaultMenuColor, "b" },
	{ &FWScript::o1_allowPlayerInput, "" },
	{ &FWScript::o1_disallowPlayerInput, "" },
	{ &FWScript::o1_changeDataDisk, "b" },
	/* 6C */
	{ 0, 0 },
	{ &FWScript::o1_loadMusic, "s" },
	{ &FWScript::o1_playMusic, "" },
	{ &FWScript::o1_fadeOutMusic, "" },
	/* 70 */
	{ &FWScript::o1_stopSample, "" },
	{ &FWScript::o1_op71, "bw" },
	{ &FWScript::o1_op72, "wbw" },
	{ &FWScript::o1_op72, "wbw" },
	/* 74 */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o2_playSample, "bbwbww" },
	/* 78 */
	{ &FWScript::o2_playSampleAlt, "bbwbww" },
	{ &FWScript::o1_disableSystemMenu, "b" },
	{ &FWScript::o1_loadMask5, "b" },
	{ &FWScript::o1_unloadMask5, "b" },
	/* 7C */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o2_addSeqListElement, "bbbbwww" },
	/* 80 */
	{ &FWScript::o2_removeSeq, "bb" },
	{ &FWScript::o2_op81, "" },
	{ &FWScript::o2_op82, "bbw" },
	{ &FWScript::o2_isSeqRunning, "bb" },
	/* 84 */
	{ &FWScript::o2_gotoIfSupNearest, "b" },
	{ &FWScript::o2_gotoIfSupEquNearest, "b" },
	{ &FWScript::o2_gotoIfInfNearest, "b" },
	{ &FWScript::o2_gotoIfInfEquNearest, "b" },
	/* 88 */
	{ &FWScript::o2_gotoIfEquNearest, "b" },
	{ &FWScript::o2_gotoIfDiffNearest, "b" },
	{ 0, 0 },
	{ &FWScript::o2_startObjectScript, "b" },
	/* 8C */
	{ &FWScript::o2_stopObjectScript, "b" },
	{ &FWScript::o2_op8D, "wwwwwwww" },
	{ &FWScript::o2_addBackground, "bs" },
	{ &FWScript::o2_removeBackground, "b" },
	/* 90 */
	{ &FWScript::o2_loadAbs, "bs" },
	{ &FWScript::o2_loadBg, "b" },
	{ 0, 0 },
	{ 0, 0 },
	/* 94 */
	{ 0, 0 },
	{ &FWScript::o1_changeDataDisk, "b" },
	{ 0, 0 },
	{ 0, 0 },
	/* 98 */
	{ 0, 0 },
	{ 0, 0 },
	{ &FWScript::o2_wasZoneChecked, "" },
	{ &FWScript::o2_op9B, "wwwwwwww" },
	/* 9C */
	{ &FWScript::o2_op9C, "wwww" },
	{ &FWScript::o2_useBgScroll, "b" },
	{ &FWScript::o2_setAdditionalBgVScroll, "c" },
	{ &FWScript::o2_op9F, "ww" },
	/* A0 */
	{ &FWScript::o2_addGfxElementA0, "ww" },
	{ &FWScript::o2_removeGfxElementA0, "ww" },
	{ &FWScript::o2_opA2, "ww" },
	{ &FWScript::o2_opA3, "ww" },
	/* A4 */
	{ &FWScript::o2_loadMask22, "b" },
	{ &FWScript::o2_unloadMask22, "b" },
	{ 0, 0 },
	{ 0, 0 },
	/* A8 */
	{ 0, 0 },
	{ &FWScript::o1_changeDataDisk, "b" }
};
const unsigned int OSScript::_numOpcodes = ARRAYSIZE(OSScript::_opcodeTable);

FWScriptInfo *scriptInfo; ///< Script factory
RawScriptArray scriptTable; ///< Table of script bytecode

/*! \todo: replace with script subsystem
 */
void setupOpcodes() {
	static FWScriptInfo fw;
	static OSScriptInfo os;
	if (g_cine->getGameType() == Cine::GType_FW) {
		scriptInfo = &fw;
	} else {
		scriptInfo = &os;
	}
}

/*! \brief Allocate empty array
 * \param len Size of array
 *
 * Explicit to prevent var=0 instead of var[i]=0 typos.
 */
ScriptVars::ScriptVars(unsigned int len) : _size(len), _vars(new int16[len]) {
	assert(_vars);
	reset();
}

/*! \brief Allocate array and read contents from savefile
 * \param fHandle Savefile open for reading
 * \param len Size of array
 */
ScriptVars::ScriptVars(Common::InSaveFile &fHandle, unsigned int len)
	: _size(len), _vars(new int16[len]) {

	assert(_vars);

	load(fHandle);
}

/*! \brief Copy constructor
 */
ScriptVars::ScriptVars(const ScriptVars &src) : _size(src._size), _vars(new int16[_size]) {
	assert(_vars);
	memcpy(_vars, src._vars, _size * sizeof(int16));
}

/*! \brief Destructor
 */
ScriptVars::~ScriptVars(void) {
	delete[] _vars;
}

/*! \brief Assignment operator
 */
ScriptVars &ScriptVars::operator=(const ScriptVars &src) {
	ScriptVars tmp(src);
	int16 *tmpvars = _vars;

	_vars = tmp._vars;
	tmp._vars = tmpvars;
	_size = src._size;

	return *this;
}

/*! \brief Direct array item access
 * \param idx Item index
 * \return Reference to item
 */
int16 &ScriptVars::operator[](unsigned int idx) {
	debug(6, "assert(%d < %d)", idx, _size);
	assert(idx < _size);
	return _vars[idx];
}

/*! \brief Direct read-only array item access
 * \param idx Item index
 * \return Copy of item
 */
int16 ScriptVars::operator[](unsigned int idx) const {
	debug(6, "assert(%d < %d)", idx, _size);
	assert(idx < _size);
	return _vars[idx];
}

/*! \brief Savefile writer
 * \param fHandle Savefile open for writing
 */
void ScriptVars::save(Common::OutSaveFile &fHandle) const {
	save(fHandle, _size);
}

/*! \brief Savefile writer with data length limit
 * \param fHandle Savefile open for writing
 * \param len Length of data to be written (len <= _size)
 */
void ScriptVars::save(Common::OutSaveFile &fHandle, unsigned int len) const {
	debug(6, "assert(%d <= %d)", len, _size);
	assert(len <= _size);
	for (unsigned int i = 0; i < len; i++) {
		fHandle.writeUint16BE(_vars[i]);
	}
}

/*! \brief Restore array from savefile
 * \param fHandle Savefile open for reading
 */
void ScriptVars::load(Common::InSaveFile &fHandle) {
	load(fHandle, _size);
}

/*! \brief Restore part of array from savefile
 * \param fHandle Savefile open for reading
 * \param len Length of data to be read
 */
void ScriptVars::load(Common::InSaveFile &fHandle, unsigned int len) {
	debug(6, "assert(%d <= %d)", len, _size);
	assert(len <= _size);
	for (unsigned int i = 0; i < len; i++) {
		_vars[i] = fHandle.readUint16BE();
	}
}

/*! \brief Reset all values to 0
 */
void ScriptVars::reset(void) {
	memset( _vars, 0, _size * sizeof(int16));
}

/*! \brief Constructor for partial loading
 * \param s Size of bytecode which will be added later
 *
 * This constructor _MUST_ be followed by setdata() method call before the
 * instance can be used. It leaves the instance in partially invalid state.
 */
RawScript::RawScript(uint16 s) : _size(s), _data(NULL),
	_labels(SCRIPT_STACK_SIZE) { }

/*! \brief Complete constructor
 * \param data Script bytecode
 * \param s Bytecode length
 */
RawScript::RawScript(const FWScriptInfo &info, const byte *data, uint16 s) :
	_size(s), _data(NULL), _labels(SCRIPT_STACK_SIZE) {

	setData(info, data);
}

/*! \brief Copy constructor
 */
RawScript::RawScript(const RawScript &src) : _size(src._size),
	_data(new byte[_size+1]), _labels(src._labels) {

	assert(_data);
	memcpy(_data, src._data, _size+1);
}

/*! \brief Destructor
 */
RawScript::~RawScript(void) {
	delete[] _data;
}

/*! \brief Assignment operator
 */
RawScript &RawScript::operator=(const RawScript &src) {
	assert(src._data);
	byte *tmp = new byte[src._size+1];

	assert(tmp);
	_labels = src._labels;
	_size = src._size;

	delete[] _data;
	_data = tmp;
	memcpy(_data, src._data, _size);
	_data[_size] = 0;

	return *this;
}

/*! \brief Get the next label in bytecode
 * \param info Script info instance
 * \param offset Starting offset
 * \return Index of the next label in bytecode or _size on end of bytecode
 *
 * computeScriptStackSub replacement
 */
int RawScript::getNextLabel(const FWScriptInfo &info, int offset) const {
	assert(_data);
	int pos = offset;

	assert(pos >= 0);

	while (pos < _size) {
		uint8 opcode = _data[pos++];
		const char *ptr = info.opcodeInfo(opcode);

		if (!ptr) {
			continue;
		}

		for (; *ptr; ++ptr) {
			switch (*ptr) {
			case 'b': // byte
				pos++;
				break;
			case 'w': // word
				pos += 2;
				break;
			case 'c': { // byte != 0 ? byte : word
					uint8 test = _data[pos];
					pos++;
					if (test) {
						pos++;
					} else {
						pos += 2;
					}
				}
				break;
			case 'l': // label
				return pos;
			case 's': // string
				while (_data[pos++] != 0);
				break;
			case 'x': // exit script
				return -pos-1;
			}
		}
	}
	return _size;
}

/*! \brief Calculate initial script labels
 * \param info Script info instance
 *
 * computeScriptStack replacement
 */
void RawScript::computeLabels(const FWScriptInfo &info) {
	assert(_data);
	int pos = 0;
	int i;

	// reset labels
	for (i = 0; i < SCRIPT_STACK_SIZE; i++) {
		_labels[i] = -1;
	}

	// parse bytecode
	while ((pos = getNextLabel(info, pos)) >= 0) {
		i = _data[pos];
		_labels[i] = ++pos;
	}
}

/*! \brief find the next label from current position
 * \param info Script info instance
 * \param index Label index to look for
 * \param offset Current position in script
 * \return Position of next instruction following the label
 *
 * computeScriptStackFromScript replacement
 */
uint16 RawScript::getLabel(const FWScriptInfo &info, byte index, uint16 offset)
	const {

	assert(_data);
	int pos = offset;

	while ((pos = getNextLabel(info, pos)) >= 0) {
		if (_data[pos++] == index) {
			return pos;
		}
	}

	return -pos - 1;
}

/*! \brief Copy bytecode and calculate labels
 * \param data Bytecode to copy, must be _size long
 */
void RawScript::setData(const FWScriptInfo &info, const byte *data) {
	assert(!_data); // this function should be called only once per instance
	_data = new byte[_size+1];

	assert(data && _data);
	memcpy(_data, data, _size * sizeof(byte));
	_data[_size] = 0;

	computeLabels(info);
}

/*! \brief Initial script labels
 * \return Precalculated script labels
 */
const ScriptVars &RawScript::labels(void) const {
	assert(_data);
	return _labels;
}

/*! \brief One byte of bytecode
 * \param pos Index in bytecode
 * \return Byte from bytecode
 */
byte RawScript::getByte(unsigned int pos) const {
	assert(_data && pos < _size);

	return _data[pos];
}

/*! \brief One word of bytecode
 * \param pos Index of the first byte in bytecode
 * \return Word of bytecode
 */
uint16 RawScript::getWord(unsigned int pos) const {
	assert(_data && pos+1 < _size);

	return READ_BE_UINT16(_data + pos);
}

/*! \brief String in bytecode
 * \param pos Index of the first char in string
 * \return Pointer to part of bytecode
 */
const char *RawScript::getString(unsigned int pos) const {
	assert(_data && pos < _size);

	return (const char*)(_data+pos);
}

/*! \brief Constructor for partial loading
 * \param size Size of bytecode which will be added later
 * \param p1 First object script parameter
 * \param p2 Second object script parameter
 * \param p3 Third object script parameter
 *
 * This constructor _MUST_ be followed by setdata() method call before the
 * instance can be used. It leaves the instance in partially invalid state.
 */
RawObjectScript::RawObjectScript(uint16 s, uint16 p1, uint16 p2, uint16 p3)
	: RawScript(s), _runCount(0), _param1(p1), _param2(p2), _param3(p3)
{ }

/*! \brief Complete constructor
 * \param data Script bytecode
 * \param s Bytecode length
 * \param p1 First object script parameter
 * \param p2 Second object script parameter
 * \param p3 Third object script parameter
 */
RawObjectScript::RawObjectScript(const FWScriptInfo &info, const byte *data,
	uint16 s, uint16 p1, uint16 p2, uint16 p3) : RawScript(info, data, s),
	_runCount(0), _param1(p1), _param2(p2), _param3(p3) { }

/*! \brief Contructor for global scripts
 * \param script Script bytecode reference
 * \param idx Script bytecode index
 */
FWScript::FWScript(const RawScript &script, int16 idx) : _script(script),
	_pos(0), _line(0), _compare(0), _index(idx),
	_labels(script.labels()), _localVars(LOCAL_VARS_SIZE),
	_globalVars(globalVars), _info(new FWScriptInfo) { }

/*! \brief Copy constructor
 */
FWScript::FWScript(const FWScript &src) : _script(src._script), _pos(src._pos),
	_line(src._line), _compare(src._compare), _index(src._index),
	_labels(src._labels), _localVars(src._localVars),
	_globalVars(src._globalVars), _info(new FWScriptInfo) { }

/*! \brief Contructor for global scripts in derived classes
 * \param script Script bytecode reference
 * \param idx Script bytecode index
 */
FWScript::FWScript(const RawScript &script, int16 idx, FWScriptInfo *info) :
	_script(script), _pos(0), _line(0), _compare(0), _index(idx),
	_labels(script.labels()), _localVars(LOCAL_VARS_SIZE),
	_globalVars(globalVars), _info(info) { }

/*! \brief Constructor for object scripts in derived classes
 * \param script Script bytecode reference
 * \param idx Script bytecode index
 */
FWScript::FWScript(RawObjectScript &script, int16 idx, FWScriptInfo *info) :
	_script(script), _pos(0), _line(0), _compare(0), _index(idx),
	_labels(script.labels()), _localVars(LOCAL_VARS_SIZE),
	_globalVars(globalVars), _info(info) {

	_localVars[0] = script.run();
}

/*! \brief Copy constructor for derived classes
 */
FWScript::FWScript(const FWScript &src, FWScriptInfo *info) :
	_script(src._script), _pos(src._pos), _line(src._line),
	_compare(src._compare), _index(src._index), _labels(src._labels),
	_localVars(src._localVars), _globalVars(src._globalVars), _info(info) { }

FWScript::~FWScript(void) {
	delete _info;
}

/*! \brief Read next byte from bytecode
 * \return Byte from bytecode
 */
byte FWScript::getNextByte() {
	byte val = _script.getByte(_pos);
	_pos++;
	return val;
}

/*! \brief Read next word from bytecode
 * \return Word from bytecode
 */
uint16 FWScript::getNextWord() {
	uint16 val = _script.getWord(_pos);
	_pos += 2;
	return val;
}

/*! \brief Read next string from bytecode
 * \return Pointer to string
 */
const char *FWScript::getNextString() {
	const char *val = _script.getString(_pos);
	_pos += strlen(val) + 1;
	return val;
}

/*! \brief Restore script state from savefile
 * \param labels Restored script labels
 * \param local Restored local script variables
 * \param compare Restored last comparison result
 * \param pos Restored script position
 */
void FWScript::load(const ScriptVars &labels, const ScriptVars &local, uint16 compare, uint16 pos) {
	assert(pos < _script._size);
	_labels = labels;
	_localVars = local;
	_compare = compare;
	_pos = _line = pos;
}

/*! \brief Execute script
 * \return <0 on script termination, >0 on script pause
 *
 * executeScript replacement.
 * Instruction handler must return 0 if the script should continue or
 * nonzero with the same meaning as return value of this function
 */
int FWScript::execute() {
	int ret = 0;

	while (!ret) {
		_line = _pos;
		byte opcode = getNextByte();
		opFunc handler = _info->opcodeHandler(opcode);

		if (handler) {
			ret = (this->*handler)();
		}
	}

	return ret;
}

/*! \brief Save script to savefile
 * \param fHandle Savefile handle
 */
void FWScript::save(Common::OutSaveFile &fHandle) const {
	_labels.save(fHandle);
	_localVars.save(fHandle);
	fHandle.writeUint16BE(_compare);
	fHandle.writeUint16BE(_pos);
	// data order sucks...
	fHandle.writeUint16BE(_index);
}

/*! \brief Contructor for global scripts
 * \param script Script bytecode reference
 * \param idx Script bytecode index
 */
OSScript::OSScript(const RawScript &script, int16 idx) :
	FWScript(script, idx, new OSScriptInfo) {}

/*! \brief Constructor for object scripts
 * \param script Script bytecode reference
 * \param idx Script bytecode index
 */
OSScript::OSScript(RawObjectScript &script, int16 idx) :
	FWScript(script, idx, new OSScriptInfo) {}

/*! \brief Copy constructor
 */
OSScript::OSScript(const OSScript &src) : FWScript(src, new OSScriptInfo) {}

/*! \brief Restore script state from savefile
 * \param labels Restored script labels
 * \param local Restored local script variables
 * \param compare Restored last comparison result
 * \param pos Restored script position
 */
void OSScript::load(const ScriptVars &labels, const ScriptVars &local, uint16 compare, uint16 pos) {
	FWScript::load(labels, local, compare, pos);
}
/*! \brief Get opcode info string
 * \param opcode Opcode to look for in opcode table
 */
const char *FWScriptInfo::opcodeInfo(byte opcode) const {
	if (opcode == 0 || opcode > FWScript::_numOpcodes) {
		return NULL;
	}

	if (!FWScript::_opcodeTable[opcode - 1].args) {
		warning("Undefined opcode 0x%02X in FWScriptInfo::opcodeInfo", opcode - 1);
		return NULL;
	}

	return FWScript::_opcodeTable[opcode - 1].args;
}

/*! \brief Get opcode handler pointer
 * \param opcode Opcode to look for in opcode table
 */
opFunc FWScriptInfo::opcodeHandler(byte opcode) const {
	if (opcode == 0 || opcode > FWScript::_numOpcodes) {
		return NULL;
	}

	if (!FWScript::_opcodeTable[opcode - 1].proc) {
		warning("Undefined opcode 0x%02X in FWScriptInfo::opcodeHandler", opcode - 1);
		return NULL;
	}

	return FWScript::_opcodeTable[opcode - 1].proc;
}

/*! \brief Create new FWScript instance
 * \param script Script bytecode
 * \param index Bytecode index
 */
FWScript *FWScriptInfo::create(const RawScript &script, int16 index) const {
	return new FWScript(script, index);
}

/*! \brief Create new FWScript instance
 * \param script Object script bytecode
 * \param index Bytecode index
 */
FWScript *FWScriptInfo::create(const RawObjectScript &script, int16 index) const {
	return new FWScript(script, index);
}

/*! \brief Load saved FWScript instance
 * \param script Script bytecode
 * \param index Bytecode index
 * \param local Local variables
 * \param labels Script labels
 * \param compare Last compare result
 * \param pos Position in script
 */
FWScript *FWScriptInfo::create(const RawScript &script, int16 index, const ScriptVars &labels, const ScriptVars &local, uint16 compare, uint16 pos) const {
	FWScript *tmp = new FWScript(script, index);
	assert(tmp);
	tmp->load(labels, local, compare, pos);
	return tmp;
}

/*! \brief Load saved FWScript instance
 * \param script Object script bytecode
 * \param index Bytecode index
 * \param local Local variables
 * \param labels Script labels
 * \param compare Last compare result
 * \param pos Position in script
 */
FWScript *FWScriptInfo::create(const RawObjectScript &script, int16 index, const ScriptVars &labels, const ScriptVars &local, uint16 compare, uint16 pos) const {
	FWScript *tmp = new FWScript(script, index);
	assert(tmp);
	tmp->load(labels, local, compare, pos);
	return tmp;
}

/*! \brief Get opcode info string
 * \param opcode Opcode to look for in opcode table
 */
const char *OSScriptInfo::opcodeInfo(byte opcode) const {
	if (opcode == 0 || opcode > OSScript::_numOpcodes) {
		return NULL;
	}

	if (!OSScript::_opcodeTable[opcode - 1].args) {
		warning("Undefined opcode 0x%02X in OSScriptInfo::opcodeInfo", opcode - 1);
		return NULL;
	}

	return OSScript::_opcodeTable[opcode - 1].args;
}

/*! \brief Get opcode handler pointer
 * \param opcode Opcode to look for in opcode table
 */
opFunc OSScriptInfo::opcodeHandler(byte opcode) const {
	if (opcode == 0 || opcode > OSScript::_numOpcodes) {
		return NULL;
	}

	if (!OSScript::_opcodeTable[opcode - 1].proc) {
		warning("Undefined opcode 0x%02X in OSScriptInfo::opcodeHandler", opcode - 1);
		return NULL;
	}

	return OSScript::_opcodeTable[opcode - 1].proc;
}

/*! \brief Create new OSScript instance
 * \param script Script bytecode
 * \param index Bytecode index
 */
FWScript *OSScriptInfo::create(const RawScript &script, int16 index) const {
	return new OSScript(script, index);
}

/*! \brief Create new OSScript instance
 * \param script Object script bytecode
 * \param index Bytecode index
 */
FWScript *OSScriptInfo::create(const RawObjectScript &script, int16 index) const {
	return new OSScript(script, index);
}

/*! \brief Load saved OSScript instance
 * \param script Script bytecode
 * \param index Bytecode index
 * \param local Local variables
 * \param labels Script labels
 * \param compare Last compare result
 * \param pos Position in script
 */
FWScript *OSScriptInfo::create(const RawScript &script, int16 index, const ScriptVars &labels, const ScriptVars &local, uint16 compare, uint16 pos) const {
	OSScript *tmp = new OSScript(script, index);
	assert(tmp);
	tmp->load(labels, local, compare, pos);
	return tmp;
}

/*! \brief Load saved OSScript instance
 * \param script Object script bytecode
 * \param index Bytecode index
 * \param local Local variables
 * \param labels Script labels
 * \param compare Last compare result
 * \param pos Position in script
 */
FWScript *OSScriptInfo::create(const RawObjectScript &script, int16 index, const ScriptVars &labels, const ScriptVars &local, uint16 compare, uint16 pos) const {
	OSScript *tmp = new OSScript(script, index);
	assert(tmp);
	tmp->load(labels, local, compare, pos);
	return tmp;
}

// ------------------------------------------------------------------------
// FUTURE WARS opcodes
// ------------------------------------------------------------------------

int FWScript::o1_modifyObjectParam() {
	byte objIdx = getNextByte();
	byte paramIdx = getNextByte();
	int16 newValue = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: modifyObjectParam(objIdx:%d,paramIdx:%d,newValue:%d)", _line, objIdx, paramIdx, newValue);

	modifyObjectParam(objIdx, paramIdx, newValue);
	return 0;
}

int FWScript::o1_getObjectParam() {
	byte objIdx = getNextByte();
	byte paramIdx = getNextByte();
	byte newValue = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: getObjectParam(objIdx:%d,paramIdx:%d,var:%d)", _line, objIdx, paramIdx, newValue);

	_localVars[newValue] = getObjectParam(objIdx, paramIdx);
	return 0;
}

int FWScript::o1_addObjectParam() {
	byte objIdx = getNextByte();
	byte paramIdx = getNextByte();
	int16 newValue = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: addObjectParam(objIdx:%d,paramIdx:%d,newValue:%d)", _line, objIdx, paramIdx, newValue);

	addObjectParam(objIdx, paramIdx, newValue);
	return 0;
}

int FWScript::o1_subObjectParam() {
	byte objIdx = getNextByte();
	byte paramIdx = getNextByte();
	int16 newValue = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: subObjectParam(objIdx:%d,paramIdx:%d,newValue:%d)", _line, objIdx, paramIdx, newValue);

	subObjectParam(objIdx, paramIdx, newValue);
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o1_add2ObjectParam() {
	uint16 a = getNextByte();
	uint16 b = getNextByte();
	uint16 c = getNextWord();
	warning("STUB: o1_add2ObjectParam(%x, %x, %x)", a, b, c);
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o1_sub2ObjectParam() {
	uint16 a = getNextByte();
	uint16 b = getNextByte();
	uint16 c = getNextWord();
	warning("STUB: o1_sub2ObjectParam(%x, %x, %x)", a, b, c);
	return 0;
}

int FWScript::o1_compareObjectParam() {
	byte objIdx = getNextByte();
	byte param1 = getNextByte();
	int16 param2 = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: compareObjectParam(objIdx:%d,type:%d,value:%d)", _line, objIdx, param1, param2);

	_compare = compareObjectParam(objIdx, param1, param2);
	return 0;
}

int FWScript::o1_setupObject() {
	byte objIdx = getNextByte();
	int16 param1 = getNextWord();
	int16 param2 = getNextWord();
	int16 param3 = getNextWord();
	int16 param4 = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: setupObject(objIdx:%d,%d,%d,%d,%d)", _line, objIdx, param1, param2, param3, param4);

	setupObject(objIdx, param1, param2, param3, param4);
	return 0;
}

int FWScript::o1_checkCollision() {
	byte objIdx = getNextByte();
	int16 param1 = getNextWord();
	int16 param2 = getNextWord();
	int16 param3 = getNextWord();
	int16 param4 = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: checkCollision(objIdx:%d,%d,%d,%d,%d)", _line, objIdx, param1, param2, param3, param4);

	_compare = checkCollision(objIdx, param1, param2, param3, param4);
	return 0;
}

int FWScript::o1_loadVar() {
	byte varIdx = getNextByte();
	byte varType = getNextByte();

	if (varType) {
		byte dataIdx = getNextByte();
		int16 var;

		switch (varType) {
		case 1:
			debugC(5, kCineDebugScript, "Line: %d: var[%d] = var[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] = _localVars[dataIdx];
			break;
		case 2:
			debugC(5, kCineDebugScript, "Line: %d: var[%d] = globalVars[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] = _globalVars[dataIdx];
			break;
		case 3:
			debugC(5, kCineDebugScript, "Line: %d: var[%d] = mouseX", _line, varIdx);
			getMouseData(mouseUpdateStatus, &dummyU16, (uint16 *)&var, &dummyU16);
			_localVars[varIdx] = var;
			break;
		case 4:
			debugC(5, kCineDebugScript, "Line: %d: var[%d] = mouseY", _line, varIdx);
			getMouseData(mouseUpdateStatus, &dummyU16, &dummyU16, (uint16 *)&var);
			_localVars[varIdx] = var;
			break;
		case 5:
			debugC(5, kCineDebugScript, "Line: %d: var[%d] = rand mod %d", _line, varIdx, dataIdx);
			_localVars[varIdx] = g_cine->_rnd.getRandomNumber(dataIdx - 1);
			break;
		case 8:
			debugC(5, kCineDebugScript, "Line: %d: var[%d] = file[%d].packedSize", _line, varIdx, dataIdx);
			_localVars[varIdx] = partBuffer[dataIdx].packedSize;
			break;
		case 9:
			debugC(5, kCineDebugScript, "Line: %d: var[%d] = file[%d].unpackedSize", _line, varIdx, dataIdx);
			_localVars[varIdx] = partBuffer[dataIdx].unpackedSize;
			break;
		default:
			error("executeScript: o1_loadVar: Unknown variable type %d", varType);
		}
	} else {
		int16 value = getNextWord();

		debugC(5, kCineDebugScript, "Line: %d: var[%d] = %d", _line, varIdx, value);
		_localVars[varIdx] = value;
	}

	return 0;
}

int FWScript::o1_addVar() {
	byte varIdx = getNextByte();
	byte varType = getNextByte();

	if (varType) {
		byte dataIdx = getNextByte();

		if (varType == 1) {
			debugC(5, kCineDebugScript, "Line: %d: var[%d] += var[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] += _localVars[dataIdx];
		} else if (varType == 2) {
			debugC(5, kCineDebugScript, "Line: %d: var[%d] += globalVar[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] += _globalVars[dataIdx];
		}
	} else {
		int16 value = getNextWord();

		debugC(5, kCineDebugScript, "Line: %d: var[%d] += %d", _line, varIdx, value);
		_localVars[varIdx] += value;
	}

	return 0;
}

int FWScript::o1_subVar() {
	byte varIdx = getNextByte();
	byte varType = getNextByte();

	if (varType) {
		byte dataIdx = getNextByte();

		if (varType == 1) {
			debugC(5, kCineDebugScript, "Line: %d: var[%d] -= var[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] -= _localVars[dataIdx];
		} else if (varType == 2) {
			debugC(5, kCineDebugScript, "Line: %d: var[%d] -= globalVar[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] -= _globalVars[dataIdx];
		}

	} else {
		int16 value = getNextWord();

		debugC(5, kCineDebugScript, "Line: %d: var[%d] -= %d", _line, varIdx, value);
		_localVars[varIdx] -= value;
	}

	return 0;
}

int FWScript::o1_mulVar() {
	byte varIdx = getNextByte();
	byte varType = getNextByte();

	if (varType) {
		byte dataIdx = getNextByte();

		if (varType == 1) {
			debugC(5, kCineDebugScript, "Line: %d: var[%d] *= var[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] *= _localVars[dataIdx];
		} else if (varType == 2) {
			debugC(5, kCineDebugScript, "Line: %d: var[%d] *= globalVar[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] *= _globalVars[dataIdx];
		}
	} else {
		int16 value = getNextWord();

		debugC(5, kCineDebugScript, "Line: %d: var[%d] *= %d", _line, varIdx, value);
		_localVars[varIdx] *= value;
	}

	return 0;
}

int FWScript::o1_divVar() {
	byte varIdx = getNextByte();
	byte varType = getNextByte();

	if (varType) {
		byte dataIdx = getNextByte();

		if (varType == 1) {
			debugC(5, kCineDebugScript, "Line: %d: var[%d] /= var[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] /= _localVars[dataIdx];
		} else if (varType == 2) {
			debugC(5, kCineDebugScript, "Line: %d: var[%d] /= globalVar[%d]", _line, varIdx, dataIdx);
			_localVars[varIdx] /= _globalVars[dataIdx];
		}
	} else {
		int16 value = getNextWord();

		debugC(5, kCineDebugScript, "Line: %d: var[%d] /= %d", _line, varIdx, value);
		_localVars[varIdx] /= value;
	}

	return 0;
}

int FWScript::o1_compareVar() {
	byte varIdx = getNextByte();
	byte varType = getNextByte();

	if (varType) {
		byte dataIdx = getNextByte();

		if (varType == 1) {
			debugC(5, kCineDebugScript, "Line: %d: compare var[%d] and var[%d]", _line, varIdx, dataIdx);
			_compare = compareVars(_localVars[varIdx], _localVars[dataIdx]);
		} else if (varType == 2) {
			debugC(5, kCineDebugScript, "Line: %d: compare var[%d] and globalVar[%d]", _line, varIdx, dataIdx);
			_compare = compareVars(_localVars[varIdx], _globalVars[dataIdx]);
		}
	} else {
		int16 value = getNextWord();

		debugC(5, kCineDebugScript, "Line: %d: compare var[%d] and %d", _line, varIdx, value);
		_compare = compareVars(_localVars[varIdx], value);
	}

	return 0;
}

int FWScript::o1_modifyObjectParam2() {
	byte objIdx = getNextByte();
	byte paramIdx = getNextByte();
	byte newValue = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: modifyObjectParam2(objIdx:%d,paramIdx:%d,var[%d])", _line, objIdx, paramIdx, newValue);

	modifyObjectParam(objIdx, paramIdx, _localVars[newValue]);
	return 0;
}

int FWScript::o1_loadMask0() {
	// OP_loadV7Element
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: addSpriteOverlay(%d)", _line, param);
	loadOverlayElement(param, 0);
	return 0;
}

int FWScript::o1_unloadMask0() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: removeSpriteOverlay(%d)", _line, param);
	freeOverlay(param, 0);
	return 0;
}

int FWScript::o1_addToBgList() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: addToBGList(%d)", _line, param);
	addToBGList(param);
	return 0;
}

int FWScript::o1_loadMask1() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: addOverlay1(%d)", _line, param);
	loadOverlayElement(param, 1);
	return 0;
}

int FWScript::o1_unloadMask1() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: removeOverlay1(%d)", _line, param);
	freeOverlay(param, 1);
	return 0;
}

int FWScript::o1_loadMask4() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: addOverlayType4(%d)", _line, param);
	loadOverlayElement(param, 4);
	return 0;
}

int FWScript::o1_unloadMask4() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: removeSpriteOverlay4(%d)", _line, param);
	freeOverlay(param, 4);
	return 0;
}

int FWScript::o1_addSpriteFilledToBgList() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: op1A(%d) -> TODO !", _line, param);
	addSpriteFilledToBGList(param);
	return 0;
}

int FWScript::o1_op1B() {
	debugC(5, kCineDebugScript, "Line: %d: freeBgIncrustList", _line);
	bgIncrustList.clear();
	return 0;
}

int FWScript::o1_label() {
	byte labelIdx = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: label(%d)", _line, labelIdx);
	_labels[labelIdx] = _pos;
	return 0;
}

int FWScript::o1_goto() {
	byte labelIdx = getNextByte();

	assert(_labels[labelIdx] != -1);

	debugC(5, kCineDebugScript, "Line: %d: goto label(%d)", _line, labelIdx);
	_pos = _labels[labelIdx];
	return 0;
}

int FWScript::o1_gotoIfSup() {
	byte labelIdx = getNextByte();

	if (_compare == kCmpGT) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(>) goto %d (true)", _line, labelIdx);
		_pos = _labels[labelIdx];
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(>) goto %d (false)", _line, labelIdx);
	}
	return 0;
}

int FWScript::o1_gotoIfSupEqu() {
	byte labelIdx = getNextByte();

	if (_compare & (kCmpGT | kCmpEQ)) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(>=) goto %d (true)", _line, labelIdx);
		_pos = _labels[labelIdx];
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(>=) goto %d (false)", _line, labelIdx);
	}
	return 0;
}

int FWScript::o1_gotoIfInf() {
	byte labelIdx = getNextByte();

	if (_compare == kCmpLT) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(<) goto %d (true)", _line, labelIdx);
		_pos = _labels[labelIdx];
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(<) goto %d (false)", _line, labelIdx);
	}
	return 0;
}

int FWScript::o1_gotoIfInfEqu() {
	byte labelIdx = getNextByte();

	if (_compare & (kCmpLT | kCmpEQ)) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(<=) goto %d (true)", _line, labelIdx);
		_pos = _labels[labelIdx];
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(<=) goto %d (false)", _line, labelIdx);
	}
	return 0;
}

int FWScript::o1_gotoIfEqu() {
	byte labelIdx = getNextByte();

	if (_compare == kCmpEQ) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(==) goto %d (true)", _line, labelIdx);
		_pos = _labels[labelIdx];
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(==) goto %d (false)", _line, labelIdx);
	}
	return 0;
}

int FWScript::o1_gotoIfDiff() {
	byte labelIdx = getNextByte();

	if (_compare != kCmpEQ) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(!=) goto %d (true)", _line, labelIdx);
		_pos = _labels[labelIdx];
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(!=) goto %d (false)", _line, labelIdx);
	}
	return 0;
}

int FWScript::o1_removeLabel() {
	byte labelIdx = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: removeLabel(%d)", _line, labelIdx);
	_labels[labelIdx] = -1;
	return 0;
}

int FWScript::o1_loop() {
	byte varIdx = getNextByte();
	byte labelIdx = getNextByte();

	_localVars[varIdx]--;

	if (_localVars[varIdx] >= 0) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: loop(var[%d]) goto %d (continue)", _line, varIdx, labelIdx);
		_pos = _labels[labelIdx];
	} else {
		debugC(5, kCineDebugScript, "Line: %d: loop(var[%d]) goto %d (stop)", _line, varIdx, labelIdx);
	}
	return 0;
}

int FWScript::o1_startGlobalScript() {
	// OP_startScript
	byte param = getNextByte();

	assert(param < NUM_MAX_SCRIPT);

	debugC(5, kCineDebugScript, "Line: %d: startScript(%d)", _line, param);
	addScriptToList0(param);
	return 0;
}

int FWScript::o1_endGlobalScript() {
	byte scriptIdx = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: stopGlobalScript(%d)", _line, scriptIdx);

	ScriptList::iterator it = globalScripts.begin();

	for (; it != globalScripts.end(); ++it) {
		if ((*it)->_index == scriptIdx) {
			(*it)->_index = -1;
		}
	}

	return 0;
}

int FWScript::o1_loadAnim() {
	// OP_loadResource
	const char *param = getNextString();

	debugC(5, kCineDebugScript, "Line: %d: loadResource(\"%s\")", _line, param);
	loadResource(param);
	return 0;
}

int FWScript::o1_loadBg() {
	const char *param = getNextString();

	debugC(5, kCineDebugScript, "Line: %d: loadBg(\"%s\")", _line, param);

	loadBg(param);
	bgIncrustList.clear();
	bgVar0 = 0;
	return 0;
}

int FWScript::o1_loadCt() {
	const char *param = getNextString();

	debugC(5, kCineDebugScript, "Line: %d: loadCt(\"%s\")", _line, param);
	loadCt(param);
	return 0;
}

int FWScript::o1_loadPart() {
	const char *param = getNextString();

	debugC(5, kCineDebugScript, "Line: %d: loadPart(\"%s\")", _line, param);
	loadPart(param);
	return 0;
}

int FWScript::o1_closePart() {
	debugC(5, kCineDebugScript, "Line: %d: closePart", _line);
	closePart();
	return 0;
}

int FWScript::o1_loadNewPrcName() {
	// OP_loadData
	byte param1 = getNextByte();
	const char *param2 = getNextString();

	assert(param1 <= 3);

	switch (param1) {
	case 0:
		debugC(5, kCineDebugScript, "Line: %d: loadPrc(\"%s\")", _line, param2);
		strcpy(newPrcName, param2);
		break;
	case 1:
		debugC(5, kCineDebugScript, "Line: %d: loadRel(\"%s\")", _line, param2);
		strcpy(newRelName, param2);
		break;
	case 2:
		debugC(5, kCineDebugScript, "Line: %d: loadObject(\"%s\")", _line, param2);
		strcpy(newObjectName, param2);
		break;
	case 3:
		debugC(5, kCineDebugScript, "Line: %d: loadMsg(\"%s\")", _line, param2);
		strcpy(newMsgName, param2);
		break;
	}
	return 0;
}

int FWScript::o1_requestCheckPendingDataLoad() {
	debugC(5, kCineDebugScript, "Line: %d: request data load", _line);
	checkForPendingDataLoadSwitch = 1;
	return 0;
}

int FWScript::o1_blitAndFade() {
	debugC(5, kCineDebugScript, "Line: %d: request fadein", _line);
	// TODO: use real code

	drawOverlays();
	fadeRequired = true;
	flip();

//	fadeFromBlack();
	return 0;
}

int FWScript::o1_fadeToBlack() {
	debugC(5, kCineDebugScript, "Line: %d: request fadeout", _line);

	fadeToBlack();
	return 0;
}

int FWScript::o1_transformPaletteRange() {
	byte startColor = getNextByte();
	byte numColor = getNextByte();
	uint16 r = getNextWord();
	uint16 g = getNextWord();
	uint16 b = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: transformPaletteRange(from:%d,numIdx:%d,r:%d,g:%d,b:%d)", _line, startColor, numColor, r, g, b);

	transformPaletteRange(startColor, numColor, r, g, b);
	return 0;
}

int FWScript::o1_setDefaultMenuColor2() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: setDefaultMenuColor2(%d)", _line, param);
	defaultMenuBoxColor2 = param;
	return 0;
}

int FWScript::o1_palRotate() {
	byte a = getNextByte();
	byte b = getNextByte();
	byte c = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: palRotate(%d,%d,%d)", _line, a, b, c);
	palRotate(a, b, c);
	return 0;
}

/*!\brief Pause script
 * \todo Make sure it works
 */
int FWScript::o1_break() {
	debugC(5, kCineDebugScript, "Line: %d: break", _line);

	return 1;
}

/*! \brief Terminate script
 * \todo Make sure it works
 */
int FWScript::o1_endScript() {
	debugC(5, kCineDebugScript, "Line: %d: endScript", _line);

	return -1;
}

int FWScript::o1_message() {
	byte param1 = getNextByte();
	uint16 param2 = getNextWord();
	uint16 param3 = getNextWord();
	uint16 param4 = getNextWord();
	uint16 param5 = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: message(%d,%d,%d,%d,%d)", _line, param1, param2, param3, param4, param5);

	addMessage(param1, param2, param3, param4, param5);
	return 0;
}

int FWScript::o1_loadGlobalVar() {
	byte varIdx = getNextByte();
	byte varType = getNextByte();

	if (varType) {
		byte dataIdx = getNextByte();

		if (varType == 1) {
			debugC(5, kCineDebugScript, "Line: %d: globalVars[%d] = var[%d]", _line, varIdx, dataIdx);
			_globalVars[varIdx] = _localVars[dataIdx];
		} else {
			debugC(5, kCineDebugScript, "Line: %d: globalVars[%d] = globalVars[%d]", _line, varIdx, dataIdx);
			_globalVars[varIdx] = _globalVars[dataIdx];
		}
	} else {
		uint16 value = getNextWord();

		debugC(5, kCineDebugScript, "Line: %d: globalVars[%d] = %d", _line, varIdx, value);
		_globalVars[varIdx] = value;
	}

	return 0;
}

int FWScript::o1_compareGlobalVar() {
	byte varIdx = getNextByte();
	byte varType = getNextByte();

	if (varType) {
		byte value = getNextByte();

		if (varType == 1) {
			debugC(5, kCineDebugScript, "Line: %d: compare globalVars[%d] and var[%d]", _line, varIdx, value);
			_compare = compareVars(_globalVars[varIdx], _localVars[value]);
		} else {
			debugC(5, kCineDebugScript, "Line: %d: compare globalVars[%d] and globalVars[%d]", _line, varIdx, value);
			_compare = compareVars(_globalVars[varIdx], _globalVars[value]);
		}
	} else {
		uint16 value = getNextWord();

		debugC(5, kCineDebugScript, "Line: %d: compare globalVars[%d] and %d", _line, varIdx, value);

		if (varIdx == 255 && (g_cine->getGameType() == Cine::GType_FW)) {	// TODO: fix
			_compare = 1;
		} else {
			_compare = compareVars(_globalVars[varIdx], value);
		}
	}

	return 0;
}

int FWScript::o1_declareFunctionName() {
	const char *param = getNextString();

	debugC(5, kCineDebugScript, "Line: %d: comment(%s)", _line, param);
	return 0;
}

int FWScript::o1_freePartRange() {
	byte startIdx = getNextByte();
	byte numIdx = getNextByte();

	assert(startIdx + numIdx <= NUM_MAX_ANIMDATA);

	debugC(5, kCineDebugScript, "Line: %d: freePartRange(%d,%d)", _line, startIdx, numIdx);
	freeAnimDataRange(startIdx, numIdx);
	return 0;
}

int FWScript::o1_unloadAllMasks() {
	debugC(5, kCineDebugScript, "Line: %d: unloadAllMasks()", _line);
	unloadAllMasks();
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o1_setScreenDimensions() {
	uint16 a = getNextWord();
	uint16 b = getNextWord();
	uint16 c = getNextWord();
	uint16 d = getNextWord();
	warning("STUB: o1_setScreenDimensions(%x, %x, %x, %x)", a, b, c, d);
	// setupScreenParam
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o1_displayBackground() {
	warning("STUB: o1_displayBackground()");
	return 0;
}

int FWScript::o1_initializeZoneData() {
	debugC(5, kCineDebugScript, "Line: %d: initializeZoneData()", _line);

	for (int i = 0; i < NUM_MAX_ZONE; i++) {
		zoneData[i] = i;
	}
	return 0;
}

int FWScript::o1_setZoneDataEntry() {
	byte zoneIdx = getNextByte();
	uint16 var = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: setZone[%d] = %d", _line, zoneIdx, var);
	zoneData[zoneIdx] = var;
	return 0;
}

int FWScript::o1_getZoneDataEntry() {
	byte zoneIdx = getNextByte();
	byte var = getNextByte();

	_localVars[var] = zoneData[zoneIdx];
	return 0;
}

int FWScript::o1_setDefaultMenuColor() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: setDefaultMenuColor(%d)", _line, param);
	defaultMenuBoxColor = param;
	return 0;
}

int FWScript::o1_allowPlayerInput() {
	debugC(5, kCineDebugScript, "Line: %d: allowPlayerInput()", _line);
	allowPlayerInput = 1;
	return 0;
}

int FWScript::o1_disallowPlayerInput() {
	debugC(5, kCineDebugScript, "Line: %d: dissallowPlayerInput()", _line);
	allowPlayerInput = 0;
	return 0;
}

int FWScript::o1_changeDataDisk() {
	byte newDisk = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: changeDataDisk(%d)", _line, newDisk);
	checkDataDisk(newDisk);
	return 0;
}

int FWScript::o1_loadMusic() {
	const char *param = getNextString();

	debugC(5, kCineDebugScript, "Line: %d: loadMusic(%s)", _line, param);
	g_sound->loadMusic(param);
	return 0;
}

int FWScript::o1_playMusic() {
	debugC(5, kCineDebugScript, "Line: %d: playMusic()", _line);
	g_sound->playMusic();
	return 0;
}

int FWScript::o1_fadeOutMusic() {
	debugC(5, kCineDebugScript, "Line: %d: fadeOutMusic()", _line);
	g_sound->fadeOutMusic();
	return 0;
}

int FWScript::o1_stopSample() {
	debugC(5, kCineDebugScript, "Line: %d: stopSample()", _line);
	g_sound->stopMusic();
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o1_op71() {
	byte a = getNextByte();
	uint16 b = getNextWord();
	warning("STUB: o1_op71(%x, %x)", a, b);
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o1_op72() {
	uint16 a = getNextWord();
	byte b = getNextByte();
	uint16 c = getNextWord();
	warning("STUB: o1_op72(%x, %x, %x)", a, b, c);
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o1_op73() {
	// I believe this opcode is identical to o1_op72(). In fact, Operation
	// Stealth doesn't even have it. It uses o1_op72() instead.
	uint16 a = getNextWord();
	byte b = getNextByte();
	uint16 c = getNextWord();
	warning("STUB: o1_op72(%x, %x, %x)", a, b, c);
	return 0;
}

int FWScript::o1_playSample() {
	debugC(5, kCineDebugScript, "Line: %d: playSample()", _line);

	byte anim = getNextByte();
	byte channel = getNextByte();

	uint16 freq = getNextWord();
	byte repeat = getNextByte();

	int16 volume = getNextWord();
	uint16 size = getNextWord();

	const byte *data = animDataTable[anim].data();

	if (!data) {
		return 0;
	}

	if (g_cine->getPlatform() == Common::kPlatformAmiga || g_cine->getPlatform() == Common::kPlatformAtariST) {
		if (size == 0xFFFF) {
			size = animDataTable[anim]._width * animDataTable[anim]._height;
		}
		if (channel < 10) { // || _currentOpcode == 0x78
			int channel1, channel2;
			if (channel == 0) {
				channel1 = 0;
				channel2 = 1;
			} else {
				channel1 = 2;
				channel2 = 3;
			}
			g_sound->playSound(channel1, freq, data, size, -1, volume, 63, repeat);
			g_sound->playSound(channel2, freq, data, size,  1, volume,  0, repeat);
		} else {
			channel -= 10;
			if (volume > 63) {
				volume = 63;
			}
			g_sound->playSound(channel, freq, data, size, 0, 0, volume, repeat);
		}
	} else {
		if (volume > 63 || volume < 0) {
			volume = 63;
		}
		if (channel >= 10) {
			channel -= 10;
		}
		if (volume < 50) {
			volume = 50;
		}
		if (g_cine->getGameType() == Cine::GType_OS && size == 0) {
			return 0;
		}
		g_sound->stopMusic();
		if (size == 0xFFFF) {
			g_sound->playSound(channel, 0, data, 0, 0, 0, volume, 0);
		} else {
			g_sound->stopSound(channel);
		}
	}
	return 0;
}

int FWScript::o1_disableSystemMenu() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: disableSystemMenu(%d)", _line, param);
	disableSystemMenu = (param != 0);
	return 0;
}

int FWScript::o1_loadMask5() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: addOverlay5(%d)", _line, param);
	loadOverlayElement(param, 5);
	return 0;
}

int FWScript::o1_unloadMask5() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: freeOverlay5(%d)", _line, param);
	freeOverlay(param, 5);
	return 0;
}

// ------------------------------------------------------------------------
// OPERATION STEALTH opcodes
// ------------------------------------------------------------------------

int FWScript::o2_loadPart() {
	const char *param = getNextString();

	debugC(5, kCineDebugScript, "Line: %d: loadPart(\"%s\")", _line, param);
	return 0;
}

int FWScript::o2_playSample() {
	if (g_cine->getPlatform() == Common::kPlatformAmiga || g_cine->getPlatform() == Common::kPlatformAtariST) {
		// no-op in these versions
		getNextByte();
		getNextByte();
		getNextWord();
		getNextByte();
		getNextWord();
		getNextWord();
		return 0;
	}
	return o1_playSample();
}

int FWScript::o2_playSampleAlt() {
	byte num = getNextByte();
	byte channel = getNextByte();
	uint16 frequency = getNextWord();
	getNextByte();
	getNextWord();
	uint16 size = getNextWord();

	if (size == 0xFFFF) {
		size = animDataTable[num]._width * animDataTable[num]._height;
	}
	if (animDataTable[num].data()) {
		if (g_cine->getPlatform() == Common::kPlatformPC) {
			// if speaker output is available, play sound on it
			// if it's another device, don't play anything
			// TODO: implement this, it's used in the introduction for example
			// on each letter displayed
		} else {
			g_sound->playSound(channel, frequency, animDataTable[num].data(), size, 0, 0, 63, 0);
		}
	}
	return 0;
}

int FWScript::o2_addSeqListElement() {
	byte param1 = getNextByte();
	byte param2 = getNextByte();
	byte param3 = getNextByte();
	byte param4 = getNextByte();
	uint16 param5 = getNextWord();
	uint16 param6 = getNextWord();
	uint16 param7 = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: addSeqListElement(%d,%d,%d,%d,%d,%d,%d)", _line, param1, param2, param3, param4, param5, param6, param7);
	addSeqListElement(param1, 0, param2, param3, param4, param5, param6, 0, param7);
	return 0;
}

int FWScript::o2_removeSeq() {
	byte a = getNextByte();
	byte b = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: removeSeq(%d,%d) -> TODO", _line, a, b);
	removeSeq(a, 0, b);
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_op81() {
	warning("STUB: o2_op81()");
	// freeUnkList();
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_op82() {
	byte a = getNextByte();
	byte b = getNextByte();
	uint16 c = getNextWord();
	warning("STUB: o2_op82(%x, %x, %x)", a, b, c);
	return 0;
}

int FWScript::o2_isSeqRunning() {
	byte a = getNextByte();
	byte b = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: OP83(%d,%d) -> TODO", _line, a, b);

	if (isSeqRunning(a, 0, b)) {
		_compare = 1;
	} else {
		_compare = 0;
	}
	return 0;
}

/*! \todo The assert may produce false positives and requires testing
 */
int FWScript::o2_gotoIfSupNearest() {
	byte labelIdx = getNextByte();

	if (_compare == kCmpGT) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(>) goto nearest %d (true)", _line, labelIdx);
		_pos = _script.getLabel(*_info, labelIdx, _pos);
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(>) goto nearest %d (false)", _line, labelIdx);
	}
	return 0;
}

/*! \todo The assert may produce false positives and requires testing
 */
int FWScript::o2_gotoIfSupEquNearest() {
	byte labelIdx = getNextByte();

	if (_compare & (kCmpGT | kCmpEQ)) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(>=) goto nearest %d (true)", _line, labelIdx);
		_pos = _script.getLabel(*_info, labelIdx, _pos);
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(>=) goto nearest %d (false)", _line, labelIdx);
	}
	return 0;
}

/*! \todo The assert may produce false positives and requires testing
 */
int FWScript::o2_gotoIfInfNearest() {
	byte labelIdx = getNextByte();

	if (_compare == kCmpLT) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(<) goto nearest %d (true)", _line, labelIdx);
		_pos = _script.getLabel(*_info, labelIdx, _pos);
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(<) goto nearest %d (false)", _line, labelIdx);
	}
	return 0;
}

/*! \todo The assert may produce false positives and requires testing
 */
int FWScript::o2_gotoIfInfEquNearest() {
	byte labelIdx = getNextByte();

	if (_compare & (kCmpLT | kCmpEQ)) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(<=) goto nearest %d (true)", _line, labelIdx);
		_pos = _script.getLabel(*_info, labelIdx, _pos);
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(<=) goto nearest %d (false)", _line, labelIdx);
	}
	return 0;
}

/*! \todo The assert may produce false positives and requires testing
 */
int FWScript::o2_gotoIfEquNearest() {
	byte labelIdx = getNextByte();

	if (_compare == kCmpEQ) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(==) goto nearest %d (true)", _line, labelIdx);
		_pos = _script.getLabel(*_info, labelIdx, _pos);
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(==) goto nearest %d (false)", _line, labelIdx);
	}
	return 0;
}

/*! \todo The assert may produce false positives and requires testing
 */
int FWScript::o2_gotoIfDiffNearest() {
	byte labelIdx = getNextByte();

	if (_compare != kCmpEQ) {
		assert(_labels[labelIdx] != -1);

		debugC(5, kCineDebugScript, "Line: %d: if(!=) goto nearest %d (true)", _line, labelIdx);
		_pos = _script.getLabel(*_info, labelIdx, _pos);
	} else {
		debugC(5, kCineDebugScript, "Line: %d: if(!=) goto nearest %d (false)", _line, labelIdx);
	}
	return 0;
}

int FWScript::o2_startObjectScript() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: startObjectScript(%d)", _line, param);
	runObjectScript(param);
	return 0;
}

int FWScript::o2_stopObjectScript() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: stopObjectScript(%d)", _line, param);
	ScriptList::iterator it = objectScripts.begin();

	for (; it != objectScripts.end(); ++it) {
		if ((*it)->_index == param) {
			(*it)->_index = -1;
		}
	}
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_op8D() {
	uint16 a = getNextWord();
	uint16 b = getNextWord();
	uint16 c = getNextWord();
	uint16 d = getNextWord();
	uint16 e = getNextWord();
	uint16 f = getNextWord();
	uint16 g = getNextWord();
	uint16 h = getNextWord();
	warning("STUB: o2_op8D(%x, %x, %x, %x, %x, %x, %x, %x)", a, b, c, d, e, f, g, h);
	// _currentScriptElement->compareResult = ...
	return 0;
}

int FWScript::o2_addBackground() {
	byte param1 = getNextByte();
	const char *param2 = getNextString();

	debugC(5, kCineDebugScript, "Line: %d: addBackground(%s,%d)", _line, param2, param1);
	addBackground(param2, param1);
	return 0;
}

int FWScript::o2_removeBackground() {
	byte param = getNextByte();

	assert(param);

	debugC(5, kCineDebugScript, "Line: %d: removeBackground(%d)", _line, param);

	if (additionalBgTable[param]) {
		free(additionalBgTable[param]);
		additionalBgTable[param] = NULL;
	}

	if (currentAdditionalBgIdx == param) {
		currentAdditionalBgIdx = 0;
	}

	if (currentAdditionalBgIdx2 == param) {
		currentAdditionalBgIdx2 = 0;
	}

	strcpy(currentBgName[param], "");
	return 0;
}

int FWScript::o2_loadAbs() {
	byte param1 = getNextByte();
	const char *param2 = getNextString();

	debugC(5, kCineDebugScript, "Line: %d: loadABS(%d,%s)", _line, param1, param2);
	loadAbs(param2, param1);
	return 0;
}

int FWScript::o2_loadBg() {
	byte param = getNextByte();

	assert(param <= 8);

	debugC(5, kCineDebugScript, "Line: %d: useBg(%d)", _line, param);

	if (additionalBgTable[param]) {
		currentAdditionalBgIdx = param;
		if (param == 8) {
			newColorMode = 3;
		} else {
			newColorMode = bgColorMode + 1;
		}
		//if (_screenNeedFadeOut == 0) {
		//	adBgVar1 = 1;
		//}
		fadeRequired = true;
	}
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_wasZoneChecked() {
	warning("STUB: o2_wasZoneChecked()");
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_op9B() {
	uint16 a = getNextWord();
	uint16 b = getNextWord();
	uint16 c = getNextWord();
	uint16 d = getNextWord();
	uint16 e = getNextWord();
	uint16 f = getNextWord();
	uint16 g = getNextWord();
	uint16 h = getNextWord();
	warning("STUB: o2_op9B(%x, %x, %x, %x, %x, %x, %x, %x)", a, b, c, d, e, f, g, h);
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_op9C() {
	uint16 a = getNextWord();
	uint16 b = getNextWord();
	uint16 c = getNextWord();
	uint16 d = getNextWord();
	warning("STUB: o2_op9C(%x, %x, %x, %x)", a, b, c, d);
	return 0;
}

int FWScript::o2_useBgScroll() {
	byte param = getNextByte();

	assert(param <= 8);

	debugC(5, kCineDebugScript, "Line: %d: useBgScroll(%d)", _line, param);

	if (additionalBgTable[param]) {
		currentAdditionalBgIdx2 = param;
	}
	return 0;
}

int FWScript::o2_setAdditionalBgVScroll() {
	byte param1 = getNextByte();

	if (param1) {
		byte param2 = getNextByte();

		debugC(5, kCineDebugScript, "Line: %d: additionalBgVScroll = var[%d]", _line, param2);
		additionalBgVScroll = _localVars[param2];
	} else {
		uint16 param2 = getNextWord();

		debugC(5, kCineDebugScript, "Line: %d: additionalBgVScroll = %d", _line, param2);
		additionalBgVScroll = param2;
	}
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_op9F() {
	warning("o2_op9F()");
	getNextWord();
	getNextWord();
	return 0;
}

int FWScript::o2_addGfxElementA0() {
	uint16 param1 = getNextWord();
	uint16 param2 = getNextWord();

	debugC(5, kCineDebugScript, "Line: %d: addGfxElementA0(%d,%d)", _line, param1, param2);
	addGfxElementA0(param1, param2);
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_removeGfxElementA0() {
	uint16 idx = getNextWord();
	uint16 param = getNextWord();
	warning("STUB? o2_removeGfxElementA0(%x, %x)", idx, param);
	removeGfxElementA0(idx, param);
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_opA2() {
	uint16 a = getNextWord();
	uint16 b = getNextWord();
	warning("STUB: o2_opA2(%x, %x)", a, b);
	// addGfxElementA2();
	return 0;
}

/*! \todo Implement this instruction
 */
int FWScript::o2_opA3() {
	uint16 a = getNextWord();
	uint16 b = getNextWord();
	warning("STUB: o2_opA3(%x, %x)", a, b);
	// removeGfxElementA2();
	return 0;
}

int FWScript::o2_loadMask22() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: addOverlay22(%d)", _line, param);
	loadOverlayElement(param, 22);
	return 0;
}

int FWScript::o2_unloadMask22() {
	byte param = getNextByte();

	debugC(5, kCineDebugScript, "Line: %d: removeOverlay22(%d)", _line, param);
	freeOverlay(param, 22);
	return 0;
}

//-----------------------------------------------------------------------

void addGfxElementA0(int16 param1, int16 param2) {
	overlayHeadElement *currentHead = &overlayHead;
	overlayHeadElement *tempHead = currentHead;
	overlayHeadElement *newElement;

	currentHead = tempHead->next;

	while (currentHead) {
		if (objectTable[currentHead->objIdx].mask == objectTable[param1].mask) {
			if (currentHead->type == 2 || currentHead->objIdx == 3) {
				break;
			}
		}

		tempHead = currentHead;
		currentHead = currentHead->next;
	}

	if (currentHead && currentHead->objIdx == param1 && currentHead->type == 20 && currentHead->x == param2)
		return;

	newElement = new overlayHeadElement;

	newElement->next = tempHead->next;
	tempHead->next = newElement;

	newElement->objIdx = param1;
	newElement->type = 20;

	newElement->x = param2;
	newElement->y = 0;
	newElement->width = 0;
	newElement->color = 0;

	if (!currentHead)
		currentHead = &overlayHead;

	newElement->previous = currentHead->previous;

	currentHead->previous = newElement;
}

/*! \todo Check that it works
 */
void removeGfxElementA0(int16 idx, int16 param) {
	overlayHeadElement *parent = &overlayHead;
	overlayHeadElement *head = overlayHead.next;
	overlayHeadElement *tmp;

	while (head) {
		if (head->objIdx == idx && head->x == param) {
			parent->next = head->next;
			tmp = head->next ? head->next : &overlayHead;
			tmp->previous = parent;
			delete head;
			return;
		}

		parent = head;
		head = head->next;
	}
}

void removeSeq(uint16 param1, uint16 param2, uint16 param3) {
	SeqListElement *currentHead = &seqList;
	SeqListElement *tempHead = currentHead;

	while (currentHead && (currentHead->var6 != param1 || currentHead->var4 != param2 || currentHead->varE != param3)) {
		tempHead = currentHead;
		currentHead = tempHead->next;
	}

	if (currentHead && currentHead->var6 == param1 && currentHead->var4 == param2 && currentHead->varE == param3) {
		currentHead->var4 = -1;
	}
}

uint16 isSeqRunning(uint16 param1, uint16 param2, uint16 param3) {
	SeqListElement *currentHead = &seqList;
	SeqListElement *tempHead = currentHead;

	while (currentHead && (currentHead->var6 != param1 || currentHead->var4 != param2 || currentHead->varE != param3)) {
		tempHead = currentHead;
		currentHead = tempHead->next;
	}

	if (currentHead && currentHead->var6 == param1 && currentHead->var4 == param2 && currentHead->varE == param3) {
		return 1;
	}

	return 0;
}

void palRotate(byte a, byte b, byte c) {
	if (c == 1) {
		uint16 currentColor = c_palette[b];

		for (int16 i = b; i > a; i--) {
			c_palette[i] = c_palette[i - 1];
		}

		c_palette[a] = currentColor;
	}
}

void addScriptToList0(uint16 idx) {
	ScriptPtr tmp(scriptInfo->create(*scriptTable[idx], idx));
	assert(tmp);
	globalScripts.push_back(tmp);
}

int16 getZoneFromPosition(byte *page, int16 x, int16 y, int16 width) {
	byte *ptr = page + (y * width) + x / 2;
	byte zoneVar;

	if (!(x % 2)) {
		zoneVar = (*(ptr) >> 4) & 0xF;
	} else {
		zoneVar = (*(ptr)) & 0xF;
	}

	return zoneVar;
}

int16 getZoneFromPositionRaw(byte *page, int16 x, int16 y, int16 width) {
	byte *ptr = page + (y * width) + x;
	byte zoneVar;

	zoneVar = (*(ptr)) & 0xF;

	return zoneVar;
}

int16 checkCollision(int16 objIdx, int16 x, int16 y, int16 numZones, int16 zoneIdx) {
	int16 lx = objectTable[objIdx].x + x;
	int16 ly = objectTable[objIdx].y + y;
	int16 idx;

	for (int16 i = 0; i < numZones; i++) {
		idx = getZoneFromPositionRaw(page3Raw, lx + i, ly, 320);

		assert(idx >= 0 && idx <= NUM_MAX_ZONE);

		if (zoneData[idx] == zoneIdx) {
			return 1;
		}
	}

	return 0;
}

uint16 compareVars(int16 a, int16 b) {
	uint16 flag = 0;

	if (a == b) {
		flag |= kCmpEQ;
	} else if (a > b) {
		flag |= kCmpGT;
	} else if (a < b) {
		flag |= kCmpLT;
	}

	return flag;
}

void executeList1(void) {
	ScriptList::iterator it = objectScripts.begin();
	for (; it != objectScripts.end();) {
		if ((*it)->_index < 0 || (*it)->execute() < 0) {
			it = objectScripts.erase(it);
		} else {
			++it;
		}
	}
}

void executeList0(void) {
	ScriptList::iterator it = globalScripts.begin();
	for (; it != globalScripts.end();) {
		if ((*it)->_index < 0 || (*it)->execute() < 0) {
			it = globalScripts.erase(it);
		} else {
			++it;
		}
	}
}

/*! \todo objectScripts.clear()?
 */
void purgeList1(void) {
}

void purgeList0(void) {
}

////////////////////////////////////
// SCRIPT DECOMPILER

#ifdef DUMP_SCRIPTS

char decompileBuffer[10000][1000];
uint16 decompileBufferPosition = 0;

char bufferDec[256];

char compareString1[256];
char compareString2[256];

const char *getObjPramName(byte paramIdx) {
	switch (paramIdx) {
	case 1:
		return ".X";
	case 2:
		return ".Y";
	case 3:
		return ".mask";
	case 4:
		return ".frame";
	case 5:
		return ".status";
	case 6:
		return ".costume";
	default:
		sprintf(bufferDec, ".param%d", paramIdx);
		return bufferDec;
	}
}

void decompileScript(byte *scriptPtr, int16 *stackPtr, uint16 scriptSize, uint16 scriptIdx) {
	char lineBuffer[256];
	byte *localScriptPtr = scriptPtr;
	uint16 exitScript;
	uint32 position = 0;

	assert(scriptPtr);
	// assert(stackPtr);

	exitScript = 0;

	sprintf(decompileBuffer[decompileBufferPosition++], "--------- SCRIPT %d ---------\n", scriptIdx);

	do {
		uint16 opcode = *(localScriptPtr + position);
		position++;

		if (position == scriptSize) {
			opcode = 0;
		}

		strcpy(lineBuffer, "");

		switch (opcode - 1) {
		case -1:
			{
				break;
			}
		case 0x0:
			{
				byte param1;
				byte param2;
				int16 param3;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				param3 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				sprintf(lineBuffer, "obj[%d]%s = %d\n", param1, getObjPramName(param2), param3);

				break;
			}
		case 0x1:
			{
				byte param1;
				byte param2;
				byte param3;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				param3 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "var[%d]=obj[%d]%s\n", param3, param1, getObjPramName(param2));
				break;
			}
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
			{
				byte param1;
				byte param2;
				int16 param3;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				param3 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				if (opcode - 1 == 0x2) {
					sprintf(lineBuffer, "obj[%d]%s+=%d\n", param1, getObjPramName(param2), param3);
				} else if (opcode - 1 == 0x3) {
					sprintf(lineBuffer, "obj[%d]%s-=%d\n", param1, getObjPramName(param2), param3);
				} else if (opcode - 1 == 0x4) {
					sprintf(lineBuffer, "obj[%d]%s+=obj[%d]%s\n", param1, getObjPramName(param2), param3, getObjPramName(param2));
				} else if (opcode - 1 == 0x5) {
					sprintf(lineBuffer, "obj[%d]%s-=obj[%d]%s\n", param1, getObjPramName(param2), param3, getObjPramName(param2));
				} else if (opcode - 1 == 0x6) {
					sprintf(compareString1, "obj[%d]%s", param1, getObjPramName(param2));
					sprintf(compareString2, "%d", param3);
				}
				break;
			}
		case 0x7:
		case 0x8:
			{
				byte param1;
				int16 param2;
				int16 param3;
				int16 param4;
				int16 param5;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param3 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param4 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param5 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				if (opcode - 1 == 0x7) {
					sprintf(lineBuffer, "setupObject(Idx:%d,X:%d,Y:%d,mask:%d,frame:%d)\n", param1, param2, param3, param4, param5);
				} else if (opcode - 1 == 0x8) {
					sprintf(lineBuffer, "checkCollision(%d,%d,%d,%d,%d)\n", param1, param2, param3, param4, param5);
				}
				break;
			}
		case 0x9:
			{
				byte param1;
				int16 param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				if (param2) {
					byte param3;

					param3 = *(localScriptPtr + position);
					position++;

					if (param2 == 1) {
						sprintf(lineBuffer, "var[%d]=var[%d]\n", param1, param3);
					} else if (param2 == 2) {
						sprintf(lineBuffer, "var[%d]=globalVar[%d]\n", param1, param3);
					} else if (param2 == 3) {
						sprintf(lineBuffer, "var[%d]=mouse.X\n", param1);
					} else if (param2 == 4) {
						sprintf(lineBuffer, "var[%d]=mouse.Y\n", param1);
					} else if (param2 == 5) {
						sprintf(lineBuffer, "var[%d]=rand() mod %d\n", param1, param3);
					} else if (param2 == 8) {
						sprintf(lineBuffer, "var[%d]=file[%d].packedSize\n", param1, param3);
					} else if (param2 == 9) {
						sprintf(lineBuffer, "var[%d]=file[%d].unpackedSize\n", param1, param3);
					} else {
						error("decompileScript: 0x09: param2 = %d", param2);
					}
				} else {
					int16 param3;

					param3 = READ_BE_UINT16(localScriptPtr + position);
					position += 2;

					sprintf(lineBuffer, "var[%d]=%d\n", param1, param3);
				}

				break;
			}
		case 0xA:
		case 0xB:
		case 0xC:
		case 0xD:
			{
				byte param1;
				byte param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				if (param2) {
					byte param3;

					param3 = *(localScriptPtr + position);
					position++;

					if (opcode - 1 == 0xA) {
						sprintf(lineBuffer, "var[%d]+=var[%d]\n", param1, param3);
					} else if (opcode - 1 == 0xB) {
						sprintf(lineBuffer, "var[%d]-=var[%d]\n", param1, param3);
					} else if (opcode - 1 == 0xC) {
						sprintf(lineBuffer, "var[%d]*=var[%d]\n", param1, param3);
					} else if (opcode - 1 == 0xD) {
						sprintf(lineBuffer, "var[%d]/=var[%d]\n", param1, param3);
					}
				} else {
					int16 param3;

					param3 = READ_BE_UINT16(localScriptPtr +  position);
					position += 2;

					if (opcode - 1 == 0xA) {
						sprintf(lineBuffer, "var[%d]+=%d\n", param1, param3);
					} else if (opcode - 1 == 0xB) {
						sprintf(lineBuffer, "var[%d]-=%d\n", param1, param3);
					} else if (opcode - 1 == 0xC) {
						sprintf(lineBuffer, "var[%d]*=%d\n", param1, param3);
					} else if (opcode - 1 == 0xD) {
						sprintf(lineBuffer, "var[%d]/=%d\n", param1, param3);
					}
				}
				break;
			}
		case 0xE:
			{
				byte param1;
				byte param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				if (param2) {
					byte param3;

					param3 = *(localScriptPtr + position);
					position++;

					if (param2 == 1) {
						sprintf(compareString1, "var[%d]", param1);
						sprintf(compareString2, "var[%d]", param3);

					} else if (param2 == 2) {
						sprintf(compareString1, "var[%d]", param1);
						sprintf(compareString2, "globalVar[%d]", param3);
					} else {
						error("decompileScript: 0x0E: param2 = %d", param2);
					}
				} else {
					int16 param3;

					param3 = READ_BE_UINT16(localScriptPtr + position);
					position += 2;

					sprintf(compareString1, "var[%d]", param1);
					sprintf(compareString2, "%d", param3);
				}
				break;
			}
		case 0xF:
			{
				byte param1;
				byte param2;
				byte param3;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				param3 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "obj[%d]%s=var[%d]\n", param1, getObjPramName(param2), param3);

				break;
			}
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				if (opcode - 1 == 0x13) {
					sprintf(lineBuffer, "loadMask0(%d)\n", param);
				} else if (opcode - 1 == 0x14) {
					sprintf(lineBuffer, "unloadMask0(%d)\n", param);
				} else if (opcode - 1 == 0x15) {
					sprintf(lineBuffer, "OP_15(%d)\n", param);
				} else if (opcode - 1 == 0x16) {
					sprintf(lineBuffer, "loadMask1(%d)\n", param);
				} else if (opcode - 1 == 0x17) {
					sprintf(lineBuffer, "unloadMask0(%d)\n", param);
				} else if (opcode - 1 == 0x18) {
					sprintf(lineBuffer, "loadMask4(%d)\n", param);
				} else if (opcode - 1 == 0x19) {
					sprintf(lineBuffer, "unloadMask4(%d)\n", param);
				}
				break;
			}
		case 0x1A:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_1A(%d)\n", param);

				break;
			}
		case 0x1B:
			{
				sprintf(lineBuffer, "bgIncrustList.clear()\n");
				break;
			}
		case 0x1D:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "label(%d)\n", param);

				break;
			}
		case 0x1E:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "goto(%d)\n", param);

				break;
			}
		// If cases
		case 0x1F:
		case 0x20:
		case 0x21:
		case 0x22:
		case 0x23:
		case 0x24:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				if (opcode - 1 == 0x1F) {
					sprintf(lineBuffer, "if(%s>%s) goto(%d)\n", compareString1, compareString2, param);
				} else if (opcode - 1 == 0x20) {
					sprintf(lineBuffer, "if(%s>=%s) goto(%d)\n", compareString1, compareString2, param);
				} else if (opcode - 1 == 0x21) {
					sprintf(lineBuffer, "if(%s<%s) goto(%d)\n", compareString1, compareString2, param);
				} else if (opcode - 1 == 0x22) {
					sprintf(lineBuffer, "if(%s<=%s) goto(%d)\n", compareString1, compareString2, param);
				} else if (opcode - 1 == 0x23) {
					sprintf(lineBuffer, "if(%s==%s) goto(%d)\n", compareString1, compareString2, param);
				} else if (opcode - 1 == 0x24) {
					sprintf(lineBuffer, "if(%s!=%s) goto(%d)\n", compareString1, compareString2, param);
				}
				break;
			}
		case 0x25:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "removeLabel(%d)\n", param);

				break;
			}
		case 0x26:
			{
				byte param1;
				byte param2;

				param1 = *(localScriptPtr + position);
				position++;
				param2 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "loop(--var[%d]) -> label(%d)\n", param1, param2);

				break;
			}
		case 0x31:
		case 0x32:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				if (opcode - 1 == 0x31) {
					sprintf(lineBuffer, "startGlobalScript(%d)\n", param);
				} else if (opcode - 1 == 0x32) {
					sprintf(lineBuffer, "endGlobalScript(%d)\n", param);
				}
				break;
			}
		case 0x3B:
		case 0x3C:
		case 0x3D:
		case OP_loadPart:
			{
				if (opcode - 1 == 0x3B) {
					sprintf(lineBuffer, "loadResource(%s)\n", localScriptPtr + position);
				} else if (opcode - 1 == 0x3C) {
					sprintf(lineBuffer, "loadBg(%s)\n",	localScriptPtr + position);
				} else if (opcode - 1 == 0x3D) {
					sprintf(lineBuffer, "loadCt(%s)\n", localScriptPtr + position);
				} else if (opcode - 1 == OP_loadPart) {
					sprintf(lineBuffer, "loadPart(%s)\n", localScriptPtr + position);
				}

				position += strlen((char *)localScriptPtr + position) + 1;
				break;
			}
		case 0x40:
			{
				sprintf(lineBuffer, "closePart()\n");
				break;
			}
		case OP_loadNewPrcName:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "loadPrc(%d,%s)\n", param, localScriptPtr + position);

				position += strlen((char *)localScriptPtr + position) + 1;
				break;
			}
		case OP_requestCheckPendingDataLoad:	// nop
			{
				sprintf(lineBuffer, "requestCheckPendingDataLoad()\n");
				break;
			}
		case 0x45:
			{
				sprintf(lineBuffer, "blitAndFade()\n");
				break;
			}
		case 0x46:
			{
				sprintf(lineBuffer, "fadeToBlack()\n");
				break;
			}
		case 0x47:
			{
				byte param1;
				byte param2;
				int16 param3;
				int16 param4;
				int16 param5;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				param3 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param4 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param5 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				sprintf(lineBuffer, "transformPaletteRange(%d,%d,%d,%d,%d)\n", param1, param2, param3, param4, param5);

				break;
			}
		case 0x49:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "setDefaultMenuColor2(%d)\n", param);

				break;
			}
		case 0x4F:
			{
				sprintf(lineBuffer, "break()\n");
				exitScript = 1;
				break;
			}
		case 0x50:
			{
				sprintf(lineBuffer, "endScript()\n\n");
				break;
			}
		case 0x51:
			{
				byte param1;
				int16 param2;
				int16 param3;
				int16 param4;
				int16 param5;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param3 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param4 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param5 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				sprintf(lineBuffer, "message(%d,%d,%d,%d,%d)\n", param1, param2, param3, param4, param5);

				break;
			}
		case 0x52:
		case 0x53:
			{
				byte param1;
				byte param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				if (param2) {
					byte param3;

					param3 = *(localScriptPtr + position);
					position++;

					if (param2 == 1) {
						if (opcode - 1 == 0x52) {
							sprintf(lineBuffer, "globalVar[%d] = var[%d]\n", param1, param3);
						} else if (opcode - 1 == 0x53) {
							sprintf(compareString1, "globalVar[%d]", param1);
							sprintf(compareString2, "var[%d]", param3);
						}
					} else if (param2 == 2) {
						if (opcode - 1 == 0x52) {
							sprintf(lineBuffer, "globalVar[%d] = globalVar[%d]\n", param1, param3);
						} else if (opcode - 1 == 0x53) {
							sprintf(compareString1, "globalVar[%d]", param1);
							sprintf(compareString2, "globalVar[%d]", param3);
						}
					} else {
						if (opcode - 1 == 0x52) {
							error("decompileScript: 0x52: param2 = %d", param2);
						} else if (opcode - 1 == 0x53) {
							error("decompileScript: 0x53: param2 = %d", param2);
						}
					}
				} else {
					int16 param3;

					param3 = READ_BE_UINT16(localScriptPtr + position);
					position += 2;

					if (opcode - 1 == 0x52) {
						sprintf(lineBuffer, "globalVar[%d] = %d\n", param1, param3);
					} else if (opcode - 1 == 0x53) {
						sprintf(compareString1, "globalVar[%d]", param1);
						sprintf(compareString2, "%d", param3);
					}
				}
				break;
			}
		case 0x59:
			{
				sprintf(lineBuffer, "comment: %s\n", localScriptPtr + position);

				position += strlen((char *)localScriptPtr + position);
				break;
			}
		case 0x5A:
			{
				byte param1;
				byte param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "freePartRang(%d,%d)\n", param1, param2);

				break;
			}
		case 0x5B:
			{
				sprintf(lineBuffer, "unloadAllMasks()\n");
				break;
			}
		case 0x65:
			{
				sprintf(lineBuffer, "setupTableUnk1()\n");
				break;
			}
		case 0x66:
			{
				byte param1;
				int16 param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				sprintf(lineBuffer, "tableUnk1[%d] = %d\n", param1, param2);

				break;
			}
		case 0x68:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "setDefaultMenuBoxColor(%d)\n", param);

				break;
			}
		case 0x69:
			{
				sprintf(lineBuffer, "allowPlayerInput()\n");
				break;
			}
		case 0x6A:
			{
				sprintf(lineBuffer, "disallowPlayerInput()\n");
				break;
			}
		case 0x6B:
			{
				byte newDisk;

				newDisk = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "changeDataDisk(%d)\n", newDisk);

				break;
			}
		case 0x6D:
			{
				sprintf(lineBuffer, "loadDat(%s)\n", localScriptPtr + position);

				position += strlen((char *)localScriptPtr + position) + 1;
				break;
			}
		case 0x6E:	// nop
			{
				sprintf(lineBuffer, "updateDat()\n");
				break;
			}
		case 0x6F:
			{
				sprintf(lineBuffer, "OP_6F() -> dat related\n");
				break;
			}
		case 0x70:
			{
				sprintf(lineBuffer, "stopSample()\n");
				break;
			}
		case 0x79:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "disableSystemMenu(%d)\n", param);

				break;
			}
		case 0x77:
		case 0x78:
			{
				byte param1;
				byte param2;
				int16 param3;
				byte param4;
				int16 param5;
				int16 param6;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				param3 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param4 = *(localScriptPtr + position);
				position++;

				param5 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param6 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				if (opcode - 1 == 0x77) {
					sprintf(lineBuffer, "playSample(%d,%d,%d,%d,%d,%d)\n", param1, param2, param3, param4, param5, param6);
				} else if (opcode - 1 == 0x78) {
					sprintf(lineBuffer, "OP_78(%d,%d,%d,%d,%d,%d)\n", param1, param2, param3, param4, param5, param6);
				}

				break;
			}
		case 0x7A:
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_7A(%d)\n", param);

				break;
			}
		case 0x7B:	// OS only
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_7B(%d)\n", param);

				break;
			}
		case 0x7F:	// OS only
			{
				byte param1;
				byte param2;
				byte param3;
				byte param4;
				int16 param5;
				int16 param6;
				int16 param7;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				param3 = *(localScriptPtr + position);
				position++;

				param4 = *(localScriptPtr + position);
				position++;

				param5 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param6 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param7 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				sprintf(lineBuffer, "OP_7F(%d,%d,%d,%d,%d,%d,%d)\n", param1, param2, param3, param4, param5, param6, param7);

				break;
			}
		case 0x80:	// OS only
			{
				byte param1;
				byte param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_80(%d,%d)\n", param1, param2);

				break;
			}
		case 0x82:	// OS only
			{
				byte param1;
				byte param2;
				uint16 param3;
				uint16 param4;
				byte param5;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				param3 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param4 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param5 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_82(%d,%d,%d,%d,%d)\n", param1, param2, param3, param4, param5);

				break;
			}
		case 0x83:	// OS only
			{
				byte param1;
				byte param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_83(%d,%d)\n", param1, param2);

				break;
			}
		case 0x89:	// OS only
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "if(%s!=%s) goto next label(%d)\n", compareString1, compareString2, param);

				break;
			}
		case 0x8B:	// OS only
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_8B(%d)\n", param);

				break;
			}
		case 0x8C:	// OS only
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_8C(%d)\n", param);

				break;
			}
		case 0x8D:	// OS only
			{
				int16 param1;
				int16 param2;
				int16 param3;
				int16 param4;
				int16 param5;
				int16 param6;
				int16 param7;
				int16 param8;

				param1 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param2 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param3 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param4 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param5 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param6 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param7 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				param8 = READ_BE_UINT16(localScriptPtr + position);
				position += 2;

				sprintf(compareString1, "obj[%d]", param1);
				sprintf(compareString2, "{%d,%d,%d,%d,%d,%d,%d}", param2, param3, param4, param5, param6, param7, param8);

				break;
			}
		case 0x8E:	// OS only
			{
				byte param1;

				param1 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "ADDBG(%d,%s)\n", param1, localScriptPtr + position);

				position += strlen((char *)localScriptPtr + position);

				break;
			}
		case 0x8F:	// OS only
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_8F(%d)\n", param);

				break;
			}
		case 0x90:	// OS only
			{
				byte param1;

				param1 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "loadABS(%d,%s)\n", param1, localScriptPtr + position);

				position += strlen((char *)localScriptPtr + position);

				break;
			}
		case 0x91:	// OS only
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_91(%d)\n", param);

				break;
			}
		case 0x9D:	// OS only
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_9D(%d) -> flip img idx\n", param);

				break;
			}
		case 0x9E:	// OS only
			{
				byte param;

				param = *(localScriptPtr + position);
				position++;

				if (param) {
					byte param2;

					param2 = *(localScriptPtr + position);
					position++;

					sprintf(lineBuffer, "OP_9E(%d,%d)\n", param, param2);
				} else {
					int16 param2;

					param2 = READ_BE_UINT16(localScriptPtr + position);
					position += 2;

					sprintf(lineBuffer, "OP_9E(%d,%d)\n", param, param2);
				}

				break;
			}
		case 0xA0:	// OS only
			{
				byte param1;
				byte param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_A0(%d,%d)\n", param1, param2);

				break;
			}
		case 0xA1:	// OS only
			{
				byte param1;
				byte param2;

				param1 = *(localScriptPtr + position);
				position++;

				param2 = *(localScriptPtr + position);
				position++;

				sprintf(lineBuffer, "OP_A1(%d,%d)\n", param1, param2);

				break;
			}
		default:
			{
				sprintf(lineBuffer, "Unsupported opcode %X in decompileScript\n\n", opcode - 1);
				position = scriptSize;
				break;
			}
		}

		// printf(lineBuffer);
		strcpy(decompileBuffer[decompileBufferPosition++], lineBuffer);

		exitScript = 0;
		if (position >= scriptSize) {
			exitScript = 1;
		}

	} while (!exitScript);
}

void dumpScript(char *dumpName) {
    Common::File fHandle;
	uint16 i;

	fHandle.open(dumpName, Common::File::kFileWriteMode);

	for (i = 0; i < decompileBufferPosition; i++) {
		fHandle.writeString(Common::String(decompileBuffer[i]));
	}

	fHandle.close();

	decompileBufferPosition = 0;
}

#endif

} // End of namespace Cine
