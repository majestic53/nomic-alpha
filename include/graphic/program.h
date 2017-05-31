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

#ifndef NOMIC_GRAPHIC_PROGRAM_H_
#define NOMIC_GRAPHIC_PROGRAM_H_

#include <map>
#include "./shader.h"

namespace nomic {

	namespace graphic {

		class program :
				public nomic::core::primitive {

			public:

				program(void);

				program(
					__in const program &other
					);

				virtual ~program(void);

				program &operator=(
					__in const program &other
					);

				void add_attribute(
					__in GLuint index,
					__in const std::string &name
					);

				void add_shader(
					__in const nomic::graphic::shader &shader
					);

				size_t attribute_count(void);

				bool contains_attribute(
					__in GLuint index
					);

				bool contains_shader(
					__in GLuint handle
					);

				void link(void);

				void remove_all_attributes(void);

				void remove_all_shaders(void);

				void remove_attribute(
					__in GLuint index
					);

				void remove_shader(
					__in GLuint handle
					);

				void set_uniform(
					__in GLint location,
					__in GLboolean value
					);

				void set_uniform(
					__in GLint location,
					__in GLint value
					);

				void set_uniform(
					__in GLint location,
					__in GLuint value
					);

				void set_uniform(
					__in GLint location,
					__in GLfloat value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::ivec2 &value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::ivec3 &value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::ivec4 &value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::uvec2 &value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::uvec3 &value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::uvec4 &value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::vec2 &value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::vec3 &value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::vec4 &value
					);

				void set_uniform(
					__in GLint location,
					__in const glm::mat4 &value
					);

				size_t shader_count(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				GLint uniform_location(
					__in const std::string &name
					);

				void use(void);

			protected:

				std::map<GLuint, std::string>::iterator find_attribute(
					__in GLuint index
					);

				std::map<GLuint, nomic::graphic::shader>::iterator find_shader(
					__in GLuint handle
					);

				std::map<GLuint, std::string> m_attribute;

				std::map<GLuint, nomic::graphic::shader> m_shader;
		};
	}
}

#endif // NOMIC_GRAPHIC_PROGRAM_H_
