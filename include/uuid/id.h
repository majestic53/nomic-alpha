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

#ifndef NOMIC_UUID_ID_H_
#define NOMIC_UUID_ID_H_

#include "../define.h"

namespace nomic {

	namespace uuid {

		class id {

			public:

				id(void);

				id(
					__in const id &other
					);

				virtual ~id(void);

				id &operator=(
					__in const id &other
					);

				bool operator==(
					__in const id &other
					);

				bool operator!=(
					__in const id &other
					);

				uint32_t get_id(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				bool valid_id(void);

			protected:

				void decrement(void);

				void generate(void);

				void increment(void);

				uint32_t m_id;
		};
	}
}

#endif // NOMIC_UUID_ID_H_
