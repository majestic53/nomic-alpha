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

#include "../../include/entity/shadowmap.h"
#include "../../include/trace.h"
#include "./shadowmap_type.h"

namespace nomic {

	namespace entity {

		shadowmap::shadowmap(
			__in_opt GLint depth,
			__in_opt const glm::uvec2 &dimensions
			) :
				nomic::entity::object(ENTITY_SHADOWMAP),
				m_depth(UNIFORM_MATRIX_DEFAULT)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Depth=%x, Dimensions={%u, %u}", depth, dimensions.x, dimensions.y);

			setup(depth, dimensions);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		shadowmap::shadowmap(
			__in const shadowmap &other
			) :
				nomic::entity::object(other),
				m_depth(other.m_depth),
				m_fbo(other.m_fbo)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		shadowmap::~shadowmap(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		shadowmap &
		shadowmap::operator=(
			__in const shadowmap &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				m_depth = other.m_depth;
				m_fbo = other.m_fbo;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		nomic::graphic::fbo &
		shadowmap::fbo(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return m_fbo;
		}

		// TODO: add vertex/fragment shader

		void 
		shadowmap::on_render(
			__in nomic::core::renderer &renderer,
			__in void *textures,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Textures=%p, Delta=%f", &renderer, textures, delta);

			renderer.set_depth_matrix(m_depth);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		shadowmap::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);

			// TODO: update depth matrix with sun position

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		shadowmap::setup(
			__in GLint depth,
			__in const glm::uvec2 &dimensions
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Depth=%x, Dimensions={%u, %u}", depth, dimensions.x, dimensions.y);

			m_fbo.set(GL_TEXTURE_2D, 0, depth, dimensions.x, dimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0, GL_DEPTH_ATTACHMENT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		shadowmap::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_SHADOWMAP_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Fbo=" << m_fbo.to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
