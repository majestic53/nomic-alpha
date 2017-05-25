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

#include "../../include/core/event.h"
#include "../../include/trace.h"
#include "./event_type.h"

namespace nomic {

	namespace core {

		event::event(
			__in uint32_t type,
			__in_opt uint32_t subtype,
			__in_opt const std::vector<uint8_t> &data
			) :
				nomic::core::object(type, subtype),
				m_data(data)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x, Data[%u]=%p", type, subtype, data.size(), &data[0]);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		event::event(
			__in const event &other
			) :
				nomic::core::object(other),
				m_data(other.m_data)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x, Data[%u]=%p", other.m_type, other.m_subtype, other.m_data.size(),
				&other.m_data[0]);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		event::~event(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		event &
		event::operator=(
			__in const event &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x, Data[%u]=%p", other.m_type, other.m_subtype, other.m_data.size(),
				&other.m_data[0]);

			if(this != &other) {
				nomic::core::object::operator=(other);
				m_data = other.m_data;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return *this;
		}

		std::vector<uint8_t> &
		event::data(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return m_data;
		}

		std::string 
		event::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			result << NOMIC_CORE_EVENT_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::object::to_string(verbose)
					<< ", Data[" << m_data.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_data[0]);
			}

			return result.str();
		}
	}
}
