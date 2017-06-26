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

#ifndef NOMIC_CORE_NOISE_H_
#define NOMIC_CORE_NOISE_H_

#include <vector>
#include "../define.h"

namespace nomic {

	namespace core {

		class noise {

			public:

				noise(
					__in_opt uint32_t seed = NOISE_SEED_DEFAULT,
					__in_opt uint32_t octaves = NOISE_OCTAVES_DEFAULT,
					__in_opt double amplitude = NOISE_AMPLITUDE_DEFAULT,
					__in_opt double persistence = NOISE_PERSISTENCE_DEFAULT
					);

				noise(
					__in const noise &other
					);

				virtual ~noise(void);

				noise &operator=(
					__in const noise &other
					);

				double amplitude(void) const;

				double generate(
					__in double x
					);

				double generate(
					__in double x,
					__in double y
					);

				double generate(
					__in double x,
					__in double y,
					__in double z
					);

				uint32_t octaves(void) const;

				double persistence(void) const;

				uint32_t seed(void) const;

				void setup(
					__in uint32_t seed,
					__in_opt uint32_t octaves = NOISE_OCTAVES_DEFAULT,
					__in_opt double amplitude = NOISE_AMPLITUDE_DEFAULT,
					__in_opt double persistence = NOISE_PERSISTENCE_DEFAULT
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				double fade(
					__in double t
					);

				double generate_octave(
					__in double x,
					__in double y,
					__in double z
					);

				double gradient(
					__in uint32_t hash,
					__in double x,
					__in double y,
					__in double z
					);

				double interpolate(
					__in double t,
					__in double a,
					__in double b
					);

				double m_amplitude;

				uint32_t m_octaves;

				double m_persistence;

				uint32_t m_seed;

				std::vector<uint32_t> m_vector;
		};
	}
}

#endif // NOMIC_CORE_NOISE_H_
