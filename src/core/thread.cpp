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

#include "../../include/core/thread.h"
#include "./thread_type.h"

namespace nomic {

	namespace core {

		thread::thread(void) :
			m_active(false),
			m_notifiable(false)
		{
			return;
		}

		thread::~thread(void)
		{
			stop();
		}

		bool 
		thread::active(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex_thread_active);

			return m_active;
		}

		bool 
		thread::notifiable(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex_thread);

			if(!active()) {
				THROW_NOMIC_CORE_THREAD_EXCEPTION(NOMIC_CORE_THREAD_EXCEPTION_STOPPED);
			}

			return m_notifiable;
		}

		void 
		thread::notify(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex_thread);

			if(!active()) {
				THROW_NOMIC_CORE_THREAD_EXCEPTION(NOMIC_CORE_THREAD_EXCEPTION_STOPPED);
			}

			if(!m_notifiable) {
				THROW_NOMIC_CORE_THREAD_EXCEPTION(NOMIC_CORE_THREAD_EXCEPTION_INVALID);
			}

			m_signal_wait.notify();
		}

		bool 
		thread::on_run(void)
		{
			return true;
		}

		bool 
		thread::on_start(void)
		{
			return true;
		}

		void 
		thread::on_stop(void)
		{
			return;
		}

		void 
		thread::run(void)
		{
			m_signal_start.notify();

			try {

				for(;;) {

					if(m_notifiable) {

						if(!active()) {
							break;
						}

						m_signal_wait.wait();
					}

					if(!active()) {
						break;
					}

					if(!on_run()) {
						break;
					}
				}
			} catch(nomic::exception &exc) {
				m_exception = exc;
			} catch(std::exception &exc) {
				m_exception = nomic::exception(exc.what(), __FILE__, __FUNCTION__, __LINE__);
			}

			m_signal_stop.notify();
		}

		void 
		thread::set_active(
			__in bool active
			)
		{
			std::lock_guard<std::mutex> lock(m_mutex_thread_active);

			m_active = active;
		}

		void 
		thread::start(
			__in bool notifiable,
			__in_opt bool detach
			)
		{
			std::lock_guard<std::mutex> lock(m_mutex_thread);

			if(active()) {
				THROW_NOMIC_CORE_THREAD_EXCEPTION(NOMIC_CORE_THREAD_EXCEPTION_STARTED);
			}

			if(!on_start()) {
				THROW_NOMIC_CORE_THREAD_EXCEPTION(NOMIC_CORE_THREAD_EXCEPTION_START);
			}

			m_exception.clear();
			m_notifiable = notifiable;
			set_active(true);
			m_thread = std::thread(&thread::run, this);

			if(!m_signal_start.wait(THREAD_TIMEOUT)) {
				set_active(false);
				m_notifiable = false;
				THROW_NOMIC_CORE_THREAD_EXCEPTION_FORMAT(NOMIC_CORE_THREAD_EXCEPTION_TIMEOUT, "Duration=%u ms",
					THREAD_TIMEOUT);
			}

			if(detach && m_thread.joinable()) {
				m_thread.detach();
			}
		}

		void 
		thread::stop(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex_thread);

			if(active()) {
				set_active(false);

				if(m_notifiable) {
					m_signal_wait.notify();
				}

				if(!m_signal_stop.wait(THREAD_TIMEOUT)) {
					THROW_NOMIC_CORE_THREAD_EXCEPTION_FORMAT(NOMIC_CORE_THREAD_EXCEPTION_TIMEOUT, "Duration=%u ms",
						THREAD_TIMEOUT);
				}

				if(m_thread.joinable()) {
					m_thread.join();
				}

				m_notifiable = false;
				on_stop();

				if(!m_exception.empty()) {
					THROW_NOMIC_CORE_THREAD_EXCEPTION_FORMAT(NOMIC_CORE_THREAD_EXCEPTION_INTERNAL, "%s",
						STRING_CHECK(m_exception.to_string(true)));
				}
			}
		}

		std::string 
		thread::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			result << NOMIC_CORE_THREAD_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " State=" << (m_active ? "Active" : "Inactive");

				if(m_active) {
					result << ", Mode=" << (m_notifiable ? "Notifiable" : "Freerunning") << ", Id=" << m_thread.get_id();
				}
			}

			return result.str();
		}

		void 
		thread::wait(void)
		{

			if(!active()) {
				THROW_NOMIC_CORE_THREAD_EXCEPTION(NOMIC_CORE_THREAD_EXCEPTION_STOPPED);
			}

			m_signal_stop.wait();

			if(active()) {
				set_active(false);

				if(m_thread.joinable()) {
					m_thread.join();
				}

				m_notifiable = false;
				on_stop();

				if(!m_exception.empty()) {
					THROW_NOMIC_CORE_THREAD_EXCEPTION_FORMAT(NOMIC_CORE_THREAD_EXCEPTION_INTERNAL, "%s",
						STRING_CHECK(m_exception.to_string(true)));
				}
			}
		}
	}
}
