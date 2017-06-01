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

#include "../../include/event/input.h"
#include "../../include/trace.h"
#include "./input_type.h"

namespace nomic {

	namespace event {

		input::input(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::event::queue::register_for_event(EVENT_INPUT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		input::~input(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		input::flush_input(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::event::queue::flush_events();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		input::on_button(
			__in uint8_t button,
			__in uint8_t state,
			__in uint8_t clicks,
			__in int32_t x,
			__in int32_t y
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Button=%x, State=%x(%s), Clicks=%u, Position={%i, %i}", button, state,
				(state == SDL_PRESSED) ? "Press" : "Release", (uint16_t) clicks, x, y);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		input::on_key(
			__in uint16_t scancode,
			__in uint16_t modifier,
			__in uint8_t state
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Scancode=%x, Modifier=%x, State=%x(%s)", scancode, modifier, state,
				(state == SDL_PRESSED) ? "Press" : "Release");
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		input::on_motion(
			__in uint32_t state,
			__in int32_t x,
			__in int32_t y,
			__in int32_t x_relative,
			__in int32_t y_relative
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "State=%x, Position={%i, %i}, Relative={%i, %i}", state, x, y, x_relative, y_relative);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		input::on_wheel(
			__in uint32_t direction,
			__in int32_t x,
			__in int32_t y
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Direction=%x(%s), Position={%i, %i}", direction,
				(direction == SDL_MOUSEWHEEL_NORMAL) ? "Normal" : "Flipped", x, y);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		input::poll_input(void)
		{
			nomic::core::event event;

			TRACE_ENTRY(LEVEL_VERBOSE);

			while(nomic::event::queue::poll_event(event)) {

				if(event.type() != EVENT_INPUT) {
					continue;
				}

				switch(event.subtype()) {
					case INPUT_BUTTON: {

							SDL_MouseButtonEvent *data = (SDL_MouseButtonEvent *) &event.data()[0];
							if(data) {
								on_button(data->button, data->state, data->clicks, data->x, data->y);
							}
						} break;
					case INPUT_KEY: {

							SDL_KeyboardEvent *data = (SDL_KeyboardEvent *) &event.data()[0];
							if(data) {
								on_key(data->keysym.scancode, data->keysym.mod, data->state);
							}
						} break;
					case INPUT_MOTION: {

							SDL_MouseMotionEvent *data = (SDL_MouseMotionEvent *) &event.data()[0];
							if(data) {
								on_motion(data->state, data->x, data->y, data->xrel, data->yrel);
							}
						} break;
					case INPUT_WHEEL: {

							SDL_MouseWheelEvent *data = (SDL_MouseWheelEvent *) &event.data()[0];
							if(data) {
								on_wheel(data->direction, data->x, data->y);
							}
						} break;
					default:
						break;
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		input::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_EVENT_INPUT_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::event::queue::to_string(true);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
