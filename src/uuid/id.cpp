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

#include "../../include/uuid/id.h"
#include "../../include/uuid/manager.h"
#include "./id_type.h"

namespace nomic {

	namespace uuid {

		id::id(void) :
			m_id(UID_INVALID)
		{
			generate();
		}

		id::id(
			__in const id &other
			) :
				m_id(other.m_id)
		{
			increment();
		}

		id::~id(void)
		{
			decrement();
		}

		id &
		id::operator=(
			__in const id &other
			)
		{

			if(this != &other) {
				decrement();
				m_id = other.m_id;
				increment();
			}

			return *this;
		}

		bool 
		id::operator==(
			__in const id &other
			)
		{
			return (m_id == other.m_id);
		}

		bool 
		id::operator!=(
			__in const id &other
			)
		{
			return (m_id != other.m_id);
		}

		void 
		id::decrement(void)
		{

			nomic::uuid::manager &instance = nomic::uuid::manager::acquire();
			if(instance.initialized() && instance.contains(m_id)) {
				instance.decrement(m_id);
			}

			instance.release();
		}

		void 
		id::generate(void)
		{

			nomic::uuid::manager &instance = nomic::uuid::manager::acquire();
			if(instance.initialized()) {
				m_id = instance.generate();
			}

			instance.release();
		}

		uint32_t 
		id::get_id(void)
		{
			return m_id;
		}

		void 
		id::increment(void)
		{

			nomic::uuid::manager &instance = nomic::uuid::manager::acquire();
			if(instance.initialized() && instance.contains(m_id)) {
				instance.increment(m_id);
			}

			instance.release();
		}

		std::string 
		id::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			result << NOMIC_UUID_ID_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Id=";

				if(m_id != UID_INVALID) {
					result << SCALAR_AS_HEX(uint32_t, m_id);
				} else {
					result << STRING_INVALID;
				}
			}

			return result.str();
		}

		bool 
		id::valid_id(void)
		{
			return (m_id != UID_INVALID);
		}
	}
}
