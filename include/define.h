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
#include <ft2build.h>
#include FT_FREETYPE_H
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
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

	#define ATLAS_DIMENSIONS_DEFAULT glm::uvec2(16, 16)
	#define ATLAS_PATH_DEFAULT "./res/block.bmp"
	#define ATLAS_WIDTH_DEFAULT 16

	#define BLOCK_ATTRIBUTES_DEFAULT (BLOCK_ATTRIBUTE_STATIC | BLOCK_ATTRIBUTE_BREAKABLE)
	#define BLOCK_COLOR_DEFAULT glm::vec4(BLOCK_COLOR_RGBA_DEFAULT)
	#define BLOCK_COLOR_RGBA_DEFAULT 1.f, 1.f, 1.f, 1.f
	#define BLOCK_DEPTH_DIRT_MAX 4
	#define BLOCK_DEPTH_GRAVEL_MAX 4
	#define BLOCK_DEPTH_SAND_MAX 4
	#define BLOCK_DEPTH_SANDSTONE_MAX 4
	#define BLOCK_FILTER_MAG_DEFAULT GL_NEAREST
	#define BLOCK_FILTER_MIN_DEFAULT GL_NEAREST
	#define BLOCK_HEIGHT_BOUNDARY 1
	#define BLOCK_HEIGHT_MAX 120
	#define BLOCK_HEIGHT_MIN 20
	#define BLOCK_HEIGHT_WATER (BLOCK_LEVEL_BEACH_SAND - 1)
	#define BLOCK_LEVEL_ALPINE_PEAK 96
	#define BLOCK_LEVEL_ALPINE 90
	#define BLOCK_LEVEL_GRASS_STEP 86
	#define BLOCK_LEVEL_GRASS 60
	#define BLOCK_LEVEL_BEACH_ROCKS 58
	#define BLOCK_LEVEL_BEACH_GRAVEL 56
	#define BLOCK_LEVEL_BEACH_SAND 50
	#define BLOCK_RADIUS (BLOCK_WIDTH / 2.f)
	#define BLOCK_SCALE_DEFAULT 1.f
	#define BLOCK_SCALE_MIN 0.f
	#define BLOCK_WIDTH 1.f
	#define BLOCK_WRAP_S_DEFAULT GL_CLAMP_TO_EDGE
	#define BLOCK_WRAP_T_DEFAULT GL_CLAMP_TO_EDGE

	#define CAMERA_CLIP_MAX 1000.f
	#define CAMERA_CLIP_MIN 0.1f
	#define CAMERA_FOV_DEFAULT 55.f
	#define CAMERA_FOV_MAX 90.f
	#define CAMERA_FOV_MIN 10.f
	#define CAMERA_PITCH_MAX 89.f
	#define CAMERA_PITCH_MIN -CAMERA_PITCH_MAX
	#define CAMERA_POSITION_DEFAULT glm::vec3(0.f, BLOCK_LEVEL_GRASS, 0.f)
	#define CAMERA_ROTATION_DEFAULT glm::vec3(0.f, 0.f, 0.f)
	#define CAMERA_SENSITIVITY 0.55f
	#define CAMERA_SPEED 0.35f
	#define CAMERA_STRAFE 0.35f
	#define CAMERA_UP_DEFAULT glm::vec3(0.f, 1.f, 0.f)
	#define CAMERA_YAW_MAX 360.f
	#define CAMERA_YAW_MIN 0.f

	#define CHANNEL_MAX UINT8_MAX

	#define CHARACTER_FILL ' '
	#define CHARACTER_NEWLINE '\n'

	#define CHARACTER_FILTER_MAG_DEFAULT GL_LINEAR
	#define CHARACTER_FILTER_MIN_DEFAULT GL_LINEAR
	#define CHARACTER_WRAP_S_DEFAULT GL_CLAMP_TO_EDGE
	#define CHARACTER_WRAP_T_DEFAULT GL_CLAMP_TO_EDGE

	#define CHUNK_ADJOIN_MIN 3
	#define CHUNK_BLOCK_COUNT (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH)
	#define CHUNK_BLOCK_SURFACE_COUNT (CHUNK_WIDTH * CHUNK_WIDTH)
	#define CHUNK_MAX_DEFAULT 64
	#define CHUNK_HEIGHT 128
	#define CHUNK_RADIUS (CHUNK_WIDTH / 2.f)
	#define CHUNK_WIDTH 16

	#define CUBEMAP_FILTER_MAG_DEFAULT GL_LINEAR
	#define CUBEMAP_FILTER_MIN_DEFAULT GL_LINEAR
	#define CUBEMAP_SCALE_DEFAULT 1.f
	#define CUBEMAP_WRAP_R_DEFAULT GL_CLAMP_TO_EDGE
	#define CUBEMAP_WRAP_S_DEFAULT GL_CLAMP_TO_EDGE
	#define CUBEMAP_WRAP_T_DEFAULT GL_CLAMP_TO_EDGE

	#define DIAGNOSTIC_FONT_DEFAULT "/usr/share/fonts/TTF/c64_pro_mono.ttf"
	#define DIAGNOSTIC_FONT_SIZE_DEFAULT 12
	#define DIAGNOSTIC_TEXT_DEFAULT STRING_INVALID
	#define DIAGNOSTIC_TEXT_LEFT_DEFAULT 10.f
	#define DIAGNOSTIC_TEXT_TOP_DEFAULT 25.f

	#define DISPLAY_DEFAULT_ALPHA CHANNEL_MAX
	#define DISPLAY_DEFAULT_BLUE (122.f / 255.f)
	#define DISPLAY_DEFAULT_FULLSCREEN false
	#define DISPLAY_DEFAULT_GREEN (79.f / 255.f)
	#define DISPLAY_DEFAULT_HEIGHT 768
	#define DISPLAY_DEFAULT_ICON "./res/icon.bmp"
	#define DISPLAY_DEFAULT_RED (41.f / 255.f)
	#define DISPLAY_DEFAULT_TITLE NOMIC
	#define DISPLAY_DEFAULT_VSYNC false
	#define DISPLAY_DEFAULT_WIDTH 1024
	#define DISPLAY_FLAGS_CLEAR (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
	#define DISPLAY_FLAGS_DEFAULT (SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_OPENGL)
	#define DISPLAY_MOUSE_RELATIVE SDL_TRUE

	#define EXCEPTION_UNKNOWN "Unknown exception"

	#define FONT_CHARACTER_MIN 0
	#define FONT_CHARACTER_MAX 127
	#define FONT_SIZE_DEFAULT 12

	#define FORMAT_STRING(_FORMAT_, ...) nomic::utility::format_as_string(_FORMAT_, __VA_ARGS__)

	#define GL_ATTRIBUTE_ACCELERATE_VISUAL 1
	#define GL_ATTRIBUTE_COLOR_SIZE 8
	#define GL_ATTRIBUTE_DEPTH_SIZE 16
	#define GL_ATTRIBUTE_DOUBLEBUFFER 1
	#define GL_ATTRIBUTE_MAJOR_VERSION 3
	#define GL_ATTRIBUTE_MINOR_VERSION 3

