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

#ifndef NOMIC_ENTITY_MESSAGE_H_
#define NOMIC_ENTITY_MESSAGE_H_

#include "./string.h"

namespace nomic {

	namespace entity {

		class message :
				public nomic::entity::string {

			public:

				explicit message(
					__in_opt const std::string &path = STRING_FONT_DEFAULT,
					__in_opt uint32_t size = STRING_FONT_SIZE_DEFAULT,
					__in_opt const glm::vec4 &color = STRING_COLOR_DEFAULT,
					__in_opt float scale = STRING_SCALE_DEFAULT
					);

				message(
					__in const message &other
					);

				virtual ~message(void);

				message &operator=(
					__in const message &other
					);

				void on_update(
					__in void *runtime,
					__in void *camera
					);

				std::string &text(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				bool m_changed;
		};
	}
}

#endif // NOMIC_ENTITY_MESSAGE_H_
