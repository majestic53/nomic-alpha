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

#ifndef NOMIC_SESSION_MANAGER_H_
#define NOMIC_SESSION_MANAGER_H_

#include "../core/singleton.h"

namespace nomic {

	namespace session {

		class manager :
				public SINGLETON_CLASS(nomic::session::manager) {

			public:

				~manager(void);

				void render(
					__in float delta
					);

				void pause(void);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unpause(void);

				void update(void);

			protected:

				SINGLETON_CLASS_BASE(nomic::session::manager);

				manager(void);

				manager(
					__in const manager &other
					) = delete;

				manager &operator=(
					__in const manager &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);
		};
	}
}

#endif // NOMIC_SESSION_MANAGER_H_
