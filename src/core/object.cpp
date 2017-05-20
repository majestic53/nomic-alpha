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

#include "../../include/core/object.h"
#include "./object_type.h"

namespace nomic {

	namespace core {

		object::object(
			__in uint32_t type,
			__in_opt uint32_t subtype
			) :
				m_subtype(subtype),
				m_type(type)
		{
			return;
		}

		object::object(
			__in const object &other
			) :
				m_subtype(other.m_subtype),
				m_type(other.m_type)
		{
			return;
		}

		object::~object(void)
		{
			return;
		}

		object &
		object::operator=(
			__in const object &other
			)
		{

			if(this != &other) {
				m_subtype = other.m_subtype;
				m_type = other.m_type;
			}

			return *this;
		}

		bool 
		object::has_subtype(void) const
		{
			return (m_subtype != SUBTYPE_UNDEFINED);
		}

		uint32_t 
		object::subtype(void) const
		{
			return m_subtype;
		}

		std::string 
		object::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			result << NOMIC_CORE_OBJECT_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Type=" << SCALAR_AS_HEX(uint32_t, m_type);

				if(m_subtype != SUBTYPE_UNDEFINED) {
					result << ", Subtype=" << SCALAR_AS_HEX(uint32_t, m_subtype);
				}
			}

			return result.str();
		}

		uint32_t 
		object::type(void) const
		{
			return m_type;
		}
	}
}
