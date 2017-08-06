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

#ifndef NOMIC_GRAPHIC_FBO_H_
#define NOMIC_GRAPHIC_FBO_H_

#include "./texture.h"

namespace nomic {

	namespace graphic {

		class fbo :
				public nomic::core::primitive {

			public:

				fbo(void);

				fbo(
					__in GLenum target,
					__in GLint level,
					__in GLint internal,
					__in GLsizei width,
					__in GLsizei height,
					__in GLint border,
					__in GLenum format,
					__in GLenum type,
					__in const GLvoid *data,
					__in GLenum attachment,
					__in_opt GLenum wrap_s = TEXTURE_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = TEXTURE_WRAP_T_DEFAULT,
					__in_opt GLenum filter_min = TEXTURE_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = TEXTURE_FILTER_MAG_DEFAULT
					);

				fbo(
					__in const fbo &other
					);

				virtual ~fbo(void);

				fbo &operator=(
					__in const fbo &other
					);

				void bind(void);

				void set(
					__in GLenum target,
					__in GLint level,
					__in GLint internal,
					__in GLsizei width,
					__in GLsizei height,
					__in GLint border,
					__in GLenum format,
					__in GLenum type,
					__in const GLvoid *data,
					__in GLenum attachment,
					__in_opt GLenum wrap_s = TEXTURE_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = TEXTURE_WRAP_T_DEFAULT,
					__in_opt GLenum filter_min = TEXTURE_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = TEXTURE_FILTER_MAG_DEFAULT
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				nomic::graphic::texture m_texture;
		};
	}
}

#endif // NOMIC_GRAPHIC_FBO_H_
