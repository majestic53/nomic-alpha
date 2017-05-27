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

#include "../../include/core/uniform.h"
#include "../../include/trace.h"
#include "./uniform_type.h"

namespace nomic {

	namespace core {

		uniform::uniform(
			__in_opt const glm::mat4 &model,
			__in_opt const glm::mat4 &projection
			) :
				m_model(model),
				m_projection(projection)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE); 
		}

		uniform::uniform(
			__in const uniform &other
			) :
				m_model(other.m_model),
				m_projection(other.m_projection)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uniform::~uniform(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uniform &
		uniform::operator=(
			__in const uniform &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				m_model = other.m_model;
				m_projection = other.m_projection;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		glm::mat4 &
		uniform::model(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return m_model;
		}

		glm::mat4 &
		uniform::projection(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return m_projection;
		}

		void 
		uniform::reset(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_model = UNIFORM_MATRIX_DEFAULT;
			m_projection = UNIFORM_MATRIX_DEFAULT;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		uniform::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_CORE_UNIFORM_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::transform::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		glm::mat4 
		uniform::view(void)
		{
			glm::mat4 result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = glm::lookAt(m_position, m_position + m_rotation, m_up);

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}
	}
}
