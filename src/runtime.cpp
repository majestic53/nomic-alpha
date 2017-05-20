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
		m_manager_uuid(nomic::uuid::manager::acquire())
		// TODO
	{
		return;
	}

	runtime::~runtime(void)
	{

		// TODO

		m_manager_uuid.release();
	}

	bool 
	runtime::active(void)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		return nomic::core::thread::active();
	}

	bool 
	runtime::on_initialize(void)
	{
		m_manager_uuid.initialize();

		// TODO
		std::cout << __FUNCTION__ << std::endl;
		// ---

		return true;
	}

	bool 
	runtime::on_run(void)
	{

		while(nomic::core::thread::active()) {

			// TODO
			std::cout << __FUNCTION__ << std::endl;
			// ---
		}

		return true;
	}

	bool 
	runtime::on_start(void)
	{
		// TODO
		std::cout << __FUNCTION__ << std::endl;
		// ---

		return true;
	}

	void 
	runtime::on_stop(void)
	{
		// TODO
		std::cout << __FUNCTION__ << std::endl;
		// ---

		return;
	}

	void 
	runtime::on_uninitialize(void)
	{

		if(nomic::core::thread::active()) {
			nomic::core::thread::stop();
		}

		// TODO
		std::cout << __FUNCTION__ << std::endl;
		// ---

		m_manager_uuid.uninitialize();
	}

	void 
	runtime::run(void)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if(nomic::core::thread::active()) {
			THROW_NOMIC_RUNTIME_EXCEPTION(NOMIC_RUNTIME_EXCEPTION_STARTED);
		}

		nomic::core::thread::start(false);
	}

	void 
	runtime::terminate(void)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if(nomic::core::thread::active()) {
			nomic::core::thread::stop();
		}
	}

	std::string 
	runtime::to_string(
		__in_opt bool verbose
		) const
	{
		std::stringstream result;

		result << NOMIC_RUNTIME_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

		if(verbose) {
			result << " Base=" << SINGLETON_CLASS(nomic::runtime)::to_string(verbose)
				<< ", Thread=" << nomic::core::thread::to_string(verbose);
		}

		return result.str();
	}

	std::string 
	runtime::version(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << NOMIC_VERSION_MAJOR << "." << NOMIC_VERSION_MINOR << "." << NOMIC_VERSION_WEEK << "." << NOMIC_VERSION_REVISION;

		if(verbose) {
			result << "-" << NOMIC_VERSION_RELEASE;
		}

		return result.str();
	}

	void 
	runtime::wait(void)
	{

		if(nomic::core::thread::active()) {
			nomic::core::thread::wait();
		}
	}
}
