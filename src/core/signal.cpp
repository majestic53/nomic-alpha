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

#include <chrono>
#include "../../include/core/signal.h"
#include "./signal_type.h"

namespace nomic {

	namespace core {

		enum {
			SIGNAL_ABANDON = 0,
			SIGNAL_CLEAR,
			SIGNAL_SET,
		};

		#define SIGNAL_MAX SIGNAL_SET

		static const std::string SIGNAL_STR[] = {
			"Abandon", "Clear", "Set",
			};

		#define SIGNAL_STRING(_TYPE_) \
			(((_TYPE_) > SIGNAL_MAX) ? STRING_UNKNOWN : STRING_CHECK(SIGNAL_STR[_TYPE_]))

		signal::signal(void) :
			m_state(SIGNAL_CLEAR)
		{
			return;
		}

		signal::~signal(void)
		{
			abandon();
		}

		void 
		signal::abandon(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex_signal);

			m_state = SIGNAL_ABANDON;
			m_condition.notify_all();
		}

		bool 
		signal::abandoned(void) const
		{
			return (m_state != SIGNAL_ABANDON);
		}

		void 
		signal::notify(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex_signal);

			if(m_state == SIGNAL_ABANDON) {
				THROW_NOMIC_CORE_SIGNAL_EXCEPTION(NOMIC_CORE_SIGNAL_EXCEPTION_ABANDONED);
			}

			m_state = SIGNAL_SET;
			m_condition.notify_all();
		}

		std::string 
		signal::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			result << NOMIC_CORE_SIGNAL_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " State=" << SCALAR_AS_HEX(uint32_t, m_state) << "(" << SIGNAL_STRING(m_state) << ")";
			}

			return result.str();
		}

		bool 
		signal::wait(
			__in uint32_t timeout
			)
		{
			bool result = true;

			std::unique_lock<std::mutex> lock(m_mutex_signal);

			if(m_state == SIGNAL_ABANDON) {
				THROW_NOMIC_CORE_SIGNAL_EXCEPTION(NOMIC_CORE_SIGNAL_EXCEPTION_ABANDONED);
			}

			if(timeout != TIMEOUT_UNDEFINED) {
				result = m_condition.wait_for(lock, std::chrono::milliseconds(timeout),
					[this](void) { return (m_state != SIGNAL_CLEAR); });
			} else {
				m_condition.wait(lock, [this](void) { return (m_state != SIGNAL_CLEAR); });
			}

			if(m_state != SIGNAL_ABANDON) {
				m_state = SIGNAL_CLEAR;
			}

			return result;
		}
	}
}
