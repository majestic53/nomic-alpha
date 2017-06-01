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

#include "../../include/core/entity.h"
#include "../../include/trace.h"
#include "./entity_type.h"

namespace nomic {

	namespace core {

		entity::entity(
			__in uint32_t type,
			__in uint32_t subtype
			) :
				nomic::core::object(type, subtype)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x", type, subtype);

			generate();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		entity::entity(
			__in const entity &other
			) :
				nomic::core::id(other),
				nomic::core::object(other)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			increment();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		entity::~entity(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			decrement();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		entity &
		entity::operator=(
			__in const entity &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				decrement();
				nomic::core::id::operator=(other);
				nomic::core::object::operator=(other);
				increment();
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", this);
			return *this;
		}

		void 
		entity::decrement(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			// TODO: decrement entry in entity manager

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::generate(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			// TODO: form entry in entity manager

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::increment(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			// TODO: increment entry in entity manager

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::render(
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f", delta);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		entity::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_CORE_ENTITY_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::object::to_string(verbose)
					<< ", Id=" << nomic::core::id::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		entity::update(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
