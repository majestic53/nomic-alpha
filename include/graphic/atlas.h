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

#ifndef NOMIC_GRAPHIC_ATLAS_H_
#define NOMIC_GRAPHIC_ATLAS_H_

#include <vector>
#include "./bitmap.h"
#include "./texture.h"

namespace nomic {

	namespace graphic {

		class atlas {

			public:

				atlas(
					__in_opt const std::string &path = std::string(),
					__in_opt const glm::uvec2 &dimensions = ATLAS_DIMENSIONS_DEFAULT,
					__in_opt uint8_t width = ATLAS_WIDTH_DEFAULT
					);

				atlas(
					__in const atlas &other
					);

				virtual ~atlas(void);

				atlas &operator=(
					__in const atlas &other
					);

				void disable(
					__in uint8_t type
					);

				void disable_all(void);

				void enable(
					__in uint8_t type
					);

				void load(
					__in const std::string &path,
					__in_opt const glm::uvec2 &dimensions = ATLAS_DIMENSIONS_DEFAULT,
					__in_opt uint8_t width = ATLAS_WIDTH_DEFAULT
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void destroy(void);

				nomic::graphic::bitmap m_bitmap;

				glm::uvec2 m_dimensions;

				std::vector<nomic::graphic::texture *> m_texture;

				uint8_t m_width;
		};
	}
}

#endif // NOMIC_GRAPHIC_ATLAS_H_
