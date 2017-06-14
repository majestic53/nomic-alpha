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

#ifndef NOMIC_FONT_MANAGER_H_
#define NOMIC_FONT_MANAGER_H_

#include <map>
#include "../core/font.h"
#include "../core/singleton.h"

namespace nomic {

	namespace font {

		class manager :
				public SINGLETON_CLASS(nomic::font::manager) {

			public:

				~manager(void);

				bool font_loaded(
					__in uint32_t id
					) const;

				uint32_t load_font(
					__in const std::string &path,
					__in uint32_t size
					);

				void unload_all_fonts(void);

				void unload_font(
					__in uint32_t id
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				SINGLETON_CLASS_BASE(nomic::font::manager);

				manager(void);

				manager(
					__in const manager &other
					) = delete;

				manager &operator=(
					__in const manager &other
					) = delete;

				std::map<uint32_t, std::pair<nomic::core::font, std::map<GLchar, nomic::graphic::character>>>::iterator find_font(
					__in uint32_t id
					);

				std::map<GLchar, nomic::graphic::character>::iterator find_character(
					__in std::map<uint32_t, std::pair<nomic::core::font, std::map<GLchar, nomic::graphic::character>>>::iterator iter,
					__in GLchar ch
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				std::map<uint32_t, std::pair<nomic::core::font, std::map<GLchar, nomic::graphic::character>>> m_font;

				FT_Library m_handle;
		};
	}
}

#endif // NOMIC_FONT_MANAGER_H_
