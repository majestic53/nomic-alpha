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

#include "../include/trace.h"
#include "./trace_type.h"

namespace nomic {

	enum {
		TRACE_FILE = 0,
		TRACE_FUNCTION,
		TRACE_LEVEL,
		TRACE_LINE,
		TRACE_MESSAGE,
		TRACE_TIMESTAMP,
	};

	static const std::string LEVEL_STR[] = {
		"Error", "Warning", "Information", "Verbose",
		};

	#define LEVEL_STRING(_TYPE_) \
		(((_TYPE_) > LEVEL_MAX) ? STRING_UNKNOWN : STRING_CHECK(LEVEL_STR[_TYPE_]))

#ifdef TRACE_COLOR
	static const std::string LEVEL_COLOR_STR[] = {
		"\x1b[91m", "\x1b[93m", "\x1b[94m", "\x1b[90m",
		};

	#define LEVEL_COLOR_STRING(_TYPE_) \
		(((_TYPE_) > LEVEL_MAX) ? LEVEL_COLOR_STR[LEVEL_VERBOSE] : LEVEL_COLOR_STR[_TYPE_])
#endif // TRACE_COLOR

	trace::trace(void)
	{
		return;
	}

	trace::~trace(void)
	{
		return;
	}

	trace_t 
	trace::dequeue(void)
	{
		trace_t result;

		std::lock_guard<std::recursive_mutex> lock(m_mutex);

		if(m_queue.empty()) {
			THROW_NOMIC_TRACE_EXCEPTION(NOMIC_TRACE_EXCEPTION_EMPTY);
		}

		result = m_queue.front();
		m_queue.pop();

		return result;
	}

	void 
	trace::enqueue(
		__in const trace_t &entry
		)
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);

		while(m_queue.size() >= QUEUE_MAX) {
			m_queue.pop();
		}

		m_queue.push(entry);
	}

	void 
	trace::flush(void)
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);

		while(!m_queue.empty()) {

			if(nomic::core::thread::active()) {
				format(dequeue());
			} else {
				dequeue();
			}
		}
	}

	void 
	trace::format(
		__in const trace_t &entry
		)
	{
		uint32_t level;
		std::string buffer;
		std::stringstream result;

		level = std::get<TRACE_LEVEL>(entry);
#ifdef TRACE_COLOR
		result << LEVEL_COLOR_STRING(level);
#endif // TRACE_COLOR

		result << "[" << nomic::utility::timestamp_as_string(std::get<TRACE_TIMESTAMP>(entry)) << "] {" << LEVEL_STRING(level) << "}";

		buffer = std::get<TRACE_MESSAGE>(entry);
		if(!buffer.empty()) {
			result << " " << buffer;
		}

		result << " (";

		buffer = std::get<TRACE_FUNCTION>(entry);
		if(!buffer.empty()) {
			result << buffer << ":";
		}

		buffer = std::get<TRACE_FILE>(entry);
		if(!buffer.empty()) {
			result << buffer << "@";
		}

		result << std::get<TRACE_LINE>(entry) << ")";
#ifdef TRACE_COLOR
		result << LEVEL_COLOR_STRING(LEVEL_VERBOSE);
#endif // TRACE_COLOR

		switch(level) {
			case LEVEL_ERROR:
			case LEVEL_WARNING:
				std::cerr << result.str() << std::endl;
				break;
			default:
				std::cout << result.str() << std::endl;
				break;
		}
	}

	void 
	trace::generate(
		__in uint32_t level,
		__in const std::string &message,
		__in const std::string &file,
		__in const std::string &function,
		__in size_t line,
		__in const std::string &format
		)
	{
		std::stringstream result;

		std::lock_guard<std::recursive_mutex> lock(m_mutex);

		result << message;

		if(!format.empty()) {

			if(!result.str().empty()) {
				result << ": ";
			}

			result << format;
		}

		enqueue(std::make_tuple(file, function, level, line, result.str(), nomic::utility::timestamp()));
		nomic::core::thread::notify();
	}

	bool 
	trace::on_initialize(void)
	{
		nomic::core::thread::start(true);

		return true;
	}

	bool 
	trace::on_run(void)
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);

		while(!m_queue.empty()) {
			format(dequeue());
		}

		return true;
	}

	void 
	trace::on_uninitialize(void)
	{

		if(nomic::core::thread::active()) {
			flush();
			nomic::core::thread::stop();
		}
	}

	std::string 
	trace::to_string(
		__in_opt bool verbose
		) const
	{
		std::stringstream result;

		result << NOMIC_TRACE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

		if(verbose) {
			result << " Base=" << SINGLETON_CLASS(nomic::trace)::to_string(verbose)
				<< ", Thread=" << nomic::core::thread::to_string(verbose);

			if(m_initialized) {
				result << ", Queue[" << m_queue.size() << "]";
			}
		}

		return result.str();
	}
}
