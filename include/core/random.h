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

#ifndef NOMIC_CORE_RANDOM_H_
#define NOMIC_CORE_RANDOM_H_

#include <random>
#include "../trace.h"
#include "./random_type.h"

namespace nomic {

	namespace core {

		template <typename T> class random {

			public:

				explicit random(
					__in_opt T seed = RANDOM_SEED_DEFAULT
					) :
						m_seed(0)
				{
					TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Seed=%u(%x)", seed, seed);

					setup(seed);

					TRACE_EXIT(LEVEL_VERBOSE);
				}

				virtual ~random(void)
				{
					TRACE_ENTRY(LEVEL_VERBOSE);
					TRACE_EXIT(LEVEL_VERBOSE);
				}

				std::mt19937 &generator(void)
				{
					TRACE_ENTRY(LEVEL_VERBOSE);
					TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", &m_generator);
					return m_generator;
				}

				T next(void)
				{
					T result;

					TRACE_ENTRY(LEVEL_VERBOSE);

					result = m_generator();

					TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u(%x)", result, result);
					return result;
				}

				uint32_t seed(void) const
				{
					TRACE_ENTRY(LEVEL_VERBOSE);
					TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u(%x)", m_seed, m_seed);
					return m_seed;
				}
	
				void setup(
					__in uint32_t seed
					)
				{
					TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Seed=%u(%x)", seed, seed);

					if(!seed) {
						m_seed = m_device();
					} else {
						m_seed = seed;
					}

					m_generator = std::mt19937(m_seed);

					TRACE_EXIT(LEVEL_VERBOSE);
				}

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const
				{
					std::stringstream result;

					TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

					result << NOMIC_CORE_RANDOM_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

					if(verbose) {
						result << " Device=" << SCALAR_AS_HEX(uintptr_t, &m_device)
							<< ", Generator=" << SCALAR_AS_HEX(uintptr_t, &m_generator)
							<< ", Seed=" << SCALAR_AS_HEX(uint32_t, m_seed);
					}

					TRACE_EXIT(LEVEL_VERBOSE);
					return result.str();
				}

			protected:

				random(
					__in const random &other
					) = delete;

				random &operator=(
					__in const random &other
					) = delete;

				std::random_device m_device;

				std::mt19937 m_generator;

				uint32_t m_seed;
		};
	}
}

#endif //NOMIC_CORE_RANDOM_H_
