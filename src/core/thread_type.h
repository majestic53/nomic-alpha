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

#ifndef NOMIC_CORE_THREAD_TYPE_H_
#define NOMIC_CORE_THREAD_TYPE_H_

#include "../../include/exception.h"

namespace nomic {

	namespace core {

		#define NOMIC_CORE_THREAD_HEADER "[NOMIC::CORE::THREAD]"
#ifndef NDEBUG
		#define NOMIC_CORE_THREAD_EXCEPTION_HEADER NOMIC_CORE_THREAD_HEADER " "
#else
		#define NOMIC_CORE_THREAD_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NOMIC_CORE_THREAD_EXCEPTION_INTERNAL = 0,
			NOMIC_CORE_THREAD_EXCEPTION_INVALID,
			NOMIC_CORE_THREAD_EXCEPTION_START,
			NOMIC_CORE_THREAD_EXCEPTION_STARTED,
			NOMIC_CORE_THREAD_EXCEPTION_STOPPED,
			NOMIC_CORE_THREAD_EXCEPTION_TIMEOUT,
		};

		#define NOMIC_CORE_THREAD_EXCEPTION_MAX NOMIC_CORE_THREAD_EXCEPTION_TIMEOUT

		static const std::string NOMIC_CORE_THREAD_EXCEPTION_STR[] = {
			NOMIC_CORE_THREAD_EXCEPTION_HEADER "Internal thread exception",
			NOMIC_CORE_THREAD_EXCEPTION_HEADER "Invalid thread operation",
			NOMIC_CORE_THREAD_EXCEPTION_HEADER "Thread failed to start",
			NOMIC_CORE_THREAD_EXCEPTION_HEADER "Thread is started",
			NOMIC_CORE_THREAD_EXCEPTION_HEADER "Thread is stopped",
			NOMIC_CORE_THREAD_EXCEPTION_HEADER "Thread operation timed-out",
			};

		#define NOMIC_CORE_THREAD_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NOMIC_CORE_THREAD_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NOMIC_CORE_THREAD_EXCEPTION_STR[_TYPE_]))

		#define THROW_NOMIC_CORE_THREAD_EXCEPTION(_EXCEPT_) \
			THROW_NOMIC_CORE_THREAD_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NOMIC_CORE_THREAD_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NOMIC_CORE_THREAD_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
	}
}

#endif // NOMIC_CORE_THREAD_TYPE_H_
