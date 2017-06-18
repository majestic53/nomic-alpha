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

#ifndef NOMIC_GRAPHIC_CHARACTER_H_
#define NOMIC_GRAPHIC_CHARACTER_H_

#include "../core/primitive.h"

namespace nomic {

	namespace graphic {

		class character :
				public nomic::core::primitive {

			public:

				character(void);

				character(
					__in FT_Face face,
					__in_opt GLenum wrap_s = CHARACTER_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = CHARACTER_WRAP_T_DEFAULT,
					__in_opt GLenum filter_min = CHARACTER_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = CHARACTER_FILTER_MAG_DEFAULT
					);

				character(
					__in const character &other
					);

				virtual ~character(void);

				character &operator=(
					__in const character &other
					);

				uint32_t advance(void) const;

				glm::ivec2 bearing(void) const;

				void bind(void);

				glm::ivec2 dimensions(void) const;

				void disable(void);

				void enable(void);

				int32_t height(void) const;

				int32_t left(void) const;

				void set(
					__in FT_Face face,
					__in_opt GLenum wrap_s = CHARACTER_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = CHARACTER_WRAP_T_DEFAULT,
					__in_opt GLenum filter_min = CHARACTER_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = CHARACTER_FILTER_MAG_DEFAULT
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				int32_t top(void) const;

				int32_t width(void) const;

			protected:

				uint32_t m_advance;

				glm::ivec2 m_bearing;

				glm::ivec2 m_dimensions;
		};	
	}
}

#endif // NOMIC_GRAPHIC_CHARACTER_H_
