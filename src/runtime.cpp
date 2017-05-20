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
		m_manager_trace(nomic::trace::acquire()),
		m_manager_uuid(nomic::uuid::manager::acquire())
		// TODO
	{
		m_manager_trace.initialize();

		TRACE_ENTRY(LEVEL_VERBOSE);
		TRACE_EXIT(LEVEL_VERBOSE);
	}

	runtime::~runtime(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		m_manager_uuid.release();

		TRACE_EXIT(LEVEL_VERBOSE);

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

	bool 
	runtime::on_initialize(void)
	{
		bool result = true;

		TRACE_ENTRY(LEVEL_VERBOSE);

		m_manager_uuid.initialize();

		// TODO

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	bool 
	runtime::on_run(void)
	{
		bool result = true;

		TRACE_ENTRY(LEVEL_VERBOSE);

		while(nomic::core::thread::active()) {

			// TODO

		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	bool 
	runtime::on_start(void)
	{
		bool result = true;

		TRACE_ENTRY(LEVEL_VERBOSE);

		// TODO

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	void 
	runtime::on_stop(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		// TODO

		TRACE_EXIT(LEVEL_VERBOSE);
	}

	void 
	runtime::on_uninitialize(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		if(nomic::core::thread::active()) {
			nomic::core::thread::stop();
		}

		// TODO

		m_manager_uuid.uninitialize();

		TRACE_EXIT(LEVEL_VERBOSE);
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
	runtime::terminate(void)
	{
		TRACE_ENTRY(LEVEL_VERBOSE);

		std::lock_guard<std::mutex> lock(m_mutex);

		if(nomic::core::thread::active()) {
			nomic::core::thread::stop();
		}

		TRACE_EXIT(LEVEL_VERBOSE);
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
				<< ", Thread=" << nomic::core::thread::to_string(verbose);
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
