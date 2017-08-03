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

#include "../../include/entity/sun.h"
#include "../../include/entity/camera.h"
#include "../../include/runtime.h"
#include "../../include/trace.h"
#include "./sun_type.h"

namespace nomic {

	namespace entity {

		sun::sun(void) :
			nomic::entity::plain(SUN_PATH_DEFAULT),
			m_radius(0)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			setup();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		sun::sun(
			__in const sun &other
			) :
				nomic::entity::plain(other),
				m_radius(other.m_radius)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		sun::~sun(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		sun &
		sun::operator=(
			__in const sun &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::plain::operator=(other);
				m_radius = other.m_radius;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		sun::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);

			if(runtime) {
				glm::vec4 color;
				glm::vec3 center, &position = nomic::entity::plain::position();
				float angle, delta = ((nomic::runtime *) runtime)->tick_cycle();

				if(camera) {
					position = ((nomic::entity::camera *) camera)->position();
					position.y = 0.f;
				}

				angle = (SUN_ANGLE_BEGIN - (SUN_ANGLE_OFFSET * delta));
				position.y += (m_radius * glm::cos(glm::radians(angle)));
				position.z -= (m_radius * glm::sin(glm::radians(angle)));

				if(delta <= 0.5) {
					color = SUN_COLOR_RISE;
					color.y += ((SUN_COLOR_APOGEE.y - SUN_COLOR_RISE.y) * (delta * 2.f));
					color.z += ((SUN_COLOR_APOGEE.z - SUN_COLOR_RISE.z) * (delta * 2.f));
				} else {
					color = SUN_COLOR_APOGEE;
					color.y -= ((SUN_COLOR_APOGEE.y - SUN_COLOR_SET.y) * ((delta - 0.5f) * 2.f));
					color.z -= ((SUN_COLOR_APOGEE.z - SUN_COLOR_SET.z) * ((delta - 0.5f) * 2.f));
				}

				nomic::entity::plain::set_color(color);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		sun::setup(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::entity::plain::set_dimensions(glm::vec2(SUN_SCALE_DEFAULT, SUN_SCALE_DEFAULT));
			m_radius = ((VIEW_RADIUS_RUNTIME * CHUNK_WIDTH) + (2 * CHUNK_WIDTH));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		sun::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_SUN_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::plain::to_string(verbose)
					<< ", Radius=" << m_radius;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
