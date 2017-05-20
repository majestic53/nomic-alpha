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

#ifndef NOMIC_TRACE_H_
#define NOMIC_TRACE_H_

#include <queue>
#include <tuple>
#include "./core/singleton.h"
#include "./core/thread.h"

namespace nomic {

	enum {
		LEVEL_ERROR = 0,
		LEVEL_WARNING,
		LEVEL_INFORMATION,
		LEVEL_VERBOSE,
	};

	#define LEVEL_MAX LEVEL_VERBOSE

	typedef std::tuple<std::string, std::string, uint32_t, size_t, std::string, time_t> trace_t;

	class trace :
			public SINGLETON_CLASS(nomic::trace),
			protected nomic::core::thread {

		public:

			~trace(void);

			void flush(void);

			void generate(
				__in uint32_t level,
				__in const std::string &message,
				__in const std::string &file,
				__in const std::string &function,
				__in size_t line,
				__in const std::string &format
				);

			std::string to_string(
				__in_opt bool verbose = false
				) const;

		protected:

			SINGLETON_CLASS_BASE(nomic::trace);

			trace(void);

			trace(
				__in const trace &other
				) = delete;

			trace &operator=(
				__in const trace &other
				) = delete;

			trace_t dequeue(void);

			void enqueue(
				__in const trace_t &entry
				);

			void format(
				__in const trace_t &entry
				);

			bool on_initialize(void);

			bool on_run(void);

			void on_stop(void);

			void on_uninitialize(void);

			std::recursive_mutex m_mutex;

			std::queue<trace_t> m_queue;
	};
}

#endif // NOMIC_TRACE_H_
