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
 * $URL$
 * $Id$
 *
 */

#if defined(__ANDROID__)

#include "base/main.h"
#include "common/config-manager.h"
#include "engines/engine.h"

#include "backends/platform/android/android.h"
#include "backends/platform/android/asset-archive.h"
#include "backends/platform/android/jni.h"

__attribute__ ((visibility("default")))
jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
	return JNI::onLoad(vm);
}

JavaVM *JNI::_vm = 0;
jobject JNI::_jobj = 0;
jobject JNI::_jobj_audio_track = 0;
jobject JNI::_jobj_egl = 0;
jobject JNI::_jobj_egl_display = 0;
jobject JNI::_jobj_egl_surface = 0;

Common::Archive *JNI::_asset_archive = 0;
OSystem_Android *JNI::_system = 0;

int JNI::surface_changeid = 0;
int JNI::egl_surface_width = 0;
int JNI::egl_surface_height = 0;
bool JNI::_ready_for_events = 0;

jfieldID JNI::_FID_Event_type = 0;
jfieldID JNI::_FID_Event_synthetic = 0;
jfieldID JNI::_FID_Event_kbd_keycode = 0;
jfieldID JNI::_FID_Event_kbd_ascii = 0;
jfieldID JNI::_FID_Event_kbd_flags = 0;
jfieldID JNI::_FID_Event_mouse_x = 0;
jfieldID JNI::_FID_Event_mouse_y = 0;
jfieldID JNI::_FID_Event_mouse_relative = 0;

jmethodID JNI::_MID_displayMessageOnOSD = 0;
jmethodID JNI::_MID_setWindowCaption = 0;
jmethodID JNI::_MID_showVirtualKeyboard = 0;
jmethodID JNI::_MID_getSysArchives = 0;
jmethodID JNI::_MID_getPluginDirectories = 0;
jmethodID JNI::_MID_initSurface = 0;
jmethodID JNI::_MID_deinitSurface = 0;

jmethodID JNI::_MID_EGL10_eglSwapBuffers = 0;

jmethodID JNI::_MID_AudioTrack_flush = 0;
jmethodID JNI::_MID_AudioTrack_pause = 0;
jmethodID JNI::_MID_AudioTrack_play = 0;
jmethodID JNI::_MID_AudioTrack_stop = 0;
jmethodID JNI::_MID_AudioTrack_write = 0;

const JNINativeMethod JNI::_natives[] = {
	{ "create", "(Landroid/content/res/AssetManager;"
				"Ljavax/microedition/khronos/egl/EGL10;"
				"Ljavax/microedition/khronos/egl/EGLDisplay;"
				"Landroid/media/AudioTrack;II)V",
		(void *)JNI::create },
	{ "destroy", "()V",
		(void *)JNI::destroy },
	{ "setSurface", "(II)V",
	 	(void *)JNI::setSurface },
	{ "main", "([Ljava/lang/String;)I",
	 	(void *)JNI::main },
	{ "pushEvent", "(Lorg/inodes/gus/scummvm/Event;)V",
		(void *)JNI::pushEvent },
	{ "enableZoning", "(Z)V",
		(void *)JNI::enableZoning },
	{ "pauseEngine", "(Z)V",
		(void *)JNI::pauseEngine }
};

JNI::JNI() {
}

JNI::~JNI() {
}

