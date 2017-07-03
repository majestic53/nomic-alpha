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

#include "./event/manager.h"
#include "./session/manager.h"
#include "./uuid/manager.h"
#include "./trace.h"
#include "./exception.h"

namespace nomic {

	class runtime :
			public SINGLETON_CLASS(nomic::runtime),
			protected nomic::core::thread {

		public:

			~runtime(void);

			bool active(void);

			uint32_t frame_rate(void);

			bool paused(void);

			void run(void);

			void run(
				__in int argc,
				__in const char *argv[]
				);

			uint32_t seed(void);

			void terminate(void);

			uint32_t tick(void);

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

			void increment_tick(void);

			bool on_event(void);

			bool on_initialize(void);

			bool on_parse(
				__in int argc,
				__in const char *argv[]
				);

			void on_pause(void);

			void on_render(
				__in float delta
				);

			bool on_run(void);

			bool on_start(void);

			void on_stop(void);

			void on_uninitialize(void);

			void on_unpause(void);

			void on_update(void);

			void set_frame_rate(
				__in uint32_t frame_rate
				);

			void set_pause(
				__in bool paused
				);

			uint32_t m_frame_rate;

			bool m_paused;

			bool m_paused_change;

			nomic::event::manager &m_manager_event;

			nomic::session::manager &m_manager_session;

			nomic::trace &m_manager_trace;

			nomic::uuid::manager &m_manager_uuid;

			std::mutex m_mutex;

			uint32_t m_tick;
	};
}

#endif // NOMIC_RUNTIME_H_
