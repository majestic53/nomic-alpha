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

#include "../../include/core/primitive.h"
#include "../../include/graphic/manager.h"
#include "../../include/trace.h"
#include "./primitive_type.h"

namespace nomic {

	namespace core {

		primitive::primitive(
			__in uint32_t type,
			__in GLenum subtype
			) :
				nomic::core::object(type, subtype),
				m_handle(HANDLE_INVALID)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x", type, subtype);

			generate();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		primitive::primitive(
			__in const primitive &other
			) :
				nomic::core::object(other),
				m_handle(other.m_handle)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x, Handle=%p", other.m_type, other.m_subtype, other.m_handle);

			increment();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		primitive::~primitive(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			decrement();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		primitive &
		primitive::operator=(
			__in const primitive &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x, Handle=%p", other.m_type, other.m_subtype, other.m_handle);

			if(this != &other) {
				decrement();
				m_handle = other.m_handle;
				increment();
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		primitive::decrement(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::manager &instance = nomic::graphic::manager::acquire();
			if(instance.initialized() && instance.contains(m_type, m_handle)) {
				instance.decrement(m_type, m_handle);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		primitive::generate(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::manager &instance = nomic::graphic::manager::acquire();
			if(instance.initialized()) {
				m_handle = instance.generate(m_type, m_subtype);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		GLuint 
		primitive::handle(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_handle);
			return m_handle;
		}

		void 
		primitive::increment(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::manager &instance = nomic::graphic::manager::acquire();
			if(instance.initialized() && instance.contains(m_type, m_handle)) {
				instance.increment(m_type, m_handle);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		primitive::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_CORE_PRIMITIVE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::object::to_string(verbose)
					<< ", Handle=" << SCALAR_AS_HEX(GLuint, m_handle);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