jint JNI::onLoad(JavaVM *vm) {
	_vm = vm;

	JNIEnv *env;

	if (_vm->GetEnv((void **)&env, JNI_VERSION_1_2))
		return JNI_ERR;

	jclass cls = env->FindClass("org/inodes/gus/scummvm/ScummVM");
	if (cls == 0)
		return JNI_ERR;

	if (env->RegisterNatives(cls, _natives, ARRAYSIZE(_natives)) < 0)
		return JNI_ERR;

	jclass event = env->FindClass("org/inodes/gus/scummvm/Event");
	if (event == 0)
		return JNI_ERR;

	_FID_Event_type = env->GetFieldID(event, "type", "I");
	if (_FID_Event_type == 0)
		return JNI_ERR;

	_FID_Event_synthetic = env->GetFieldID(event, "synthetic", "Z");
	if (_FID_Event_synthetic == 0)
		return JNI_ERR;

	_FID_Event_kbd_keycode = env->GetFieldID(event, "kbd_keycode", "I");
	if (_FID_Event_kbd_keycode == 0)
		return JNI_ERR;

	_FID_Event_kbd_ascii = env->GetFieldID(event, "kbd_ascii", "I");
	if (_FID_Event_kbd_ascii == 0)
		return JNI_ERR;

	_FID_Event_kbd_flags = env->GetFieldID(event, "kbd_flags", "I");
	if (_FID_Event_kbd_flags == 0)
		return JNI_ERR;

	_FID_Event_mouse_x = env->GetFieldID(event, "mouse_x", "I");
	if (_FID_Event_mouse_x == 0)
		return JNI_ERR;

	_FID_Event_mouse_y = env->GetFieldID(event, "mouse_y", "I");
	if (_FID_Event_mouse_y == 0)
		return JNI_ERR;

	_FID_Event_mouse_relative = env->GetFieldID(event, "mouse_relative", "Z");
	if (_FID_Event_mouse_relative == 0)
		return JNI_ERR;

	return JNI_VERSION_1_2;
}

JNIEnv *JNI::getEnv() {
	JNIEnv *env = 0;

	jint res = _vm->GetEnv((void **)&env, JNI_VERSION_1_2);

	if (res != JNI_OK) {
		LOGE("GetEnv() failed: %d", res);
		abort();
	}

	return env;
}

void JNI::attachThread() {
	JNIEnv *env = 0;

	jint res = _vm->AttachCurrentThread(&env, 0);

	if (res != JNI_OK) {
		LOGE("AttachCurrentThread() failed: %d", res);
		abort();
	}
}

void JNI::detachThread() {
	jint res = _vm->DetachCurrentThread();

	if (res != JNI_OK) {
		LOGE("DetachCurrentThread() failed: %d", res);
		abort();
	}
}

void JNI::setReadyForEvents(bool ready) {
	_ready_for_events = ready;
}

void JNI::throwByName(JNIEnv *env, const char *name, const char *msg) {
	jclass cls = env->FindClass(name);

	// if cls is 0, an exception has already been thrown
	if (cls != 0)
		env->ThrowNew(cls, msg);

	env->DeleteLocalRef(cls);
}

void JNI::throwRuntimeException(JNIEnv *env, const char *msg) {
	throwByName(env, "java/lang/RuntimeException", msg);
}

// calls to the dark side

void JNI::displayMessageOnOSD(const char *msg) {
	JNIEnv *env = JNI::getEnv();
	jstring java_msg = env->NewStringUTF(msg);

	env->CallVoidMethod(_jobj, _MID_displayMessageOnOSD, java_msg);

	if (env->ExceptionCheck()) {
		LOGE("Failed to display OSD message");

		env->ExceptionDescribe();
		env->ExceptionClear();
	}

	env->DeleteLocalRef(java_msg);
}

void JNI::setWindowCaption(const char *caption) {
	JNIEnv *env = JNI::getEnv();
	jstring java_caption = env->NewStringUTF(caption);

	env->CallVoidMethod(_jobj, _MID_setWindowCaption, java_caption);

	if (env->ExceptionCheck()) {
		LOGE("Failed to set window caption");

		env->ExceptionDescribe();
		env->ExceptionClear();
	}

	env->DeleteLocalRef(java_caption);
}

