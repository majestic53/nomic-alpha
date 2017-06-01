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

#include "../../include/core/id.h"
#include "../../include/uuid/manager.h"
#include "../../include/trace.h"
#include "./id_type.h"

namespace nomic {

	namespace core {

		id::id(void) :
			m_id(UID_INVALID)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			generate();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		id::id(
			__in const id &other
			) :
				m_id(other.m_id)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", other.m_id);

			increment();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		id::~id(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			decrement();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		id &
		id::operator=(
			__in const id &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", other.m_id);

			if(this != &other) {
				decrement();
				m_id = other.m_id;
				increment();
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		bool 
		id::operator==(
			__in const id &other
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=(%x==%x)", m_id, other.m_id);

			result = (m_id == other.m_id);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		id::operator!=(
			__in const id &other
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=(%x!=%x)", m_id, other.m_id);

			result = (m_id != other.m_id);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		id::decrement(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::uuid::manager &instance = nomic::uuid::manager::acquire();
			if(instance.initialized() && instance.contains(m_id)) {
				instance.decrement(m_id);
			}

			instance.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		id::generate(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::uuid::manager &instance = nomic::uuid::manager::acquire();
			if(instance.initialized()) {
				m_id = instance.generate();
			}

			instance.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint32_t 
		id::get_id(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_id);
			return m_id;
		}

		void 
		id::increment(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::uuid::manager &instance = nomic::uuid::manager::acquire();
			if(instance.initialized() && instance.contains(m_id)) {
				instance.increment(m_id);
			}

			instance.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		id::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_CORE_ID_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Id=";

				if(m_id != UID_INVALID) {
					result << SCALAR_AS_HEX(uint32_t, m_id);
				} else {
					result << STRING_INVALID;
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		bool 
		id::valid_id(void)
		{
			bool result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = (m_id != UID_INVALID);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}
	}
}
