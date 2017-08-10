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

#ifndef NOMIC_CORE_SINGLETON_H_
#define NOMIC_CORE_SINGLETON_H_

#include "../define.h"
#include "./singleton_type.h"

namespace nomic {

	namespace core {

		#define SINGLETON_CLASS(_CLASS_) nomic::core::singleton<_CLASS_>
		#define SINGLETON_CLASS_BASE(_CLASS_) friend class nomic::core::singleton<_CLASS_>

		template <class T> class singleton {

			public:

				~singleton(void)
				{
					release();
				}

				static T &acquire(void)
				{
					static T result;

					if(!result.m_references && !result.on_create()) {
						THROW_NOMIC_CORE_SINGLETON_EXCEPTION(NOMIC_CORE_SINGLETON_EXCEPTION_CREATE);
					}

					++result.m_references;

					return result;
				}

				void initialize(void)
				{
					std::lock_guard<std::mutex> lock(m_mutex_singleton);

					if(m_initialized) {
						THROW_NOMIC_CORE_SINGLETON_EXCEPTION(NOMIC_CORE_SINGLETON_EXCEPTION_INITIALIZED);
					}

					if(!on_initialize()) {
						THROW_NOMIC_CORE_SINGLETON_EXCEPTION(NOMIC_CORE_SINGLETON_EXCEPTION_INITIALIZE);
					}

					m_initialized = true;
				}

				bool initialized(void) const
				{
					return m_initialized;
				}

				size_t references(void) const
				{
					return m_references;
				}

				size_t release(void)
				{
					size_t result = 0;

					if(m_references > 0) {

						result = --m_references;
						if(!result) {

							if(m_initialized) {
								m_initialized = false;
								on_uninitialize();
							}

							on_destroy();
						}
					}

					return result;
				}

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const
				{
					std::stringstream result;

					result << NOMIC_CORE_SINGLETON_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

					if(verbose) {
						result << " State=" << (m_initialized ? "Initialized" : "Uninitialized")
							<< ", References=" << m_references;
					}

					return result.str();
				}

				void uninitialize(void)
				{
					std::lock_guard<std::mutex> lock(m_mutex_singleton);

					if(m_initialized) {
						m_initialized = false;
						on_uninitialize();
					}
				}

			protected:

				singleton(void) :
					m_initialized(false),
					m_references(0)
				{
					return;
				}

				singleton(
					__in const singleton &other
					) = delete;

				singleton &operator=(
					__in const singleton &other
					) = delete;

				virtual bool on_create(void)
				{
					return true;
				}

				virtual void on_destroy(void)
				{
					return;
				}

				virtual bool on_initialize(void)
				{
					return true;
				}

				virtual void on_uninitialize(void)
				{
					return;
				}

				bool m_initialized;

				std::mutex m_mutex_singleton;

				size_t m_references;
		};
	}
}

#endif // NOMIC_CORE_SINGLETON_H_
