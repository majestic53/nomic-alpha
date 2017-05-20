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

#ifndef NOMIC_CORE_THREAD_H_
#define NOMIC_CORE_THREAD_H_

#include <thread>
#include "../exception.h"
#include "./signal.h"

namespace nomic {

	namespace core {

		class thread {

			public:

				thread(void);

				virtual ~thread(void);

				bool active(void);

				bool notifiable(void);

				void notify(void);

				void start(
					__in bool notifiable,
					__in_opt bool detach = false
					);

				void stop(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void wait(void);

			protected:

				thread(
					__in const thread &other
					) = delete;

				thread &operator=(
					__in const thread &other
					) = delete;

				virtual bool on_run(void);

				virtual bool on_start(void);

				virtual void on_stop(void);

				void run(void);

				void set_active(
					__in bool active
					);

				bool m_active;

				nomic::exception m_exception;

				std::mutex m_mutex_thread;

				std::mutex m_mutex_thread_active;

				bool m_notifiable;

				nomic::core::signal m_signal_start;

				nomic::core::signal m_signal_stop;

				nomic::core::signal m_signal_wait;

				std::thread m_thread;
		};
	}
}

#endif // NOMIC_CORE_THREAD_H_
