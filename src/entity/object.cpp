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

#include "../../include/entity/object.h"
#include "../../include/trace.h"
#include "./object_type.h"

namespace nomic {

	namespace entity {

		object::object(
			__in uint32_t type,
			__in uint32_t subtype,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up,
			__in_opt const glm::mat4 &model,
			__in_opt const glm::mat4 &projection,
			__in_opt const glm::mat4 &view
			) :
				nomic::core::entity(type, subtype, position, rotation, up),
				nomic::core::uniform(model, projection, view)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x", type, subtype);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		object::object(
			__in const object &other
			) :
				nomic::core::entity(other),
				nomic::core::uniform(other),
				m_vao(other.m_vao)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		object::~object(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		object &
		object::operator=(
			__in const object &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::core::entity::operator=(other);
				nomic::core::uniform::operator=(other);
				m_vao = other.m_vao;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		object::on_render(
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f", delta);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		object::on_update(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		object::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_OBJECT_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Entity=" << nomic::core::entity::to_string(verbose)
					<< ", Uniform=" << nomic::core::uniform::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		nomic::graphic::vao &
		object::vertex_array(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return m_vao;
		}
	}
}