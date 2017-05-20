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

#ifndef NOMIC_RUNTIME_H_
#define NOMIC_RUNTIME_H_

#include "./uuid/manager.h"
#include "./trace.h"
#include "./exception.h"

namespace nomic {

	class runtime :
			public SINGLETON_CLASS(nomic::runtime),
			protected nomic::core::thread {

		public:

			~runtime(void);

			void run(void);

			bool active(void);

			void terminate(void);

			std::string to_string(
				__in_opt bool verbose = false
				) const;

			static std::string version(
				__in_opt bool verbose = false
				);

			void wait(void);

		protected:

			SINGLETON_CLASS_BASE(nomic::runtime);

			runtime(void);

			runtime(
				__in const runtime &other
				) = delete;

			runtime &operator=(
				__in const runtime &other
				) = delete;

			bool on_initialize(void);

			bool on_run(void);

			bool on_start(void);

			void on_stop(void);

			void on_uninitialize(void);

			nomic::trace &m_manager_trace;

			nomic::uuid::manager &m_manager_uuid;

			std::mutex m_mutex;
	};
}

#endif // NOMIC_RUNTIME_H_
