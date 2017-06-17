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

#ifndef NOMIC_GRAPHIC_VBO_H_
#define NOMIC_GRAPHIC_VBO_H_

#include <vector>
#include "../core/primitive.h"

namespace nomic {

	namespace graphic {

		class vbo :
				public nomic::core::primitive {

			public:

				explicit vbo(
					__in GLenum target
					);

				vbo(
					__in GLenum target,
					__in uint32_t size,
					__in GLenum usage
					);

				vbo(
					__in GLenum target,
					__in const std::vector<uint8_t> &data,
					__in GLenum usage
					);

				vbo(
					__in const vbo &other
					);

				~vbo(void);

				vbo &operator=(
					__in const vbo &other
					);

				void bind(void) const;

				void set(
					__in uint32_t size,
					__in GLenum usage
					);

				void set(
					__in const std::vector<uint8_t> &data,
					__in GLenum usage
					);

				void set_subdata(
					__in GLintptr offset,
					__in GLsizeiptr size,
					__in const GLvoid *data
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				std::vector<uint8_t> m_data;

				GLenum m_usage;
		};
	}
}

#endif // NOMIC_GRAPHIC_VBO_H_
