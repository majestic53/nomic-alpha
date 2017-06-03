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

#ifndef NOMIC_GRAPHIC_DISPLAY_H_
#define NOMIC_GRAPHIC_DISPLAY_H_

#include "../core/singleton.h"

namespace nomic {

	namespace graphic {

		class display :
				public SINGLETON_CLASS(nomic::graphic::display) {

			public:

				~display(void);

				void clear(void);

				void set_dimensions(
					__in const glm::uvec2 &dimension
					);

				void set_fullscreen(
					__in bool fullscreen
					);

				void set_title(
					__in const std::string &title
					);

				void show(void);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				SINGLETON_CLASS_BASE(nomic::graphic::display);

				display(void);

				display(
					__in const display &other
					) = delete;

				display &operator=(
					__in const display &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				SDL_Window *m_window;

				SDL_GLContext m_window_context;
		};
	}
}

#endif // NOMIC_GRAPHIC_DISPLAY_H_
