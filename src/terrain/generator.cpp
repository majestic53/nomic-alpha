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

#include "../../include/terrain/generator.h"
#include "../../include/trace.h"
#include "./generator_type.h"

namespace nomic {

	namespace terrain {

		generator::generator(
			__in_opt uint32_t seed,
			__in_opt uint32_t octaves,
			__in_opt double amplitude,
			__in_opt uint32_t max
			) :
				nomic::core::noise(seed, octaves, amplitude),
				m_max(max),
				m_random(seed)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Seed=%u(%x), Octaves=%u, Amplitude=%g, Max=%u", seed, seed, octaves, amplitude, max);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		generator::generator(
			__in const generator &other
			) :
				nomic::core::noise(other),
				m_max(other.m_max)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Seed=%u(%x), Octaves=%u, Amplitude=%g, Max=%u", other.m_seed, other.m_seed,
				other.m_octaves, other.m_amplitude, other.m_max);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		generator::~generator(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		generator &
		generator::operator=(
			__in const generator &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Seed=%u(%x), Octaves=%u, Amplitude=%g, Max=%u", other.m_seed, other.m_seed,
				other.m_octaves, other.m_amplitude, other.m_max);

			if(this != &other) {
				nomic::core::noise::operator=(other);
				m_max = other.m_max;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		generator::chunk(
			__in const glm::ivec2 &position,
			__inout nomic::terrain::chunk &chunk
			)
		{
			double scale;
			uint32_t height;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}, Chunk=%p", position.x, position.y, &chunk);

			chunk.set_position(position);

			for(int32_t z = (CHUNK_WIDTH - 1); z >= 0; --z) {

				for(int32_t x = 0; x < CHUNK_WIDTH; ++x) {
					scale = std::abs(nomic::core::noise::generate(((position.x * CHUNK_WIDTH) + x) / NOISE_SCALE,
						((position.y * CHUNK_WIDTH) + z) / NOISE_SCALE));

					height = (((BLOCK_HEIGHT_MAX - BLOCK_HEIGHT_MIN) * scale) + BLOCK_HEIGHT_MIN);
					if(height < BLOCK_HEIGHT_MIN) {
						height = BLOCK_HEIGHT_MIN;
					}

					if(height > BLOCK_HEIGHT_MAX) {
						height = BLOCK_HEIGHT_MAX;
					}

					for(int32_t y = (CHUNK_HEIGHT - 1); y >= 0; --y) {

						if(!y) {
							chunk.set_block(glm::uvec3(x, y, z), BLOCK_BOUNDARY,
								BLOCK_ATTRIBUTE_STATIC | ~BLOCK_ATTRIBUTE_BREAKABLE);
						} else if(y > height) {
							chunk.set_block(glm::uvec3(x, y, z), BLOCK_AIR,
								BLOCK_ATTRIBUTE_STATIC | ~BLOCK_ATTRIBUTE_BREAKABLE);
						} else {
							chunk_column(glm::vec3(x, y, z), chunk);
							break;
						}
					}
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint32_t 
		generator::chunk_block_blend(
			__in uint32_t position,
			__in uint32_t min,
			__in uint32_t max,
			__in uint32_t a,
			__in uint32_t b
			)
		{
			uint32_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position=%u, Range={%u, %u}, A=%u, B=%u", position, min, max, a, b);

			if(min > max) {
				THROW_NOMIC_TERRAIN_GENERATOR_EXCEPTION_FORMAT(NOMIC_TERRAIN_GENERATOR_EXCEPTION_RANGE_INVALID,
					"Range={%u, %u}", min, max);
			}

			if(position > max) {
				result = a;
			} else if(position < min) {
				result = b;
			} else {
				uint32_t lower, upper;
				std::vector<double> interval, weight;

				lower = ((a <= b) ? a : b);
				upper = ((a > b) ? a : b);

				if(min < (max - 1)) {

					for(uint32_t iter = lower; iter <= upper; ++iter) {
						interval.push_back(iter);

						if(iter == a) {
							weight.push_back((position - min) / (double) (max - min));
						} else if(iter == b) {
							weight.push_back(1.0 - ((position - min) / (double) (max - min)));
						} else {
							weight.push_back(0.0);
						}
					}

					result = (uint32_t) std::piecewise_linear_distribution<>(interval.begin(), interval.end(), weight.begin())(
						m_random.generator());

					if((result != lower) && (result != upper)) {
						result = ((((lower <= result) ? (result - lower) : (lower - result))
							<= ((upper <= result) ? (result - upper) : (upper - result))) ?
							lower : upper);
					}
				} else {

					std::uniform_int_distribution<> distribution(0, 1);
					if(distribution(m_random.generator())) {
						result = a;
					} else {
						result = b;
					}
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		generator::chunk_column(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			uint8_t type;
			bool top = true;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Chunk=%p", position.x, position.y, position.z, &chunk);

			for(int32_t y = position.y; y >= 0; --y) {

				// TODO: move this code out of for loop and populate block columns
				if(!y) {
					chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_BOUNDARY,
						BLOCK_ATTRIBUTE_STATIC | ~BLOCK_ATTRIBUTE_BREAKABLE);
				} else if(y >= BLOCK_LEVEL_ALPINE_PEAK) { // snow/stone
					chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_SNOW);
				} else if((y < BLOCK_LEVEL_ALPINE_PEAK) && (y >= BLOCK_LEVEL_ALPINE)) { // stone/gravel
					chunk.set_block(glm::uvec3(position.x, y, position.z), chunk_block_blend(y,
						((BLOCK_LEVEL_ALPINE_PEAK - 1) - ((BLOCK_LEVEL_ALPINE_PEAK - 1) - BLOCK_LEVEL_ALPINE)),
						BLOCK_LEVEL_ALPINE_PEAK - 1, BLOCK_STONE, BLOCK_GRAVEL));
				} else if((y < BLOCK_LEVEL_ALPINE) && (y >= BLOCK_LEVEL_GRASS_STEP)) { // gravel/dirt
					type = chunk_block_blend(y, ((BLOCK_LEVEL_ALPINE - 1) - ((BLOCK_LEVEL_ALPINE - 1) - BLOCK_LEVEL_GRASS_STEP)),
						BLOCK_LEVEL_ALPINE - 1, BLOCK_GRAVEL, BLOCK_DIRT);

					if(top && (type == BLOCK_DIRT)) { // grass
						top = false;
						chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_GRASS);
					} else { // gravel/dirt
						chunk.set_block(glm::uvec3(position.x, y, position.z), type);
					}
				} else if((y < BLOCK_LEVEL_GRASS_STEP) && (y >= BLOCK_LEVEL_GRASS)) { // dirt

					if(top) { // grass
						top = false;
						chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_GRASS);
					} else { // dirt
						chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_DIRT);
					}
				} else if((y < BLOCK_LEVEL_GRASS) && (y >= BLOCK_LEVEL_BEACH_ROCKS)) { // dirt/stone
					type = chunk_block_blend(y, ((BLOCK_LEVEL_GRASS - 1) - ((BLOCK_LEVEL_GRASS - 1) - BLOCK_LEVEL_BEACH_ROCKS)),
						BLOCK_LEVEL_GRASS - 1, BLOCK_DIRT, BLOCK_STONE);

					if(top && (type == BLOCK_DIRT)) { // grass
						top = false;
						chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_GRASS);
					} else { // stone
						chunk.set_block(glm::uvec3(position.x, y, position.z), type);
					}
				} else if((y < BLOCK_LEVEL_BEACH_ROCKS) && (y >= BLOCK_LEVEL_BEACH_GRAVEL)) { // stone/gravel
					chunk.set_block(glm::uvec3(position.x, y, position.z), chunk_block_blend(y,
						((BLOCK_LEVEL_BEACH_ROCKS - 1) - ((BLOCK_LEVEL_BEACH_ROCKS - 1) - BLOCK_LEVEL_BEACH_GRAVEL)),
						BLOCK_LEVEL_BEACH_ROCKS - 1, BLOCK_STONE, BLOCK_GRAVEL));
				} else { // sand (underwater)
					chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_SAND);
				}
				// ---
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint32_t 
		generator::max(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", m_max);
			return m_max;
		}

		uint32_t 
		generator::seed(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u(%x)", m_seed, m_seed);
			return m_seed;
		}

		void 
		generator::setup(
			__in uint32_t seed,
			__in_opt uint32_t octaves,
			__in_opt double amplitude,
			__in_opt uint32_t max
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Seed=%u(%x), Octaves=%u, Amplitude=%g, Max=%u", seed, seed, octaves, amplitude, max);

			nomic::core::noise::setup(seed, octaves, amplitude);
			m_max = max;
			m_random.setup(seed);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		generator::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_TERRAIN_GENERATOR_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::noise::to_string(verbose)
					<< ", Max=" << m_max;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
