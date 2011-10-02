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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "tsage/blue_force/blueforce_scenes2.h"
#include "tsage/scenes.h"
#include "tsage/tsage.h"

namespace TsAGE {

namespace BlueForce {

/*--------------------------------------------------------------------------
 * Scene 200 - Credits - Motorcycle Training
 *
 *--------------------------------------------------------------------------*/

void Scene200::Action1::signal() {
	Scene200 *scene = (Scene200 *)BF_GLOBALS._sceneManager._scene;
	SceneObject *owner = static_cast<SceneObject *>(this->_owner);
	static const uint32 black = 0;
	assert(owner);

	switch (_actionIndex++) {
	case 0:
		setDelay(1);
		break;
	case 1:
		BF_GLOBALS._scenePalette.loadPalette(235);
		BF_GLOBALS._scenePalette.refresh();
		_state = 0;
		setDelay(30);
		break;
	case 2:
		assert(_owner);
		owner->animate(ANIM_MODE_5, this);
		break;
	case 3:
		if (++_state < 2) {
			scene->_action2.signal();
			owner->setFrame(1);
			_actionIndex = 2;
		}
		setDelay(2);
		break;
	case 4: {
		PaletteRotation *rot;
		rot = BF_GLOBALS._scenePalette.addRotation(64, 79, 1);
		rot->setDelay(10);
		rot = BF_GLOBALS._scenePalette.addRotation(96, 111, 1);
		rot->setDelay(10);
		
		scene->setAction(&scene->_sequenceManager, this, 201, &scene->_object1, &scene->_object2,
			&scene->_object3, &scene->_object4, &scene->_object5, &scene->_object6, NULL);
		break;
	}
	case 5:
		BF_GLOBALS._sceneManager.changeScene(210);
		break;
	default:
		break;
	}
}

void Scene200::Action2::signal() {
	SceneObject *owner = static_cast<SceneObject *>(this->_owner);
	assert(owner);

	switch (_actionIndex++) {
	case 1:
		owner->setPosition(owner->_position);
		owner->animate(ANIM_MODE_5, this);
		break;
	case 2:
		owner->setPosition(owner->_position);
		owner->setFrame(1);
		break;
	default:
		break;
	}
}
	

/*--------------------------------------------------------------------------*/

void Scene200::postInit(SceneObjectList *OwnerList) {
	SceneExt::postInit();
	loadScene(200);
	setZoomPercents(0, 100, 200, 100);
	BF_GLOBALS._sound1.play(3);

	_object10.postInit();
	_object10.setVisage(200);
	_object10.setPosition(Common::Point(114, 102));
	_object10.setStrip(2);
	_object10.setFrame(1);
	_object10.changeZoom(100);

	_object1.postInit();
	_object1.hide();
	_object2.postInit();
	_object2.hide();
	_object3.postInit();
	_object3.hide();
	_object4.postInit();
	_object4.hide();
	_object5.postInit();
	_object5.hide();
	_object6.postInit();
	_object6.hide();

	_object11.postInit();
	_object11.setVisage(200);
	_object11.setPosition(Common::Point(96, 112), 1000);
	_object11.setStrip(3);
	_object11.setFrame(1); 
	_object11.changeZoom(100);

	_object10.setAction(&_action1);
	_object11.setAction(&_action2);
}

void Scene200::remove() {
	BF_GLOBALS._scenePalette.clearListeners();
	SceneExt::remove();
}

/*--------------------------------------------------------------------------
 * Scene 210 - Credits - Car Training
 *
 *--------------------------------------------------------------------------*/

void Scene210::Action1::signal() {
	Scene210 *scene = (Scene210 *)BF_GLOBALS._sceneManager._scene;
	SceneObject *owner = static_cast<SceneObject *>(_owner);
	assert(owner);

	switch (_actionIndex++) {
	case 0:
		setDelay(1);
		break;
	case 1:
		owner->animate(ANIM_MODE_5, this);
		break;
	case 2: {
		PaletteRotation *rot;
		rot = BF_GLOBALS._scenePalette.addRotation(64, 79, 1);
		rot->setDelay(10);
		rot = BF_GLOBALS._scenePalette.addRotation(96, 111, 1);
		rot->setDelay(10);
		
		scene->setAction(&scene->_sequenceManager, this, 210, &scene->_object10, &scene->_object11,
			&scene->_object12, &scene->_object13, &scene->_object14, &scene->_object15, NULL);
		break;
	}
	case 3:
		BF_GLOBALS._sceneManager.changeScene(220);
		break;
	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/

void Scene210::postInit(SceneObjectList *OwnerList) {
	SceneExt::postInit();
	loadScene(210);
	BF_GLOBALS._scenePalette.loadPalette(235);
	BF_GLOBALS._scenePalette.refresh();

	_object9.postInit();
	_object9.setVisage(210);
	_object9.setPosition(Common::Point(146, 151));
	_object9.setStrip(1);
	_object9.setFrame(1);
	_object9.changeZoom(100);
	_object9.setAction(&_action1);

	_object10.postInit();
	_object10.hide();
	_object11.postInit();
	_object11.hide();
	_object12.postInit();
	_object12.hide();
	_object13.postInit();
	_object13.hide();
	_object14.postInit();
	_object14.hide();
	_object15.postInit();
	_object15.hide();
}

void Scene210::remove() {
	BF_GLOBALS._scenePalette.clearListeners();
	SceneExt::remove();
}

/*--------------------------------------------------------------------------
 * Scene 220 - Credits - Martial Arts
 *
 *--------------------------------------------------------------------------*/

void Scene220::Action1::signal() {
	Scene220 *scene = (Scene220 *)BF_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		setDelay(1);
		break;
	case 1:
		BF_GLOBALS._scenePalette.loadPalette(235);
		BF_GLOBALS._scenePalette.refresh();
		setDelay(30);
		break;
	case 2:
		BF_GLOBALS._scenePalette.clearListeners();
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	case 3:
		scene->_object2.setVisage(221);
		scene->_object2.setFrame(1);
		scene->_object2.setPosition(Common::Point(178, 122));
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	case 4:
		BF_GLOBALS._scenePalette.loadPalette(2);
		BF_GLOBALS._scenePalette.refresh();
		BF_GLOBALS._scenePalette.loadPalette(235);
		BF_GLOBALS._scenePalette.refresh();
		setDelay(5);
		break;
	case 5:
		scene->_object1.remove();

		scene->_object2.setVisage(222);
		scene->_object2.setFrame(1);
		scene->_object2.setPosition(Common::Point(164, 138));
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	case 6:
		scene->_object2.setVisage(223);
		scene->_object2.setFrame(1);
		scene->_object2.setPosition(Common::Point(164, 139));
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	case 7:
		scene->_object2.setVisage(230);
		scene->_object2.setFrame(1);
		scene->_object2.setPosition(Common::Point(168, 136));
		scene->_object2.animate(ANIM_MODE_4, 13, 1, this);
		break;
	case 8:
		scene->_object2.animate(ANIM_MODE_5, this);

		scene->_object3.postInit();
		scene->_object3.setVisage(231);
		scene->_object3.setPosition(Common::Point(65, 179));
		scene->_object3.setStrip(1);
		scene->_object3.setFrame(1);
		scene->_object3.changeZoom(100);
		scene->_object3.setAction(&scene->_action2, this);
		break;
	case 9:
		break;
	case 10:
		scene->_object2.setVisage(224);
		scene->_object2.setFrame(1);
		scene->_object2.setPosition(Common::Point(148, 143));
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	case 11: {
		PaletteRotation *rot;
		rot = BF_GLOBALS._scenePalette.addRotation(64, 79, 1);
		rot->setDelay(10);
		rot = BF_GLOBALS._scenePalette.addRotation(96, 111, 1);
		rot->setDelay(10);
		
		scene->setAction(&scene->_sequenceManager, this, 220, &scene->_object4, &scene->_object5,
			&scene->_object6, &scene->_object7, &scene->_object8, &scene->_object9, NULL);
		break;
	}
	case 12:
		scene->_object2.setVisage(232);
		scene->_object2.setFrame(1);
		scene->_object2.setPosition(Common::Point(146, 143));
		scene->_object2._numFrames = 5;
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	case 13:
		scene->_object2.setVisage(228);
		scene->_object2.setFrame(1);
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	case 14:
		scene->_object2.setVisage(229);
		scene->_object2.setFrame(1);
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	case 15:
		BF_GLOBALS._sceneManager.changeScene(225);
		break;
	default:
		break;
	}
}

void Scene220::Action2::signal() {
	Scene220 *scene = (Scene220 *)BF_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		scene->_object3.setPosition(scene->_object3._position);
		scene->_object3.animate(ANIM_MODE_5, this);
		break;
	case 1:
		setDelay(50);
		break;
	case 2:
		scene->_object3.remove();
		remove();
		break;
	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/

void Scene220::postInit(SceneObjectList *OwnerList) {
	SceneExt::postInit();
	loadScene(220);

	_object2.postInit();
	_object2.setVisage(220);
	_object2.setPosition(Common::Point(182, 122));
	_object2.setStrip(1);
	_object2.setFrame(1);
	_object2.changeZoom(100);

	_object1.postInit();
	_object1.setVisage(220);
	_object1.setPosition(Common::Point(164, 138));
	_object1.setStrip(2);
	_object1.setFrame(1);
	_object1.changeZoom(100);

	_object4.postInit();
	_object4.hide();
	_object5.postInit();
	_object5.hide();
	_object6.postInit();
	_object6.hide();
	_object7.postInit();
	_object7.hide();
	_object8.postInit();
	_object8.hide();
	_object9.postInit();
	_object9.hide();

	_object2.setAction(&_action1);
}

void Scene220::remove() {
	BF_GLOBALS._scenePalette.clearListeners();
	SceneExt::remove();
}

} // End of namespace BlueForce

} // End of namespace TsAGE