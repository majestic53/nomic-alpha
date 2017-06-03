/**
 * Nomic
 * Copyright (C) 2017 David Jolly
 *
 * Nomic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nomic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NOMIC_DEFINE_H_
#define NOMIC_DEFINE_H_

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace nomic {

#ifndef __in
	#define __in
#endif // __in
#ifndef __in_opt
	#define __in_opt
#endif // __in_opt
#ifndef __inout
	#define __inout
#endif // __inout
#ifndef __inout_opt
	#define __inout_opt
#endif // __inout_opt

	#define CAMERA_CLIP_MAX 1000.f
	#define CAMERA_CLIP_MIN 0.1f
	#define CAMERA_FOV_DEFAULT 45.f
	#define CAMERA_FOV_MAX 90.f
	#define CAMERA_FOV_MIN 10.f
	#define CAMERA_PITCH_MAX 89.f
	#define CAMERA_PITCH_MIN -CAMERA_PITCH_MAX
	#define CAMERA_SENSITIVITY 0.55f
	#define CAMERA_SPEED 0.35f
	#define CAMERA_STRAFE 0.35f
	#define CAMERA_YAW_MAX 360.f
	#define CAMERA_YAW_MIN 0.f

	#define CHANNEL_MAX UINT8_MAX

	#define DISPLAY_DEFAULT_ALPHA (255 / (float) CHANNEL_MAX)
	#define DISPLAY_DEFAULT_BLUE (122 / (float) CHANNEL_MAX)
	#define DISPLAY_DEFAULT_FULLSCREEN false
	#define DISPLAY_DEFAULT_GREEN (78 / (float) CHANNEL_MAX)
	#define DISPLAY_DEFAULT_HEIGHT 768
	#define DISPLAY_DEFAULT_RED (49 / (float) CHANNEL_MAX)
	#define DISPLAY_DEFAULT_TITLE NOMIC " " STRING_CONCAT(NOMIC_VERSION_MAJOR) "." STRING_CONCAT(NOMIC_VERSION_MINOR) \
		"." STRING_CONCAT(NOMIC_VERSION_WEEK) "." STRING_CONCAT(NOMIC_VERSION_REVISION)
	#define DISPLAY_DEFAULT_VSYNC false
	#define DISPLAY_DEFAULT_WIDTH 1024
	#define DISPLAY_FLAGS_CLEAR (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
	#define DISPLAY_FLAGS_DEFAULT (SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_OPENGL)
	#define DISPLAY_MOUSE_RELATIVE SDL_TRUE

	#define EXCEPTION_UNKNOWN "Unknown exception"

	#define FORMAT_STRING(_FORMAT_, ...) nomic::utility::format_as_string(_FORMAT_, __VA_ARGS__)

	#define GL_ATTRIBUTE_ACCELERATE_VISUAL 1
	#define GL_ATTRIBUTE_COLOR_SIZE 8
	#define GL_ATTRIBUTE_DEPTH_SIZE 24
	#define GL_ATTRIBUTE_DOUBLEBUFFER 1
	#define GL_ATTRIBUTE_MAJOR_VERSION 3
	#define GL_ATTRIBUTE_MINOR_VERSION 3

#ifndef NDEBUG
	#define _GL_CHECK_ERROR(_LEVEL_, _COMMAND_) { \
		GLenum err = glGetError(); \
		while(err != GL_NO_ERROR) { \
			TRACE_MESSAGE_FORMAT(_LEVEL_, "%s failed! Error=%x(%s)", _COMMAND_, err, GL_ERROR_AS_STRING(err)); \
			err = glGetError(); \
		} \
		}
	#define GL_CHECK(_LEVEL_, _COMMAND_, ...) _COMMAND_(__VA_ARGS__); _GL_CHECK_ERROR(_LEVEL_, _COMMAND_)
	#define GL_CHECK_RESULT(_LEVEL_, _COMMAND_, _RESULT_, ...) (_RESULT_) = _COMMAND_(__VA_ARGS__); _GL_CHECK_ERROR(_LEVEL_, _COMMAND_)
#else
	#define GL_CHECK(_LEVEL_, _COMMAND_, ...) _COMMAND_(__VA_ARGS__)
	#define GL_CHECK_RESULT(_LEVEL_, _COMMAND_, _RESULT_, ...) (_RESULT_) = _COMMAND_(__VA_ARGS__)
#endif // NDEBUG
	#define GL_ERROR_AS_STRING(_ERROR_) gluErrorString(_ERROR_)
	#define GL_FLUSH_ERROR() while(glGetError() != GL_NO_ERROR)

	#define HANDLE_COUNT 1
	#define HANDLE_INVALID 0

	#define MILLISECONDS_PER_SECOND 1000

	#define NOMIC "Nomic"
	#define NOMIC_COPYRIGHT "Copyright (C) 2017 David Jolly"
	#define NOMIC_VERSION_MAJOR 0
	#define NOMIC_VERSION_MINOR 1
	#define NOMIC_VERSION_RELEASE "pre-alpha"
	#define NOMIC_VERSION_REVISION 4
	#define NOMIC_VERSION_WEEK 1722

	#define OBJECT_COUNT 1

	#define QUEUE_MAX 0x1000

	#define REFERENCE_INIT 1

	#define RUNTIME_PAUSE_DELAY 200
	#define RUNTIME_TICKS_PER_SECOND 25
	#define RUNTIME_TICK_SKIP 5

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex << (uintptr_t) ((_TYPE_) (_VAL_)) \
			<< std::dec

	#define SCALAR_INVALID(_TYPE_) ((_TYPE_) -1)

	#define SDL_FLAGS_INIT (SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO)

	#define SEND_EVENT(_EVENT_) { \
		nomic::event::manager &instance = nomic::event::manager::acquire(); \
		if(instance.initialized()) { \
			instance.receive_event(_EVENT_); \
		} \
		instance.release(); \
		}

	#define STRING_EMPTY "<EMPTY>"
	#define STRING_INVALID "<INVALID>"
	#define STRING_UNKNOWN "<UNKNOWN>"

	#define STRING_CHECK(_STR_) (_STR_.empty() ? STRING_EMPTY : _STR_.c_str())

	#define _STRING_CONCAT(_STRING_) # _STRING_
	#define STRING_CONCAT(_STRING_) _STRING_CONCAT(_STRING_)

	#define SUBTYPE_UNDEFINED SCALAR_INVALID(uint32_t)

	#define THREAD_TIMEOUT 5000

	#define THROW_EXCEPTION(_EXCEPT_) THROW_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		nomic::exception::generate(_EXCEPT_, __FILE__, __FUNCTION__, __LINE__, FORMAT_STRING(_FORMAT_, __VA_ARGS__))

	#define TIMEOUT_UNDEFINED SCALAR_INVALID(uint32_t)

#ifndef NDEBUG
	#define _TRACE(_LEVEL_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...) { \
		if((_LEVEL_) <= TRACE) { \
			nomic::trace &instance = nomic::trace::acquire(); \
			if(instance.initialized()) { \
				instance.generate(_LEVEL_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, FORMAT_STRING(_FORMAT_, __VA_ARGS__)); \
			} \
			instance.release(); \
		} \
		}
#else
	#define _TRACE(_LEVEL_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...)
#endif // NDEBUG
	#define TRACE_ENTRY(_LEVEL_) \
		_TRACE(_LEVEL_, "", __FILE__, __FUNCTION__, __LINE__, TRACE_PREFIX_ENTRY "%s", __FUNCTION__, "")
	#define TRACE_ENTRY_FORMAT(_LEVEL_, _FORMAT_, ...) \
		_TRACE(_LEVEL_, "", __FILE__, __FUNCTION__, __LINE__, TRACE_PREFIX_ENTRY "%s: " _FORMAT_, __FUNCTION__, __VA_ARGS__)
	#define TRACE_EXCEPTION(_MESSAGE_, _FILE_, _FUNCTION_, _LINE_) \
		_TRACE(LEVEL_ERROR, _MESSAGE_, __FILE__, __FUNCTION__, __LINE__, "", "")
	#define TRACE_EXIT(_LEVEL_) \
		_TRACE(_LEVEL_, "", __FILE__, __FUNCTION__, __LINE__, TRACE_PREFIX_EXIT "%s", __FUNCTION__, "")
	#define TRACE_EXIT_FORMAT(_LEVEL_, _FORMAT_, ...) \
		_TRACE(_LEVEL_, "", __FILE__, __FUNCTION__, __LINE__, TRACE_PREFIX_ENTRY "%s: " _FORMAT_, __FUNCTION__, __VA_ARGS__)
	#define TRACE_MESSAGE(_LEVEL_, _MESSAGE_) TRACE_MESSAGE_FORMAT(_LEVEL_, _MESSAGE_, "")
	#define TRACE_MESSAGE_FORMAT(_LEVEL_, _FORMAT_, ...) \
		_TRACE(_LEVEL_, "", __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)
	#define TRACE_PREFIX_ENTRY "+"
	#define TRACE_PREFIX_EXIT "-"

	#define TRANSFORM_POSITION_DEFAULT glm::vec3(0.f, 0.f, 0.f)
	#define TRANSFORM_ROTATION_DEFAULT glm::vec3(0.f, 0.f, -1.f)
	#define TRANSFORM_UP_DEFAULT glm::vec3(0.f, 1.f, 0.f)

	#define UID_INVALID 0

	#define UNIFORM_MATRIX_DEFAULT glm::mat4(1.f)

	enum {
		ENTITY_CAMERA = 0,
	};

	enum {
		EVENT_UNDEFINED = 0,
		EVENT_INPUT,
	};

	enum {
		KEY_BACKWARD = 22,
		KEY_FORWARD = 26,
		KEY_LEFT = 4,
		KEY_RIGHT = 7,
	};

	enum {
		INPUT_BUTTON = 0,
		INPUT_KEY,
		INPUT_MOTION,
		INPUT_WHEEL,
	};

	enum {
		PRIMITIVE_PROGRAM = 0,
		PRIMITIVE_SHADER,
		PRIMITIVE_VAO,
		PRIMITIVE_VBO,
	};

	class utility {

		public:

			static std::string format_as_string(
				__in const char *format,
				...
				);

			static std::string read_file(
				__in const std::string &path
				);

			static time_t timestamp(void);

			static std::string timestamp_as_string(
				__in const time_t &time
				);
	};
}

#endif // NOMIC_DEFINE_H_
