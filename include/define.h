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
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

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

	#define EXCEPTION_MALFORMED "Malformed exception"
	#define EXCEPTION_UNKNOWN "Unknown exception"

	#define NOMIC "Nomic"
	#define NOMIC_COPYRIGHT "Copyright (C) 2017 David Jolly"
	#define NOMIC_VERSION_MAJOR 0
	#define NOMIC_VERSION_MINOR 1
	#define NOMIC_VERSION_RELEASE "pre-alpha"
	#define NOMIC_VERSION_REVISION 1
	#define NOMIC_VERSION_WEEK 1720

	#define OBJECT_COUNT 1

	#define REFERENCE_INIT 1

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex << (uintptr_t) ((_TYPE_) (_VAL_)) \
			<< std::dec

	#define SCALAR_INVALID(_TYPE_) ((_TYPE_) -1)

	#define STRING_EMPTY "<EMPTY>"
	#define STRING_INVALID "<INVALID>"
	#define STRING_UNKNOWN "<UNKNOWN>"

	#define STRING_CHECK(_STR_) (_STR_.empty() ? STRING_EMPTY : _STR_.c_str())

	#define SUBTYPE_UNDEFINED SCALAR_INVALID(uint32_t)

	#define THREAD_TIMEOUT 5000

	#define THROW_EXCEPTION(_EXCEPT_) THROW_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		nomic::exception::generate(_EXCEPT_, __FILE__, __FUNCTION__, __LINE__, _FORMAT_, __VA_ARGS__)

	#define TIMEOUT_UNDEFINED SCALAR_INVALID(uint32_t)

	#define UID_INVALID 0
}

#endif // NOMIC_DEFINE_H_
