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

				if(min < (max - 1)) { // granularity > 2

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
				} else { // granularity <= 2
					result = chunk_block_pick_uniform(a, b);
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		uint32_t 
		generator::chunk_block_pick(
			__in uint32_t a,
			__in uint32_t b
			)
		{
			uint32_t result = a, value;
			std::vector<double> weight = { 0.56, 0.04 };

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "A=%u, B=%u", a, b);

			std::discrete_distribution<> distribution(weight.begin(), weight.end());

			value = (uint32_t) distribution(m_random.generator());
			switch(value) {
				case 0:
					result = a;
					break;
				case 1:
					result = b;
					break;
				default:
					break;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		uint32_t 
		generator::chunk_block_pick(
			__in uint32_t a,
			__in uint32_t b,
			__in uint32_t c
			)
		{
			uint32_t result = a, value;
			std::vector<double> weight = { 0.9, 0.56, 0.04 };

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "A=%u, B=%u, C=%u", a, b, c);

			std::discrete_distribution<> distribution(weight.begin(), weight.end());

			value = (uint32_t) distribution(m_random.generator());
			switch(value) {
				case 0:
					result = a;
					break;
				case 1:
					result = b;
					break;
				case 2:
					result = c;
					break;
				default:
					break;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		uint32_t 
		generator::chunk_block_pick_uniform(
			__in uint32_t a,
			__in uint32_t b
			)
		{
			uint32_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "A=%u, B=%u", a, b);

			std::uniform_int_distribution<> distribution(0, 1);
			if(distribution(m_random.generator())) {
				result = a;
			} else {
				result = b;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		void 
		generator::chunk_column(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			bool top = true;
			uint32_t depth = 0, zone;
			uint8_t attribute = BLOCK_ATTRIBUTES_DEFAULT, type;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Chunk=%p", position.x, position.y, position.z, &chunk);

			int32_t y = position.y;
			if(!y) { // boundary
				zone = BLOCK_ZONE_BOUNDARY;
				type = BLOCK_BOUNDARY;
				attribute &= ~BLOCK_ATTRIBUTE_BREAKABLE;
			} else if(y >= BLOCK_LEVEL_ALPINE_PEAK) { // snow/stone
				zone = BLOCK_ZONE_PEAK;
				type = BLOCK_SNOW;
			} else if((y < BLOCK_LEVEL_ALPINE_PEAK) && (y >= BLOCK_LEVEL_ALPINE)) { // stone/gravel
				zone = BLOCK_ZONE_ALPINE;
				type = chunk_block_blend(y, ((BLOCK_LEVEL_ALPINE_PEAK - 1) - ((BLOCK_LEVEL_ALPINE_PEAK - 1) - BLOCK_LEVEL_ALPINE)),
					BLOCK_LEVEL_ALPINE_PEAK - 1, BLOCK_STONE, BLOCK_GRAVEL);
			} else if((y < BLOCK_LEVEL_ALPINE) && (y >= BLOCK_LEVEL_GRASS_STEP)) { // gravel/dirt
				zone = BLOCK_ZONE_ALPINE;
				type = chunk_block_blend(y, ((BLOCK_LEVEL_ALPINE - 1) - ((BLOCK_LEVEL_ALPINE - 1) - BLOCK_LEVEL_GRASS_STEP)),
					BLOCK_LEVEL_ALPINE - 1, BLOCK_GRAVEL, BLOCK_DIRT);

				if(top && (type == BLOCK_DIRT)) { // grass
					top = false;
					type = BLOCK_GRASS;
				}
			} else if((y < BLOCK_LEVEL_GRASS_STEP) && (y >= BLOCK_LEVEL_GRASS)) { // dirt
				zone = BLOCK_ZONE_GRASS;
				type = BLOCK_DIRT;

				if(top) { // grass
					top = false;
					type = BLOCK_GRASS;
				}
			} else if((y < BLOCK_LEVEL_GRASS) && (y >= BLOCK_LEVEL_BEACH_ROCKS)) { // dirt/stone
				zone = BLOCK_ZONE_GRASS;
				type = chunk_block_blend(y, ((BLOCK_LEVEL_GRASS - 1) - ((BLOCK_LEVEL_GRASS - 1) - BLOCK_LEVEL_BEACH_ROCKS)),
					BLOCK_LEVEL_GRASS - 1, BLOCK_DIRT, BLOCK_STONE);

				if(top && (type == BLOCK_DIRT)) { // grass
					top = false;
					type = BLOCK_GRASS;
				}
			} else if((y < BLOCK_LEVEL_BEACH_ROCKS) && (y >= BLOCK_LEVEL_BEACH_GRAVEL)) { // stone/gravel
				zone = BLOCK_ZONE_BEACH;
				type = chunk_block_blend(y, ((BLOCK_LEVEL_BEACH_ROCKS - 1) - ((BLOCK_LEVEL_BEACH_ROCKS - 1) - BLOCK_LEVEL_BEACH_GRAVEL)),
					BLOCK_LEVEL_BEACH_ROCKS - 1, BLOCK_STONE, BLOCK_GRAVEL);
			} else { // sand/gravel/stone (underwater)
				zone = BLOCK_ZONE_SEA;
				type = chunk_block_blend(y, BLOCK_LEVEL_BEACH_SAND - (BLOCK_LEVEL_BEACH_SAND - BLOCK_HEIGHT_MIN),
					BLOCK_LEVEL_BEACH_SAND, BLOCK_SAND, BLOCK_STONE);

				if(top && (type == BLOCK_STONE)) { // gravel/stone
					top = false;
					type = chunk_block_blend(y, BLOCK_LEVEL_BEACH_SAND - (BLOCK_LEVEL_BEACH_SAND - BLOCK_HEIGHT_MIN),
						BLOCK_LEVEL_BEACH_SAND, BLOCK_SAND, BLOCK_STONE);

					if(type == BLOCK_STONE) {
						type = BLOCK_SANDSTONE;
					}
				}
			}

			chunk.set_block(glm::uvec3(position.x, y, position.z), type, attribute);

			// TODO: add real water effect
			if((y < BLOCK_HEIGHT_WATER)
					&& (chunk.type(glm::uvec3(position.x, BLOCK_HEIGHT_WATER + 1, position.z)) == BLOCK_AIR)) {
				chunk.set_block(glm::uvec3(position.x, BLOCK_HEIGHT_WATER, position.z), BLOCK_WATER,
					BLOCK_ATTRIBUTES_DEFAULT | ~BLOCK_ATTRIBUTE_BREAKABLE);
			}
			// ---

			--y;

			for(; y >= BLOCK_HEIGHT_MIN; --y) { // block columns for y >= min
				attribute = BLOCK_ATTRIBUTES_DEFAULT;

				switch(zone) {
					case BLOCK_ZONE_ALPINE:
					case BLOCK_ZONE_GRASS:
					case BLOCK_ZONE_PEAK:
						switch(type) {
							case BLOCK_DIRT: // dirt/gravel/stone
							case BLOCK_GRASS:

								type = chunk_block_pick(BLOCK_DIRT, BLOCK_GRAVEL, BLOCK_STONE);
								if((type == BLOCK_DIRT) && (depth >= BLOCK_DEPTH_DIRT_MAX)) {
									type = chunk_block_pick(BLOCK_GRAVEL, BLOCK_STONE);
									depth = 0;
								} else {
									++depth;
								}
								break;
							case BLOCK_GRAVEL: // gravel/stone

								type = chunk_block_pick(BLOCK_GRAVEL, BLOCK_STONE);
								if((type == BLOCK_GRAVEL) && (depth >= BLOCK_DEPTH_GRAVEL_MAX)) {
									type = BLOCK_STONE;
									depth = 0;
								} else {
									++depth;
								}
								break;
							default: // stone
								type = BLOCK_STONE;
								break;
						}
						break;
					case BLOCK_ZONE_BEACH:
					case BLOCK_ZONE_SEA:
						switch(type) {
							case BLOCK_SAND: // sand/sandstone/gravel

								type = chunk_block_pick(BLOCK_SAND, BLOCK_SANDSTONE, BLOCK_GRAVEL);
								if((type == BLOCK_SAND) && (depth >= BLOCK_DEPTH_SAND_MAX)) {
									type = chunk_block_pick(BLOCK_SANDSTONE, BLOCK_GRAVEL);
									depth = 0;
								} else {
									++depth;
								}
								break;
							case BLOCK_SANDSTONE: // sandstone/gravel/stone

								type = chunk_block_pick(BLOCK_SANDSTONE, BLOCK_GRAVEL, BLOCK_STONE);
								if((type == BLOCK_SANDSTONE) && (depth >= BLOCK_DEPTH_SANDSTONE_MAX)) {
									type = chunk_block_pick(BLOCK_GRAVEL, BLOCK_STONE);
									depth = 0;
								} else {
									++depth;
								}
								break;
							case BLOCK_GRAVEL: // gravel/stone

								type = chunk_block_pick(BLOCK_GRAVEL, BLOCK_STONE);
								if((type == BLOCK_GRAVEL) && (depth >= BLOCK_DEPTH_GRAVEL_MAX)) {
									type = BLOCK_STONE;
									depth = 0;
								} else {
									++depth;
								}
								break;
							default: // stone
								type = BLOCK_STONE;
								break;
						}
						break;
					default:
						type = BLOCK_BOUNDARY;
						break;
				}

				chunk.set_block(glm::uvec3(position.x, y, position.z), type, attribute);
			}

			for(; y >= BLOCK_HEIGHT_BOUNDARY; --y) { // block column < min and > boundary
				chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_STONE, BLOCK_ATTRIBUTES_DEFAULT);
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
