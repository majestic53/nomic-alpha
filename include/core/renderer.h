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

#ifndef NOMIC_CORE_RENDERER_H_
#define NOMIC_CORE_RENDERER_H_

#include "../graphic/program.h"

namespace nomic {

	namespace core {

		class renderer :
				public nomic::graphic::program {

			public:

				explicit renderer(
					__in_opt bool blend = RENDERER_BLEND_DEFAULT,
					__in_opt GLenum blend_sfactor = RENDERER_BLEND_SFACTOR_DEFAULT,
					__in_opt GLenum blend_dfactor = RENDERER_BLEND_DFACTOR_DEFAULT,
					__in_opt bool cull = RENDERER_CULL_DEFAULT,
					__in_opt GLenum cull_mode = RENDERER_CULL_MODE_DEFAULT,
					__in_opt bool depth = RENDERER_DEPTH_DEFAULT,
					__in_opt GLenum depth_mode = RENDERER_DEPTH_MODE_DEFAULT
					);

				renderer(
					__in const renderer &other
					);

				virtual ~renderer(void);

				renderer &operator=(
					__in const renderer &other
					);

				bool blended(void) const;

				bool culled(void) const;

				bool depth(void) const;

				GLuint get_id(void) const;

				uint32_t mode(void) const;

				void set_blend(
					__in bool blend,
					__in_opt GLenum sfactor = RENDERER_BLEND_SFACTOR_DEFAULT,
					__in_opt GLenum dfactor = RENDERER_BLEND_DFACTOR_DEFAULT
					);

				void set_cull(
					__in bool cull,
					__in_opt GLenum mode = RENDERER_CULL_MODE_DEFAULT
					);

				void set_depth(
					__in bool depth,
					__in_opt GLenum mode = RENDERER_DEPTH_MODE_DEFAULT
					);

				void set_mode(
					__in uint32_t mode
					);

				void set_model(
					__in const glm::mat4 &model
					);

				void set_position(
					__in const glm::vec3 &position
					);

				void set_projection(
					__in const glm::mat4 &projection
					);

				void set_underwater(
					__in const GLboolean underwater
					);

				void set_view(
					__in const glm::mat4 &view
					);

				void set_shaders(
					__in const std::string &vertex,
					__in const std::string &fragment
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void use(void);

				void use(
					__in const glm::vec3 &position,
					__in GLboolean underwater,
					__in const glm::mat4 &projection
					);

				void use(
					__in const glm::vec3 &position,
					__in GLboolean underwater,
					__in const glm::mat4 &projection,
					__in const glm::mat4 &view
					);

				void use(
					__in const glm::vec3 &position,
					__in GLboolean underwater,
					__in const glm::mat4 &projection,
					__in const glm::mat4 &view,
					__in const glm::mat4 &model
					);

			protected:

				void add(void);

				void remove(void);

				void set_attributes(void);

				bool m_blend;

				GLenum m_blend_dfactor;

				GLenum m_blend_sfactor;

				bool m_cull;

				GLenum m_cull_mode;

				bool m_depth;

				GLenum m_depth_mode;

				uint32_t m_mode;

				GLint m_uniform_model;

				GLint m_uniform_position;

				GLint m_uniform_projection;

				GLint m_uniform_underwater;

				GLint m_uniform_view;
		};
	}
}

#endif // NOMIC_CORE_RENDERER_H_
