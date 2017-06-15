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

#ifndef NOMIC_CORE_FONT_H_
#define NOMIC_CORE_FONT_H_

#include "../graphic/character.h"
#include "./id.h"

namespace nomic {

	namespace core {

		class font :
				public nomic::core::id {

			public:

				explicit font(
					__in_opt FT_Library parent = nullptr,
					__in_opt const std::string &path = std::string(),
					__in_opt uint32_t size = FONT_SIZE_DEFAULT
					);

				font(
					__in const font &other
					);

				virtual ~font(void);

				font &operator=(
					__in const font &other
					);

				void generate(
					__inout nomic::graphic::character &output,
					__in char value
					);

				void load(
					__in FT_Library parent,
					__in const std::string &path,
					__in_opt uint32_t size = FONT_SIZE_DEFAULT
					);

				bool loaded(void) const;

				uint32_t size(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unload(void);

			protected:

				FT_Face m_face;

				uint32_t m_size;
		};		
	}
}

#endif // NOMIC_CORE_FONT_H_
