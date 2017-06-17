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

		typedef std::map<GLchar, nomic::graphic::character> character_set;

		typedef std::pair<nomic::core::font, nomic::font::character_set> context;

		class manager :
				public SINGLETON_CLASS(nomic::font::manager) {

			public:

				~manager(void);

				nomic::graphic::character &character(
					__in uint32_t id,
					__in GLchar value
					);

				uint32_t load(
					__in const std::string &path,
					__in uint32_t size
					);

				bool loaded(
					__in uint32_t id
					) const;

				void unload(
					__in uint32_t id
					);

				void unload_all(void);

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

				nomic::font::character_set::iterator find_character(
					__in std::map<uint32_t, nomic::font::context>::iterator iter,
					__in GLchar value
					);

				std::map<uint32_t, nomic::font::context>::iterator find_font(
					__in uint32_t id
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				std::map<uint32_t, nomic::font::context> m_font;

				FT_Library m_handle;
		};
	}
}

#endif // NOMIC_FONT_MANAGER_H_
