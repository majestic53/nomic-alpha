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

#ifndef NOMIC_EVENT_INPUT_H_
#define NOMIC_EVENT_INPUT_H_

#include "./queue.h"

namespace nomic {

	namespace event {

		class input :
				protected nomic::event::queue {

			public:

				input(void);

				virtual ~input(void);

				void flush_input(void);

				void poll_input(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				input(
					__in const input &other
					) = delete;

				input &operator=(
					__in const input &other
					) = delete;

				virtual void on_button(
					__in uint8_t button,
					__in uint8_t state,
					__in uint8_t clicks,
					__in int32_t x,
					__in int32_t y
					);

				virtual void on_key(
					__in uint16_t scancode,
					__in uint16_t modifier,
					__in uint8_t state
					);

				virtual void on_motion(
					__in uint32_t state,
					__in int32_t x,
					__in int32_t y,
					__in int32_t x_relative,
					__in int32_t y_relative
					);

				virtual void on_wheel(
					__in uint32_t direction,
					__in int32_t x,
					__in int32_t y
					);
		};
	}
}

#endif // NOMIC_EVENT_INPUT_H_
