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

#include "../../include/entity/diagnostic.h"
#include "../../include/entity/camera.h"
#include "../../include/runtime.h"
#include "../../include/trace.h"
#include "./diagnostic_type.h"

namespace nomic {

	namespace entity {

		diagnostic::diagnostic(
			__in_opt bool verbose,
			__in_opt const glm::vec4 &color,
			__in_opt float scale
			) :
				nomic::entity::string(DIAGNOSTIC_FONT_DEFAULT, DIAGNOSTIC_FONT_SIZE_DEFAULT,
					glm::uvec2(DIAGNOSTIC_TEXT_LEFT_DEFAULT, DIAGNOSTIC_TEXT_TOP_DEFAULT), color, scale),
				m_verbose(verbose)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x, Color={%f, %f, %f, %f}, Scale=%f", verbose, color.x, color.y, color.z, color.w,
				scale);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		diagnostic::diagnostic(
			__in const diagnostic &other
			) :
				nomic::entity::string(other),
				m_verbose(other.m_verbose)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		diagnostic::~diagnostic(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		diagnostic &
		diagnostic::operator=(
			__in const diagnostic &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::string::operator=(other);
				m_verbose = other.m_verbose;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		diagnostic::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);

			nomic::runtime *runtime_ref = (nomic::runtime *) runtime;
			if(runtime_ref) {
				result << NOMIC << " " << runtime_ref->version(true) << std::endl << "Seed=" << runtime_ref->seed();
			}

			if(m_verbose) {
				result << std::endl << std::endl;

				nomic::entity::camera *camera_ref = (nomic::entity::camera *) camera;
				if(camera_ref) {
					glm::vec3 position = camera_ref->position();
					glm::vec3 rotation = camera_ref->rotation();
					glm::uvec3 position_block = camera_ref->block();
					glm::ivec2 position_chunk = camera_ref->chunk();
					result << "Chunk=" << position_chunk.x << "," << position_chunk.y
						<< ", Block=" << position_block.x << "," << position_block.y << "," << position_block.z
						<< ", Mode=" << (runtime_ref->session().underwater() ? "Water" : "Air") << std::endl
						<< std::endl << "Pos=" << position.x << "," << position.y << "," << position.z
						<< std::endl << "Rot=" << rotation.x << "," << rotation.y << "," << rotation.z
						<< std::endl << "Fov=" << camera_ref->fov();
				}

				if(runtime_ref) {
					result << ", Fps=" << runtime_ref->frame_rate() << ", Tick=" << runtime_ref->tick()
						<< "(@" << RUNTIME_TICKS_PER_SECOND << "/sec)";
				}
			}

			m_text = result.str();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		diagnostic::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_DIAGNOSTIC_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::string::to_string(verbose)
					<< ", Mode=" << (m_verbose ? "Verbose" : "Normal");
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