void JNI::showVirtualKeyboard(bool enable) {
	JNIEnv *env = JNI::getEnv();

	env->CallVoidMethod(_jobj, _MID_showVirtualKeyboard, enable);

	if (env->ExceptionCheck()) {
		LOGE("Error trying to show virtual keyboard");

		env->ExceptionDescribe();
		env->ExceptionClear();
	}
}

void JNI::addSysArchivesToSearchSet(Common::SearchSet &s, int priority) {
	JNIEnv *env = JNI::getEnv();

	s.add("ASSET", _asset_archive, priority, false);

	jobjectArray array =
		(jobjectArray)env->CallObjectMethod(_jobj, _MID_getSysArchives);

	if (env->ExceptionCheck()) {
		LOGE("Error finding system archive path");

		env->ExceptionDescribe();
		env->ExceptionClear();

		return;
	}

	jsize size = env->GetArrayLength(array);
	for (jsize i = 0; i < size; ++i) {
		jstring path_obj = (jstring)env->GetObjectArrayElement(array, i);
		const char *path = env->GetStringUTFChars(path_obj, 0);

		if (path != 0) {
			s.addDirectory(path, path, priority);
			env->ReleaseStringUTFChars(path_obj, path);
		}

		env->DeleteLocalRef(path_obj);
	}
}

void JNI::getPluginDirectories(Common::FSList &dirs) {
	JNIEnv *env = JNI::getEnv();

	jobjectArray array =
		(jobjectArray)env->CallObjectMethod(_jobj, _MID_getPluginDirectories);

	if (env->ExceptionCheck()) {
		LOGE("Error finding plugin directories");

		env->ExceptionDescribe();
		env->ExceptionClear();

		return;
	}

	jsize size = env->GetArrayLength(array);
	for (jsize i = 0; i < size; ++i) {
		jstring path_obj = (jstring)env->GetObjectArrayElement(array, i);

		if (path_obj == 0)
			continue;

		const char *path = env->GetStringUTFChars(path_obj, 0);

		if (path == 0) {
			LOGE("Error getting string characters from plugin directory");

			env->ExceptionClear();
			env->DeleteLocalRef(path_obj);

			continue;
		}

		dirs.push_back(Common::FSNode(path));

		env->ReleaseStringUTFChars(path_obj, path);
		env->DeleteLocalRef(path_obj);
	}
}

bool JNI::initSurface() {
	JNIEnv *env = JNI::getEnv();

	jobject obj = env->CallObjectMethod(_jobj, _MID_initSurface);

	if (!obj || env->ExceptionCheck()) {
		LOGE("initSurface failed");

		env->ExceptionDescribe();
		env->ExceptionClear();

		return false;
	}

	_jobj_egl_surface = env->NewGlobalRef(obj);

	return true;
}

void JNI::deinitSurface() {
	JNIEnv *env = JNI::getEnv();

	env->CallVoidMethod(_jobj, _MID_deinitSurface);

	if (env->ExceptionCheck()) {
		LOGE("deinitSurface failed");

		env->ExceptionDescribe();
		env->ExceptionClear();
	}

	env->DeleteGlobalRef(_jobj_egl_surface);
	_jobj_egl_surface = 0;
}

void JNI::setAudioPause() {
	JNIEnv *env = JNI::getEnv();

	env->CallVoidMethod(_jobj_audio_track, _MID_AudioTrack_flush);

	if (env->ExceptionCheck()) {
		LOGE("Error flushing AudioTrack");

		env->ExceptionDescribe();
		env->ExceptionClear();
	}

	env->CallVoidMethod(_jobj_audio_track, _MID_AudioTrack_pause);

	if (env->ExceptionCheck()) {
		LOGE("Error setting AudioTrack: pause");

		env->ExceptionDescribe();
		env->ExceptionClear();
	}
}

void JNI::setAudioPlay() {
	JNIEnv *env = JNI::getEnv();

	env->CallVoidMethod(_jobj_audio_track, _MID_AudioTrack_play);

	if (env->ExceptionCheck()) {
		LOGE("Error setting AudioTrack: play");

		env->ExceptionDescribe();
		env->ExceptionClear();
	}
}