#ifndef NDEBUG
	#define _GL_CHECK_ERROR(_LEVEL_, _COMMAND_) { \
		GLenum err = glGetError(); \
		while(err != GL_NO_ERROR) { \
			TRACE_MESSAGE_FORMAT(_LEVEL_, "%s failed! Error=%x(%s)", STRING_CONCAT(_COMMAND_), err, GL_ERROR_AS_STRING(err)); \
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

	#define INPUT_EVENT_QUEUING

	#define MESSAGE_POSITION_DEFAULT glm::uvec2(MESSAGE_POSITION_LEFT_DEFAULT, MESSAGE_POSITION_TOP_DEFAULT)
	#define MESSAGE_POSITION_LEFT_DEFAULT 10
	#define MESSAGE_POSITION_TOP_DEFAULT 10

	#define MILLISECONDS_PER_SECOND 1000

	#define NOISE_AMPLITUDE_DEFAULT 1.0
	#define NOISE_AMPLITUDE_MIN 0.0
	#define NOISE_OCTAVES_DEFAULT 10
	#define NOISE_PERSISTENCE_DEFAULT 1.0
	#define NOISE_PERSISTENCE_MIN 0.0
	#define NOISE_SCALE 356.0

	#define NOISE_SEED_DEFAULT 612443400 //0

	#define NOMIC "Nomic"
	#define NOMIC_COPYRIGHT "Copyright (C) 2017 David Jolly"
	#define NOMIC_VERSION_MAJOR 0
	#define NOMIC_VERSION_MINOR 1
	#define NOMIC_VERSION_RELEASE "alpha"
	#define NOMIC_VERSION_REVISION 2
	#define NOMIC_VERSION_WEEK 1729

	#define OBJECT_COUNT 1

	#define PLAIN_COLOR_DEFAULT glm::vec4(PLAIN_COLOR_RGBA_DEFAULT)
	#define PLAIN_COLOR_RGBA_DEFAULT 1.f, 1.f, 1.f, 1.f
	#define PLAIN_DIMENSION_DEFAULT glm::vec2(1.f)
	#define PLAIN_SCALE_DEFAULT 1.f

	#define RANDOM_SEED_DEFAULT 0

	#define QUEUE_MAX 0x1000

	#define REFERENCE_INIT 1

	#define RENDERER_BLEND_DEFAULT true
	#define RENDERER_BLEND_DFACTOR_DEFAULT GL_ONE_MINUS_SRC_ALPHA
	#define RENDERER_BLEND_SFACTOR_DEFAULT GL_SRC_ALPHA
	#define RENDERER_CULL_DEFAULT true
	#define RENDERER_CULL_MODE_DEFAULT GL_BACK
	#define RENDERER_DEPTH_DEFAULT true
	#define RENDERER_DEPTH_MODE_DEFAULT GL_LESS

	#define RUNTIME_PAUSE_DELAY 200
	#define RUNTIME_TICKS_PER_SECOND 60
	#define RUNTIME_TICK_SKIP 5

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex << (uintptr_t) ((_TYPE_) (_VAL_)) \
			<< std::dec

	#define SCALAR_INVALID(_TYPE_) ((_TYPE_) -1)

	#define SCALE_DEFAULT 1.f

	#define SDL_FLAGS_INIT (SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO)

	#define SEND_EVENT(_EVENT_) { \
		nomic::event::manager &instance = nomic::event::manager::acquire(); \
		if(instance.initialized()) { \
			instance.receive_event(_EVENT_); \
		} \
		instance.release(); \
		}

	#define SESSION_DEBUG_DEFAULT true

	#define STRING_EMPTY "<EMPTY>"
	#define STRING_INVALID "<INVALID>"
	#define STRING_UNKNOWN "<UNKNOWN>"

	#define STRING_ADVANCE_SHIFT 6
	#define STRING_COLOR_DEFAULT glm::vec4(1.f, 1.f, 1.f, 1.f)
	#define STRING_FONT_DEFAULT "/usr/share/fonts/TTF/c64_pro_mono.ttf"
	#define STRING_FONT_SIZE_DEFAULT 12
	#define STRING_POSITION_DEFAULT glm::uvec2(10, 10)
	#define STRING_SCALE_DEFAULT 1.f
	#define STRING_VERTICAL_PAD 5

	#define STRING_CHECK(_STR_) (_STR_.empty() ? STRING_EMPTY : _STR_.c_str())

	#define _STRING_CONCAT(_STRING_) # _STRING_
	#define STRING_CONCAT(_STRING_) _STRING_CONCAT(_STRING_)

	#define SUBTYPE_UNDEFINED SCALAR_INVALID(uint32_t)

	#define TEXTURE_FILTER_MAG_DEFAULT GL_NEAREST
	#define TEXTURE_FILTER_MIN_DEFAULT GL_NEAREST
	#define TEXTURE_WRAP_S_DEFAULT GL_CLAMP_TO_EDGE
	#define TEXTURE_WRAP_T_DEFAULT GL_CLAMP_TO_EDGE

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

	#define UNIFORM_COLOR "color"
	#define UNIFORM_MODEL "model"
	#define UNIFORM_POSITION "position"
	#define UNIFORM_PROJECTION "projection"
	#define UNIFORM_UNDERWATER "underwater"
	#define UNIFORM_VIEW "view"

	#define VIEW_RADIUS_RUNTIME (VIEW_WIDTH / 2)
	#define VIEW_RADIUS_SPAWN (VIEW_WIDTH / 2)
	#define VIEW_SELECTIVE_SHOW
	#define VIEW_WIDTH 30

	enum {
		BITMAP_DEPTH_8 = 1,
		BITMAP_DEPTH_16,
		BITMAP_DEPTH_24,
		BITMAP_DEPTH_32,
	};

	enum {
		BLOCK_AIR = 0,
		BLOCK_BOUNDARY,
		BLOCK_DIRT,
		BLOCK_DIRT_GRASS_SIDE,
		BLOCK_GRASS,
		BLOCK_STONE,
		BLOCK_STONE_SNOW_SIDE,
		BLOCK_SNOW,
		BLOCK_GRAVEL,
		BLOCK_SAND,
		BLOCK_SANDSTONE,
		BLOCK_SANDSTONE_SIDE,
		BLOCK_WATER,
	};

	#define BLOCK_MAX BLOCK_WATER

	static const std::string BLOCK_STR[] = {
		"Air", "Boundary", "Dirt", "Dirt/Grass", "Grass", "Stone", "Stone/Snow", "Snow",
		"Gravel", "Sand", "Sandstone", "Sandstone/Side", "Water",
		};

	#define BLOCK_STRING(_TYPE_) \
		(((_TYPE_) > BLOCK_MAX) ? STRING_UNKNOWN : STRING_CHECK(BLOCK_STR[_TYPE_]))

	enum {
		BLOCK_ATTRIBUTE_STATIC = 1,
		BLOCK_ATTRIBUTE_BREAKABLE = 2,
		BLOCK_ATTRIBUTE_HIDDEN = 4,
	};

	enum {
		BLOCK_FACE_RIGHT = 0,
		BLOCK_FACE_LEFT,
		BLOCK_FACE_TOP,
		BLOCK_FACE_BOTTOM,
		BLOCK_FACE_BACK,
		BLOCK_FACE_FRONT,
	};

	#define BLOCK_FACE_COUNT (BLOCK_FACE_MAX + 1)
	#define BLOCK_FACE_MIN BLOCK_FACE_RIGHT
	#define BLOCK_FACE_MAX BLOCK_FACE_FRONT

	enum {
		BLOCK_ZONE_ALPINE = 0,
		BLOCK_ZONE_BEACH,
		BLOCK_ZONE_BOUNDARY,
		BLOCK_ZONE_GRASS,
		BLOCK_ZONE_PEAK,
		BLOCK_ZONE_SEA,
	};

	enum {
		ENTITY_CAMERA = 0,
		ENTITY_SKYBOX,
		ENTITY_STRING,
		ENTITY_PLAIN,
		ENTITY_BLOCK,
		ENTITY_CHUNK,
		ENTITY_AXIS,
		ENTITY_RETICLE,
	};

	enum {
		EVENT_UNDEFINED = 0,
		EVENT_INPUT,
	};

	enum {
		KEY_BACKWARD = SDL_SCANCODE_S,
		KEY_DEBUG = SDL_SCANCODE_GRAVE,
		KEY_DESCEND = SDL_SCANCODE_X,
		KEY_ELEVATE = SDL_SCANCODE_SPACE,
		KEY_FORWARD = SDL_SCANCODE_W,
		KEY_LEFT = SDL_SCANCODE_A,
		KEY_LEFT_STRAFE = SDL_SCANCODE_Q,
		KEY_RIGHT = SDL_SCANCODE_D,
		KEY_RIGHT_STRAFE = SDL_SCANCODE_E,
	};

	enum {
		INPUT_BUTTON = 0,
		INPUT_KEY,
		INPUT_MOTION,
		INPUT_WHEEL,
	};

	enum {
		PRIMITIVE_CHARACTER = 0,
		PRIMITIVE_CUBEMAP,
		PRIMITIVE_PROGRAM,
		PRIMITIVE_SHADER,
		PRIMITIVE_TEXTURE,
		PRIMITIVE_VAO,
		PRIMITIVE_VBO,
	};

	enum {
		RENDER_PERSPECTIVE = 0,
		RENDER_ORTHOGONAL,
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
