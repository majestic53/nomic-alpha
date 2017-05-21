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

#include "../include/runtime.h"
#include "./runtime_type.h"

namespace nomic {

	runtime::runtime(void) :
		m_frame_rate(0),
		m_paused(false),
		m_paused_change(false),
		m_manager_trace(nomic::trace::acquire()),
		m_manager_uuid(nomic::uuid::manager::acquire()),
		// TODO: acquire managers
		m_tick(0)
	{
		m_manager_trace.initialize();

		TRACE_ENTRY(LEVEL_VERBOSE);
		TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime starting");
		TRACE_EXIT(LEVEL_VERBOSE);
	}

	runtime::~runtime(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		// TODO: release managers

		m_manager_uuid.release();

		TRACE_EXIT(LEVEL_VERBOSE);

		TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime terminating");
		m_manager_trace.uninitialize();
		m_manager_trace.release();
	}

	bool 
	runtime::active(void)
	{
		bool result;

		TRACE_ENTRY(LEVEL_VERBOSE);

		std::lock_guard<std::mutex> lock(m_mutex);

		result = nomic::core::thread::active();

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	uint32_t 
	runtime::frame_rate(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		std::lock_guard<std::mutex> lock(m_mutex);

		if(!nomic::core::thread::active()) {
			THROW_NOMIC_RUNTIME_EXCEPTION(NOMIC_RUNTIME_EXCEPTION_STOPPED);
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", m_frame_rate);
		return m_frame_rate;
	}

	void 
	runtime::increment_tick(void)
	{
		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Tick=(%u->%u)", m_tick, m_tick + 1);

		std::lock_guard<std::mutex> lock(m_mutex);

		++m_tick;

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	bool 
	runtime::on_event(void)
	{
		SDL_Event event;
		bool result = true;

		TRACE_ENTRY(LEVEL_VERBOSE);

		if(!m_paused) {

			while(SDL_PollEvent(&event) && result) {

				switch(event.type) {
					case SDL_KEYDOWN:
					case SDL_KEYUP:

						if(!event.key.repeat) {
							TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Encountered key event=(%s, %s, Code=%x)",
								(event.key.type == SDL_KEYDOWN) ? "Down" : "Up",
								(event.key.state == SDL_PRESSED) ? "Press" : "Release",
								event.key.keysym.scancode);

							// TODO: handle keyboard input
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
						TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE,
							"Encountered mouse button event=(%s, %s, Code=%x, Position={%i, %i}, Click=%u)",
							(event.button.type == SDL_KEYDOWN) ? "Down" : "Up",
							(event.button.state == SDL_PRESSED) ? "Press" : "Release",
							event.button.button, event.button.x, event.button.y, event.button.clicks);

						// TODO: handle mouse button input

						break;
					case SDL_MOUSEMOTION:
						TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE,
							"Encountered mouse motion event=(State=%x, Position={%i, %i}, Relative={%i, %i})",
							event.motion.state, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);

						// TODO: handle mouse motion input

						break;
					case SDL_MOUSEWHEEL:
						TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Encountered mouse wheel event=(%s, Position={%i, %i})",
							(event.wheel.direction == SDL_MOUSEWHEEL_NORMAL) ? "Normal" : "Flip",
							event.wheel.x, event.wheel.y);

						// TODO: handle mouse wheel input

						break;
					case SDL_QUIT:
						TRACE_MESSAGE(LEVEL_INFORMATION, "Encountered quit event");
						result = false;
						break;
					case SDL_WINDOWEVENT:
						TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Encountered window event=%x", event.window.event);

						switch(event.window.event) {
							case SDL_WINDOWEVENT_FOCUS_LOST:
								TRACE_MESSAGE(LEVEL_INFORMATION, "Window lost focus");
								set_pause(true);
								break;
						}
						break;
					default:
						break;
				}
			}
		} else {

			while(SDL_PollEvent(&event) && result) {

				switch(event.type) {
					case SDL_QUIT:
						TRACE_MESSAGE(LEVEL_INFORMATION, "Encountered quit event");
						result = false;
						break;
					case SDL_WINDOWEVENT:
						TRACE_MESSAGE_FORMAT(LEVEL_VERBOSE, "Encountered window event=%x", event.window.event);

						switch(event.window.event) {
							case SDL_WINDOWEVENT_FOCUS_GAINED:
								TRACE_MESSAGE(LEVEL_INFORMATION, "Window gained focus");
								set_pause(false);
								break;
						}
						break;
					default:
						break;
				}
			}
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	bool 
	runtime::on_initialize(void)
	{
		bool result = true;

		TRACE_ENTRY(LEVEL_VERBOSE);

		m_manager_uuid.initialize();

		// TODO: initialize managers

		TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime initialized");

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	bool 
	runtime::on_parse(
		__in int argc,
		__in const char *argv[]
		)
	{
		bool result = true;

		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Argv[%u]=%p", argc, argv);

		// TODO: parse input arguments

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	void 
	runtime::on_pause(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		// TODO: handle pause event

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	void 
	runtime::on_render(
		__in float delta
		)
	{
		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f", delta);

		// TODO: handle render event

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	bool 
	runtime::on_run(void)
	{
		bool result = false;
		uint32_t duration, frame, next, skip;

		TRACE_ENTRY(LEVEL_VERBOSE);

		TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime loop entered");

		duration = (MILLISECONDS_PER_SECOND / RUNTIME_TICKS_PER_SECOND);
		frame = 0;
		next = SDL_GetTicks();
		TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Ticks/second=%u (%u ms)", RUNTIME_TICKS_PER_SECOND, duration);

		while(nomic::core::thread::active()) {

			if(!on_event()) {
				break;
			}

			if(!m_paused) {

				if(m_paused_change) {
					on_unpause();
					m_paused_change = false;
					next = SDL_GetTicks();
				}

				skip = 0;

				while((next < SDL_GetTicks()) && (skip < RUNTIME_TICK_SKIP)) {

					if(!(m_tick % RUNTIME_TICKS_PER_SECOND)) {
						set_frame_rate(frame);
						TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Runtime frames/sec=%u", frame);
						frame = 0;
					}

					on_update();
					next += duration;
					++skip;
					increment_tick();
				}

				on_render((SDL_GetTicks() + duration - next) / (float) duration);
				++frame;
			} else {

				if(m_paused_change) {
					on_pause();
					m_paused_change = false;
				}

				SDL_Delay(RUNTIME_PAUSE_DELAY);
				next = SDL_GetTicks();
			}
		}

		TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime loop exited");

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	bool 
	runtime::on_start(void)
	{
		bool result = true;

		TRACE_ENTRY(LEVEL_VERBOSE);

		if(SDL_Init(SDL_FLAGS_INIT)) {
			THROW_NOMIC_RUNTIME_EXCEPTION_FORMAT(NOMIC_RUNTIME_EXCEPTION_EXTERNAL, "%s", SDL_GetError());
		}

		TRACE_MESSAGE(LEVEL_INFORMATION, "SDL runtime initialized");

		// TODO: initialize gl managers

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	void 
	runtime::on_stop(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		// TODO: uninitialize gl managers

		SDL_Quit();
		TRACE_MESSAGE(LEVEL_INFORMATION, "SDL runtime uninitialized");

		// TODO: reset runtime member variables
		m_frame_rate = 0;
		m_paused = false;
		m_paused_change = false;
		m_tick = 0;
		// ---

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	void 
	runtime::on_uninitialize(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		if(nomic::core::thread::active()) {
			nomic::core::thread::stop();
		}

		// TODO: uninitialize managers

		m_manager_uuid.uninitialize();
		TRACE_MESSAGE(LEVEL_INFORMATION, "Runtime uninitialized");

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	void 
	runtime::on_unpause(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		// TODO: handle unpause event

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	void 
	runtime::on_update(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		// TODO: handle update event

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	bool 
	runtime::paused(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		std::lock_guard<std::mutex> lock(m_mutex);

		if(!nomic::core::thread::active()) {
			THROW_NOMIC_RUNTIME_EXCEPTION(NOMIC_RUNTIME_EXCEPTION_STOPPED);
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_paused);
		return m_paused;
	}

	void 
	runtime::run(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		std::lock_guard<std::mutex> lock(m_mutex);

		if(nomic::core::thread::active()) {
			THROW_NOMIC_RUNTIME_EXCEPTION(NOMIC_RUNTIME_EXCEPTION_STARTED);
		}

		nomic::core::thread::start(false);

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	void 
	runtime::run(
		__in int argc,
		__in const char *argv[]
		)
	{
		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Argv[%u]=%p", argc, argv);

		std::lock_guard<std::mutex> lock(m_mutex);

		if(nomic::core::thread::active()) {
			THROW_NOMIC_RUNTIME_EXCEPTION(NOMIC_RUNTIME_EXCEPTION_STARTED);
		}

		if(!on_parse(argc, argv)) {
			THROW_NOMIC_RUNTIME_EXCEPTION(NOMIC_RUNTIME_EXCEPTION_START);
		}

		nomic::core::thread::start(false);

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	void 
	runtime::set_frame_rate(
		__in uint32_t frame_rate
		)
	{
		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Frame rate=%u", frame_rate);

		std::lock_guard<std::mutex> lock(m_mutex);

		m_frame_rate = frame_rate;

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	void 
	runtime::set_pause(
		__in bool paused
		)
	{
		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Paused=%x", paused);

		std::lock_guard<std::mutex> lock(m_mutex);

		m_paused = paused;
		m_paused_change = true;

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	void 
	runtime::terminate(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		std::lock_guard<std::mutex> lock(m_mutex);

		if(nomic::core::thread::active()) {
			nomic::core::thread::stop();
		}

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	uint32_t 
	runtime::tick(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		std::lock_guard<std::mutex> lock(m_mutex);

		if(!nomic::core::thread::active()) {
			THROW_NOMIC_RUNTIME_EXCEPTION(NOMIC_RUNTIME_EXCEPTION_STOPPED);
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", m_tick);
		return m_tick;
	}

	std::string 
	runtime::to_string(
		__in_opt bool verbose
		) const
	{
		std::stringstream result;

		TRACE_ENTRY(LEVEL_VERBOSE);

		result << NOMIC_RUNTIME_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

		if(verbose) {
			result << " Base=" << SINGLETON_CLASS(nomic::runtime)::to_string(verbose)
				<< ", Thread=" << nomic::core::thread::to_string(verbose)
				<< ", State=" << (m_paused ? "Paused" : "Running");
		}

		TRACE_EXIT(LEVEL_VERBOSE);
		return result.str();
	}

	std::string 
	runtime::version(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

		result << NOMIC_VERSION_MAJOR << "." << NOMIC_VERSION_MINOR << "." << NOMIC_VERSION_WEEK << "." << NOMIC_VERSION_REVISION;

		if(verbose) {
			result << "-" << NOMIC_VERSION_RELEASE;
		}

		TRACE_EXIT(LEVEL_VERBOSE);
		return result.str();
	}

	void 
	runtime::wait(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		if(nomic::core::thread::active()) {
			nomic::core::thread::wait();
		}

		TRACE_EXIT(LEVEL_VERBOSE);
	}
}