void JNI::setAudioStop() {
	JNIEnv *env = JNI::getEnv();

	env->CallVoidMethod(_jobj_audio_track, _MID_AudioTrack_stop);

	if (env->ExceptionCheck()) {
		LOGE("Error setting AudioTrack: stop");

		env->ExceptionDescribe();
		env->ExceptionClear();
	}
}

// natives for the dark side

void JNI::create(JNIEnv *env, jobject self, jobject asset_manager,
				jobject egl, jobject egl_display,
				jobject at, jint audio_sample_rate, jint audio_buffer_size) {
	assert(!_system);

	_asset_archive = new AndroidAssetArchive(asset_manager);
	assert(_asset_archive);

	_system = new OSystem_Android(audio_sample_rate, audio_buffer_size);
	assert(_system);

	// weak global ref to allow class to be unloaded
	// ... except dalvik implements NewWeakGlobalRef only on froyo
	//_jobj = env->NewWeakGlobalRef(self);

	_jobj = env->NewGlobalRef(self);

	jclass cls = env->GetObjectClass(_jobj);

#define FIND_METHOD(prefix, name, signature) do {							\
		_MID_ ## prefix ## name = env->GetMethodID(cls, #name, signature);	\
		if (_MID_ ## prefix ## name == 0)									\
			return;															\
	} while (0)

	FIND_METHOD(, setWindowCaption, "(Ljava/lang/String;)V");
	FIND_METHOD(, displayMessageOnOSD, "(Ljava/lang/String;)V");
	FIND_METHOD(, showVirtualKeyboard, "(Z)V");
	FIND_METHOD(, getSysArchives, "()[Ljava/lang/String;");
	FIND_METHOD(, getPluginDirectories, "()[Ljava/lang/String;");
	FIND_METHOD(, initSurface, "()Ljavax/microedition/khronos/egl/EGLSurface;");
	FIND_METHOD(, deinitSurface, "()V");

	_jobj_egl = env->NewGlobalRef(egl);
	_jobj_egl_display = env->NewGlobalRef(egl_display);

	cls = env->GetObjectClass(_jobj_egl);

	FIND_METHOD(EGL10_, eglSwapBuffers,
				"(Ljavax/microedition/khronos/egl/EGLDisplay;"
				"Ljavax/microedition/khronos/egl/EGLSurface;)Z");

	_jobj_audio_track = env->NewGlobalRef(at);

	cls = env->GetObjectClass(_jobj_audio_track);

	FIND_METHOD(AudioTrack_, flush, "()V");
	FIND_METHOD(AudioTrack_, pause, "()V");
	FIND_METHOD(AudioTrack_, play, "()V");
	FIND_METHOD(AudioTrack_, stop, "()V");
	FIND_METHOD(AudioTrack_, write, "([BII)I");

#undef FIND_METHOD

	g_system = _system;
}

void JNI::destroy(JNIEnv *env, jobject self) {
	delete _asset_archive;
	_asset_archive = 0;

	delete _system;
	g_system = 0;
	_system = 0;

	// see above
	//JNI::getEnv()->DeleteWeakGlobalRef(_jobj);

	JNI::getEnv()->DeleteGlobalRef(_jobj_egl_display);
	JNI::getEnv()->DeleteGlobalRef(_jobj_egl);
	JNI::getEnv()->DeleteGlobalRef(_jobj_audio_track);
	JNI::getEnv()->DeleteGlobalRef(_jobj);
}

void JNI::setSurface(JNIEnv *env, jobject self, jint width, jint height) {
	egl_surface_width = width;
	egl_surface_height = height;
	surface_changeid++;
}

