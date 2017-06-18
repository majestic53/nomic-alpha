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

#ifndef NOMIC_GRAPHIC_TEXTURE_H_
#define NOMIC_GRAPHIC_TEXTURE_H_

#include "../core/primitive.h"

namespace nomic {

	namespace graphic {

		class texture :
				public nomic::core::primitive {

			public:

				explicit texture(
					__in_opt const std::string &path = std::string(),
					__in_opt GLenum wrap_s = TEXTURE_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = TEXTURE_WRAP_T_DEFAULT,
					__in_opt GLenum filter_min = TEXTURE_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = TEXTURE_FILTER_MAG_DEFAULT
					);

				texture(
					__in const texture &other
					);

				virtual ~texture(void);

				texture &operator=(
					__in const texture &other
					);

				void bind(void);

				uint8_t depth(void) const;

				glm::uvec2 dimensions(void) const;

				void disable(void);

				void enable(void);

				uint32_t height(void) const;

				GLenum mode(void) const;

				void set(
					__in const std::string &path,
					__in_opt GLenum wrap_s = TEXTURE_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = TEXTURE_WRAP_T_DEFAULT,
					__in_opt GLenum filter_min = TEXTURE_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = TEXTURE_FILTER_MAG_DEFAULT
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				uint32_t width(void) const;

			protected:

				uint8_t m_depth;

				glm::uvec2 m_dimensions;

				GLenum m_mode;
		};
	}
}

#endif // NOMIC_GRAPHIC_TEXTURE_H_
