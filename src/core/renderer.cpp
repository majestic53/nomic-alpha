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

#include "../../include/core/renderer.h"
#include "../../include/render/manager.h"
#include "../../include/trace.h"
#include "./renderer_type.h"

namespace nomic {

	namespace core {

		renderer::renderer(
			__in uint32_t type,
			__in_opt bool blend,
			__in_opt GLenum blend_sfactor,
			__in_opt GLenum blend_dfactor,
			__in_opt bool cull,
			__in_opt GLenum cull_mode,
			__in_opt bool depth,
			__in_opt GLenum depth_mode
			) :
				m_blend(blend),
				m_blend_dfactor(blend_dfactor),
				m_blend_sfactor(blend_sfactor),
				m_cull(cull),
				m_cull_mode(cull_mode),
				m_depth(depth),
				m_depth_mode(depth_mode),
				m_mode(RENDER_PERSPECTIVE),
				m_type(type),
				m_uniform_model(0),
				m_uniform_position(0),
				m_uniform_projection(0),
				m_uniform_rotation(0),
				m_uniform_underwater(0),
				m_uniform_view(0)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Blend=%x, Blend Factor={%x, %x}, Cull=%x, Cull Mode=%x, Depth=%x, Depth Mode=%x",
				blend, blend_sfactor, blend_dfactor, cull, cull_mode, depth, depth_mode);

