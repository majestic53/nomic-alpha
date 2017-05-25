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

#ifndef NOMIC_CORE_EVENT_H_
#define NOMIC_CORE_EVENT_H_

#include <vector>
#include "./object.h"

namespace nomic {

	namespace core {

		class event :
				public nomic::core::object {

			public:

				event(
					__in uint32_t type,
					__in_opt uint32_t subtype = SUBTYPE_UNDEFINED,
					__in_opt const std::vector<uint8_t> &data = std::vector<uint8_t>()
					);

				event(
					__in const event &other
					);

				virtual ~event(void);

				event &operator=(
					__in const event &other
					);

				std::vector<uint8_t> &data(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				std::vector<uint8_t> m_data;
		};
	}
}

#endif // NOMIC_CORE_EVENT_H_
