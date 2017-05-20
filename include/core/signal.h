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

#ifndef NOMIC_CORE_SIGNAL_H_
#define NOMIC_CORE_SIGNAL_H_

#include <condition_variable>
#include "../define.h"

namespace nomic {

	namespace core {

		class signal {

			public:

				signal(void);

				virtual ~signal(void);

				bool abandoned(void) const;

				void notify(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				bool wait(
					__in uint32_t timeout = TIMEOUT_UNDEFINED
					);

			protected:

				signal(
					__in const signal &other
					) = delete;

				signal &operator=(
					__in const signal &other
					) = delete;

				void abandon(void);

				std::condition_variable m_condition;

				std::mutex m_mutex_signal;

				uint32_t m_state;
		};
	}
}

#endif // NOMIC_CORE_SIGNAL_H_
