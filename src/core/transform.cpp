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

#include "../../include/core/transform.h"
#include "../../include/trace.h"
#include "./transform_type.h"

namespace nomic {

	namespace core {

		transform::transform(
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up
			) :
				m_position(position),
				m_rotation(rotation),
				m_up(up)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}",
				position.x, position.y, position.z, rotation.x, rotation.y, rotation.z,
				up.x, up.y, up.z);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		transform::transform(
			__in const transform &other
			) :
				m_position(other.m_position),
				m_rotation(other.m_rotation),
				m_up(other.m_up)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}",
				other.m_position.x, other.m_position.y, other.m_position.z,
				other.m_rotation.x, other.m_rotation.y, other.m_rotation.z,
				other.m_up.x, other.m_up.y, other.m_up.z);
			TRACE_EXIT(LEVEL_VERBOSE);	
		}

		transform::~transform(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		transform &
		transform::operator=(
			__in const transform &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}",
				other.m_position.x, other.m_position.y, other.m_position.z,
				other.m_rotation.x, other.m_rotation.y, other.m_rotation.z,
				other.m_up.x, other.m_up.y, other.m_up.z);

			if(this != &other) {
				m_position = other.m_position;
				m_rotation = other.m_rotation;
				m_up = other.m_up;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		glm::vec3 &
		transform::position(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}", m_position.x, m_position.y, m_position.z);
			return m_position;
		}

		void 
		transform::reset(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_position = TRANSFORM_POSITION_DEFAULT;
			m_rotation = TRANSFORM_ROTATION_DEFAULT;
			m_up = TRANSFORM_UP_DEFAULT;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		glm::vec3 &
		transform::rotation(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Rotation={%f, %f, %f}", m_rotation.x, m_rotation.y, m_rotation.z);
			return m_rotation;
		}

		std::string 
		transform::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_CORE_TRANSFORM_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Position={" << m_position.x << ", " << m_position.y << ", " << m_position.z << "}"
					<< ", Rotation={" << m_rotation.x << ", " << m_rotation.y << ", " << m_rotation.z << "}"
					<< ", Up={" << m_up.x << ", " << m_up.y << ", " << m_up.z << "}";
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		glm::vec3 &
		transform::up(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Up={%f, %f, %f}", m_up.x, m_up.y, m_up.z);
			return m_up;
		}
	}
}