jint JNI::main(JNIEnv *env, jobject self, jobjectArray args) {
	assert(_system);

	const int MAX_NARGS = 32;
	int res = -1;

	int argc = env->GetArrayLength(args);
	if (argc > MAX_NARGS) {
		throwByName(env, "java/lang/IllegalArgumentException",
					"too many arguments");
		return 0;
	}

	char *argv[MAX_NARGS];

	// note use in cleanup loop below
	int nargs;

	for (nargs = 0; nargs < argc; ++nargs) {
		jstring arg = (jstring)env->GetObjectArrayElement(args, nargs);

		if (arg == 0) {
			argv[nargs] = 0;
		} else {
			const char *cstr = env->GetStringUTFChars(arg, 0);

			argv[nargs] = const_cast<char *>(cstr);

			// exception already thrown?
			if (cstr == 0)
				goto cleanup;
		}

		env->DeleteLocalRef(arg);
	}

#ifdef DYNAMIC_MODULES
	PluginManager::instance().addPluginProvider(new AndroidPluginProvider());
#endif

	LOGI("Entering scummvm_main with %d args", argc);

	res = scummvm_main(argc, argv);

	LOGI("scummvm_main exited with code %d", res);

	_system->quit();

cleanup:
	nargs--;

	for (int i = 0; i < nargs; ++i) {
		if (argv[i] == 0)
			continue;

		jstring arg = (jstring)env->GetObjectArrayElement(args, nargs);

		// Exception already thrown?
		if (arg == 0)
			return res;

		env->ReleaseStringUTFChars(arg, argv[i]);
		env->DeleteLocalRef(arg);
	}

	return res;
}

void JNI::pushEvent(JNIEnv *env, jobject self, jobject java_event) {
	// drop events until we're ready and after we quit
	if (!_ready_for_events)
		return;

	assert(_system);

	Common::Event event;
	event.type = (Common::EventType)env->GetIntField(java_event,
														_FID_Event_type);

	event.synthetic =
		env->GetBooleanField(java_event, _FID_Event_synthetic);

	switch (event.type) {
	case Common::EVENT_KEYDOWN:
	case Common::EVENT_KEYUP:
		event.kbd.keycode = (Common::KeyCode)env->GetIntField(
			java_event, _FID_Event_kbd_keycode);
		event.kbd.ascii = static_cast<int>(env->GetIntField(
			java_event, _FID_Event_kbd_ascii));
		event.kbd.flags = static_cast<int>(env->GetIntField(
			java_event, _FID_Event_kbd_flags));
		break;
	case Common::EVENT_MOUSEMOVE:
	case Common::EVENT_LBUTTONDOWN:
	case Common::EVENT_LBUTTONUP:
	case Common::EVENT_RBUTTONDOWN:
	case Common::EVENT_RBUTTONUP:
	case Common::EVENT_WHEELUP:
	case Common::EVENT_WHEELDOWN:
	case Common::EVENT_MBUTTONDOWN:
	case Common::EVENT_MBUTTONUP:
		event.mouse.x =
			env->GetIntField(java_event, _FID_Event_mouse_x);
		event.mouse.y =
			env->GetIntField(java_event, _FID_Event_mouse_y);
		// This is a terrible hack.	 We stash "relativeness"
		// in the kbd.flags field until pollEvent() can work
		// it out.
		event.kbd.flags = env->GetBooleanField(
			java_event, _FID_Event_mouse_relative) ? 1 : 0;
		break;
	default:
		break;
	}

	_system->pushEvent(event);
}

void JNI::enableZoning(JNIEnv *env, jobject self, jboolean enable) {
	assert(_system);

	_system->enableZoning(enable);
}

void JNI::pauseEngine(JNIEnv *env, jobject self, jboolean pause) {
	if (!_system || !g_engine)
		return;

	if ((pause && !g_engine->isPaused()) || (!pause && g_engine->isPaused())) {
		LOGD("pauseEngine: %d", pause);
		g_engine->pauseEngine(pause);
	}
}

#endif
