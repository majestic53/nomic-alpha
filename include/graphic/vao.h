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

#ifndef NOMIC_GRAPHIC_VAO_H_
#define NOMIC_GRAPHIC_VAO_H_

#include <map>
#include "./vbo.h"

namespace nomic {

	namespace graphic {

		class vao :
				public nomic::core::primitive {

			public:

				vao(void);

				vao(
					__in const vao &other
					);

				virtual ~vao(void);

				vao &operator=(
					__in const vao &other
					);

				void add(
					__in const nomic::graphic::vbo &vbo,
					__in GLuint index,
					__in GLint size,
					__in GLenum type,
					__in_opt GLboolean normalized = GL_FALSE,
					__in_opt GLsizei stride = 0,
					__in_opt const GLvoid *pointer = nullptr
					);

				void bind(void) const;

				bool contains(
					__in GLuint index
					) const;

				void disable(
					__in GLuint index
					) const;

				void disable_all(void) const;

				void enable(
					__in GLuint index
					) const;

				void enable_all(void) const;

				void remove(
					__in GLuint index
					);

				void remove_all(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				std::map<GLuint, nomic::graphic::vbo>::iterator find(
					__in GLuint index
					);

				std::map<GLuint, nomic::graphic::vbo> m_vbo;
		};
	}
}

#endif // NOMIC_GRAPHIC_VAO_H_
