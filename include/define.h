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

	#define EXCEPTION_UNKNOWN "Unknown exception"

	#define FORMAT_STRING(_FORMAT_, ...) nomic::utility::format_as_string(_FORMAT_, __VA_ARGS__)

	#define MILLISECONDS_PER_SECOND 1000

	#define NOMIC "Nomic"
	#define NOMIC_COPYRIGHT "Copyright (C) 2017 David Jolly"
	#define NOMIC_VERSION_MAJOR 0
	#define NOMIC_VERSION_MINOR 1
	#define NOMIC_VERSION_RELEASE "pre-alpha"
	#define NOMIC_VERSION_REVISION 3
	#define NOMIC_VERSION_WEEK 1720

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

	#define STRING_EMPTY "<EMPTY>"
	#define STRING_INVALID "<INVALID>"
	#define STRING_UNKNOWN "<UNKNOWN>"

	#define STRING_CHECK(_STR_) (_STR_.empty() ? STRING_EMPTY : _STR_.c_str())

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
				instance.release(); \
			} \
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

	#define UID_INVALID 0

	class utility {

		public:

			static std::string format_as_string(
				__in const char *format,
				...
				);

			static time_t timestamp(void);

			static std::string timestamp_as_string(
				__in const time_t &time
				);
	};
}

#endif // NOMIC_DEFINE_H_
