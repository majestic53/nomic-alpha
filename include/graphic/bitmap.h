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

#ifndef NOMIC_GRAPHIC_BITMAP_H_
#define NOMIC_GRAPHIC_BITMAP_H_

#include "../define.h"

namespace nomic {

	namespace graphic {

		class bitmap {

			public:

				explicit bitmap(
					__in_opt const std::string &path = std::string()
					);

				bitmap(
					__in const bitmap &other
					);

				virtual ~bitmap(void);

				bitmap &operator=(
					__in const bitmap &other
					);

				uint8_t depth(void) const;

				uint32_t height(void) const;

				bool loaded(void) const;

				std::string path(void) const;

				uint32_t pitch(void) const;

				uint32_t pixel(
					__in uint32_t x,
					__in uint32_t y
					) const;

				void *pixels(void) const;

				void set(
					__in const std::string &path
					);

				SDL_Surface *surface(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				uint32_t width(void) const;

			protected:

				void destroy(void);

				std::string m_path;

				SDL_Surface *m_surface;
		};
	}
}

#endif // NOMIC_GRAPHIC_BITMAP_H_