			add();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		renderer::renderer(
			__in const renderer &other
			) :
				nomic::graphic::program(other),
				m_blend(other.m_blend),
				m_blend_dfactor(other.m_blend_dfactor),
				m_blend_sfactor(other.m_blend_sfactor),
				m_cull(other.m_cull),
				m_cull_mode(other.m_cull_mode),
				m_depth(other.m_depth),
				m_depth_mode(other.m_depth_mode),
				m_mode(other.m_mode),
				m_type(other.m_type),
				m_uniform_model(other.m_uniform_model),
				m_uniform_position(other.m_uniform_position),
				m_uniform_projection(other.m_uniform_projection),
				m_uniform_rotation(other.m_uniform_rotation),
				m_uniform_underwater(other.m_uniform_underwater),
				m_uniform_view(other.m_uniform_view)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Blend=%x, Blend Factor={%x, %x}, Cull=%x, Cull Mode=%x, Depth=%x, Depth Mode=%x",
				other.m_blend, other.m_blend_sfactor, other.m_blend_dfactor, other.m_cull, other.m_cull_mode, other.m_depth,
				other.m_depth_mode);

			add();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		renderer::~renderer(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			remove();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		renderer &
		renderer::operator=(
			__in const renderer &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Blend=%x, Blend Factor={%x, %x}, Cull=%x, Cull Mode=%x, Depth=%x, Depth Mode=%x",
				other.m_blend, other.m_blend_sfactor, other.m_blend_dfactor, other.m_cull, other.m_cull_mode, other.m_depth,
				other.m_depth_mode);

			if(this != &other) {
				remove();
				nomic::graphic::program::operator=(other);
				m_blend = other.m_blend;
				m_blend_dfactor = other.m_blend_dfactor;
				m_blend_sfactor = other.m_blend_sfactor;
				m_cull = other.m_cull;
				m_cull_mode = other.m_cull_mode;
				m_depth = other.m_depth;
				m_depth_mode = other.m_depth_mode;
				m_mode = other.m_mode;
				m_type = other.m_type;
				m_uniform_model = other.m_uniform_model;
				m_uniform_position = other.m_uniform_position;
				m_uniform_projection = other.m_uniform_projection;
				m_uniform_rotation = other.m_uniform_rotation;
				m_uniform_underwater = other.m_uniform_underwater;
				m_uniform_view = other.m_uniform_view;
				add();
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		renderer::add(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::render::manager &instance = nomic::render::manager::acquire();
			if(instance.initialized()) {
				instance.add(this);
			}

			instance.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		renderer::blended(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_blend);
			return m_blend;
		}

		bool 
		renderer::culled(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_cull);
			return m_cull;
		}

		bool 
		renderer::depth(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_depth);
			return m_depth;
		}

		uint32_t 
		renderer::mode(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_mode);
			return m_mode;
		}

		void 
		renderer::remove(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::render::manager &instance = nomic::render::manager::acquire();
			if(instance.initialized() && instance.contains(m_type)) {
				instance.remove(this);
			}

			instance.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_attributes(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(m_blend) {
				GL_CHECK(LEVEL_WARNING, glEnable, GL_BLEND);
				GL_CHECK(LEVEL_WARNING, glBlendFunc, m_blend_sfactor, m_blend_dfactor);
			} else {
				GL_CHECK(LEVEL_WARNING, glDisable, GL_BLEND);
			}

			if(m_cull) {
				GL_CHECK(LEVEL_WARNING, glEnable, GL_CULL_FACE);
				GL_CHECK(LEVEL_WARNING, glCullFace, m_cull_mode);
			} else {
				GL_CHECK(LEVEL_WARNING, glDisable, GL_CULL_FACE);
			}

			if(m_depth) {
				GL_CHECK(LEVEL_WARNING, glEnable, GL_DEPTH_TEST);
				GL_CHECK(LEVEL_WARNING, glDepthFunc, m_depth_mode);
			} else {
				GL_CHECK(LEVEL_WARNING, glDisable, GL_DEPTH_TEST);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_blend(
			__in bool blend,
			__in_opt GLenum sfactor,
			__in_opt GLenum dfactor
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Blend=%x, Factor={%x, %x}", blend, sfactor, dfactor);

			m_blend = blend;
			m_blend_dfactor = dfactor;
			m_blend_sfactor = sfactor;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_cull(
			__in bool cull,
			__in_opt GLenum mode
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Cull=%x, Mode=%x", cull, mode);

			m_cull = cull;
			m_cull_mode = mode;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_depth(
			__in bool depth,
			__in_opt GLenum mode
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Depth=%x, Mode=%x", depth, mode);

			m_depth = depth;
			m_depth_mode = mode;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_mode(
			__in uint32_t mode
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Mode=%x", mode);

			m_mode = mode;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_model(
			__in const glm::mat4 &model
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::program::set_uniform(m_uniform_model, model);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_position(
			__in const glm::vec3 &position
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::program::set_uniform(m_uniform_position, position);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_projection(
			__in const glm::mat4 &projection
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::program::set_uniform(m_uniform_projection, projection);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_rotation(
			__in const glm::vec3 &rotation
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::program::set_uniform(m_uniform_rotation, rotation);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_underwater(
			__in const GLboolean underwater
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Underwater=%x", underwater);

			nomic::graphic::program::set_uniform(m_uniform_underwater, underwater);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_view(
			__in const glm::mat4 &view
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::program::set_uniform(m_uniform_view, view);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::set_shaders(
			__in const std::string &vertex,
			__in const std::string &fragment
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Vertex[%u]=%s, Fragment[%u]=%p", vertex.size(), STRING_CHECK(vertex), fragment.size(),
				STRING_CHECK(fragment));

			nomic::graphic::program::add_shader(nomic::graphic::shader(GL_VERTEX_SHADER, vertex));
			nomic::graphic::program::add_shader(nomic::graphic::shader(GL_FRAGMENT_SHADER, fragment));
			nomic::graphic::program::link();
			m_uniform_model = nomic::graphic::program::uniform_location(UNIFORM_MODEL);
			m_uniform_position = nomic::graphic::program::uniform_location(UNIFORM_POSITION);
			m_uniform_projection = nomic::graphic::program::uniform_location(UNIFORM_PROJECTION);
			m_uniform_rotation = nomic::graphic::program::uniform_location(UNIFORM_ROTATION);
			m_uniform_underwater = nomic::graphic::program::uniform_location(UNIFORM_UNDERWATER);
			m_uniform_view = nomic::graphic::program::uniform_location(UNIFORM_VIEW);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		renderer::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_CORE_RENDERER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::graphic::program::to_string(verbose)
					<< ", Type=" << SCALAR_AS_HEX(uint32_t, m_type) << "(" << RENDERER_STRING(m_type) << ")"
					<< ", Blend=" << m_blend << ", Blend Factor={" << SCALAR_AS_HEX(GLenum, m_blend_sfactor)
					<< ", " << SCALAR_AS_HEX(GLenum, m_blend_dfactor) << "}, Cull=" << m_cull
					<< ", Cull Mode=" << SCALAR_AS_HEX(GLenum, m_cull_mode)
					<< ", Depth=" << m_depth << ", Depth Mode=" << SCALAR_AS_HEX(GLenum, m_depth_mode)
					<< ", Mode=" << SCALAR_AS_HEX(uint32_t, m_mode)
						<< "(" << ((m_mode == RENDER_PERSPECTIVE) ? "Perspective" : "Orthogonal") << ")"
					<< ", Model=" << m_uniform_model << ", Position=" << m_uniform_position
					<< ", Rotation=" << m_uniform_rotation << ", Projection=" << m_uniform_projection
					<< ", Underwater=" << m_uniform_underwater << ", View=" << m_uniform_view;
			}

			return result.str();
		}

		uint32_t 
		renderer::type(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_type);
			return m_type;
		}

		void 
		renderer::use(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			
			nomic::graphic::program::use();
			set_attributes();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::use(
			__in const glm::vec3 &position,
			__in const glm::vec3 &rotation,
			__in GLboolean underwater,
			__in const glm::mat4 &projection
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Rotation={%f, %f, %f}, Underwater=%x, Projection=%p",
				position.x, position.y, position.z, rotation.x, rotation.y, rotation.z, underwater, &projection);

			nomic::graphic::program::use();
			nomic::graphic::program::set_uniform(m_uniform_position, position);
			nomic::graphic::program::set_uniform(m_uniform_projection, projection);
			nomic::graphic::program::set_uniform(m_uniform_rotation, rotation);
			nomic::graphic::program::set_uniform(m_uniform_underwater, underwater);
			set_attributes();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::use(
			__in const glm::vec3 &position,
			__in const glm::vec3 &rotation,
			__in GLboolean underwater,
			__in const glm::mat4 &projection,
			__in const glm::mat4 &view
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Rotation={%f, %f, %f}, Underwater=%x, Projection=%p, View=%p",
				position.x, position.y, position.z, rotation.x, rotation.y, rotation.z, underwater, &projection, &view);

			nomic::graphic::program::use();
			nomic::graphic::program::set_uniform(m_uniform_position, position);
			nomic::graphic::program::set_uniform(m_uniform_projection, projection);
			nomic::graphic::program::set_uniform(m_uniform_rotation, rotation);
			nomic::graphic::program::set_uniform(m_uniform_underwater, underwater);
			nomic::graphic::program::set_uniform(m_uniform_view, view);
			set_attributes();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		renderer::use(
			__in const glm::vec3 &position,
			__in const glm::vec3 &rotation,
			__in GLboolean underwater,
			__in const glm::mat4 &projection,
			__in const glm::mat4 &view,
			__in const glm::mat4 &model
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,
				"Position={%f, %f, %f}, Rotation={%f, %f, %f}, Underwater=%x, Projection=%p, View=%p, Model=%p",
				position.x, position.y, position.z, rotation.x, rotation.y, rotation.z, underwater, &projection, &view, &model);

			nomic::graphic::program::use();
			nomic::graphic::program::set_uniform(m_uniform_model, model);
			nomic::graphic::program::set_uniform(m_uniform_position, position);
			nomic::graphic::program::set_uniform(m_uniform_projection, projection);
			nomic::graphic::program::set_uniform(m_uniform_rotation, rotation);
			nomic::graphic::program::set_uniform(m_uniform_underwater, underwater);
			nomic::graphic::program::set_uniform(m_uniform_view, view);
			set_attributes();

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
