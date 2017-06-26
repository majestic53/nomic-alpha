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

#ifndef NOMIC_TERRAIN_GENERATOR_H_
#define NOMIC_TERRAIN_GENERATOR_H_

#include "../core/noise.h"
#include "../core/random.h"
#include "./chunk.h"

namespace nomic {

	namespace terrain {

		class generator :
				protected nomic::core::noise {

			public:

				generator(
					__in_opt uint32_t seed = NOISE_SEED_DEFAULT,
					__in_opt uint32_t octaves = NOISE_OCTAVES_DEFAULT,
					__in_opt double amplitude = NOISE_AMPLITUDE_DEFAULT,
					__in_opt uint32_t max = CHUNK_MAX_DEFAULT
					);

				generator(
					__in const generator &other
					);

				virtual ~generator(void);

				generator &operator=(
					__in const generator &other
					);

				void chunk(
					__in const glm::ivec2 &position,
					__inout nomic::terrain::chunk &chunk
					);

				uint32_t max(void) const;

				uint32_t seed(void) const;

				void setup(
					__in uint32_t seed,
					__in_opt uint32_t octaves = NOISE_OCTAVES_DEFAULT,
					__in_opt double amplitude = NOISE_AMPLITUDE_DEFAULT,
					__in_opt uint32_t max = CHUNK_MAX_DEFAULT
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				uint32_t chunk_block_blend(
					__in uint32_t position,
					__in uint32_t min,
					__in uint32_t max,
					__in uint32_t a,
					__in uint32_t b
					);

				void chunk_column(
					__in const glm::uvec3 &position,
					__in nomic::terrain::chunk &chunk
					);

				uint32_t m_max;

				nomic::core::random<double> m_random;
		};
	}
}

#endif // NOMIC_TERRAIN_GENERATOR_H_
