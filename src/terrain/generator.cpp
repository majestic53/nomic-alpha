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
				m_max(max),
				m_noise_cloud(seed + seed, octaves, amplitude),
				m_noise_terrain(seed, octaves, amplitude),
				m_random(seed)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Seed=%u(%x), Octaves=%u, Amplitude=%g, Max=%u", seed, seed, octaves, amplitude, max);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		generator::generator(
			__in const generator &other
			) :
				m_max(other.m_max),
				m_noise_cloud(other.m_noise_cloud),
				m_noise_terrain(other.m_noise_terrain)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
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
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				m_max = other.m_max;
				m_noise_cloud = other.m_noise_cloud;
				m_noise_terrain = other.m_noise_terrain;
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
			uint8_t attributes = (BLOCK_ATTRIBUTE_STATIC & ~BLOCK_ATTRIBUTE_BREAKABLE);

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}, Chunk=%p", position.x, position.y, &chunk);

			chunk.set_position(position);

			for(int32_t z = (CHUNK_WIDTH - 1); z >= 0; --z) {

				for(int32_t x = 0; x < CHUNK_WIDTH; ++x) {
					scale = std::abs(m_noise_terrain.generate(((position.x * CHUNK_WIDTH) + x) / NOISE_SCALE,
						((position.y * CHUNK_WIDTH) + z) / NOISE_SCALE));

					height = (((BLOCK_HEIGHT_MAX - BLOCK_HEIGHT_MIN) * scale) + BLOCK_HEIGHT_MIN);
					if(height < BLOCK_HEIGHT_MIN) {
						height = BLOCK_HEIGHT_MIN;
					}

					if(height > BLOCK_HEIGHT_MAX) {
						height = BLOCK_HEIGHT_MAX;
					}

					for(int32_t y = (CHUNK_HEIGHT - 1); y >= 0; --y) {

						if(!y) { // boundary
							chunk.set_block(glm::uvec3(x, y, z), BLOCK_BOUNDARY, attributes);
						} else if(y == BLOCK_HEIGHT_CLOUD) { // cloud
							chunk_decoration_cloud(glm::uvec3(x, y, z), chunk, y, std::abs(m_noise_cloud.generate(
								((position.x * CHUNK_WIDTH) + x) / NOISE_SCALE_CLOUD,
								((position.y * CHUNK_WIDTH) + z) / NOISE_SCALE_CLOUD)));
						} else if(y > height) { // air
							chunk.set_block(glm::uvec3(x, y, z), BLOCK_AIR, attributes | BLOCK_ATTRIBUTE_HIDDEN);
						} else { // other
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
			uint32_t depth = 0, zone;
			bool bottom = false, top = true;
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

				if(top) {
					top = false;

					if(!chunk_decoration_alpine(glm::uvec3(position.x, y + 1, position.z), chunk)) { // alpine decoration
						chunk_decoration_tree_spruce(glm::uvec3(position.x, y + 1, position.z), chunk);
					}
				}
			} else if((y < BLOCK_LEVEL_ALPINE) && (y >= BLOCK_LEVEL_GRASS_STEP)) { // gravel/dirt
				zone = BLOCK_ZONE_ALPINE;
				type = chunk_block_blend(y, ((BLOCK_LEVEL_ALPINE - 1) - ((BLOCK_LEVEL_ALPINE - 1) - BLOCK_LEVEL_GRASS_STEP)),
					BLOCK_LEVEL_ALPINE - 1, BLOCK_GRAVEL, BLOCK_DIRT);

				if(top) {
					bool result = false;

					top = false;

					result = chunk_decoration_alpine(glm::uvec3(position.x, y + 1, position.z), chunk); // alpine decoration
					if(!result) {
						result = chunk_decoration_tree_spruce(glm::uvec3(position.x, y + 1, position.z), chunk);
					}

					if(!result && (type == BLOCK_DIRT)) { // gtass
						type = BLOCK_GRASS;
					}
				}
			} else if((y < BLOCK_LEVEL_GRASS_STEP) && (y >= BLOCK_LEVEL_GRASS)) { // dirt
				zone = BLOCK_ZONE_GRASS;
				type = BLOCK_DIRT;

				if(top) { // grass
					top = false;
					type = BLOCK_GRASS;

					if(!chunk_decoration_grassland(glm::uvec3(position.x, y + 1, position.z), chunk)) { // grass decoration
						chunk_decoration_tree_oak(glm::uvec3(position.x, y + 1, position.z), chunk);
					}
				}
			} else if((y < BLOCK_LEVEL_GRASS) && (y >= BLOCK_LEVEL_BEACH_ROCKS)) { // dirt/stone
				zone = BLOCK_ZONE_GRASS;
				type = chunk_block_blend(y, ((BLOCK_LEVEL_GRASS - 1) - ((BLOCK_LEVEL_GRASS - 1) - BLOCK_LEVEL_BEACH_ROCKS)),
					BLOCK_LEVEL_GRASS - 1, BLOCK_DIRT, BLOCK_STONE);

				if(top && (type == BLOCK_DIRT)) { // grass
					top = false;
					type = BLOCK_GRASS;

					if(!chunk_decoration_grassland(glm::uvec3(position.x, y + 1, position.z), chunk)) { // grass decoration
						chunk_decoration_tree_oak(glm::uvec3(position.x, y + 1, position.z), chunk);
					}
				}
			} else if((y < BLOCK_LEVEL_BEACH_ROCKS) && (y >= BLOCK_LEVEL_BEACH_GRAVEL)) { // stone/gravel
				zone = BLOCK_ZONE_BEACH;
				type = chunk_block_blend(y, ((BLOCK_LEVEL_BEACH_ROCKS - 1) - ((BLOCK_LEVEL_BEACH_ROCKS - 1) - BLOCK_LEVEL_BEACH_GRAVEL)),
					BLOCK_LEVEL_BEACH_ROCKS - 1, BLOCK_STONE, BLOCK_GRAVEL);

				if(top && (type == BLOCK_GRAVEL)) {
					top = false;
					type = chunk_block_pick_uniform(BLOCK_GRAVEL, BLOCK_CLAY);
					chunk_decoration_beach(glm::uvec3(position.x, y + 1, position.z), chunk); // beach decoration
				}
			} else { // sand/gravel/stone (underwater)
				zone = BLOCK_ZONE_SEA;
				type = chunk_block_blend(y, BLOCK_LEVEL_BEACH_SAND - (BLOCK_LEVEL_BEACH_SAND - BLOCK_HEIGHT_MIN),
					BLOCK_LEVEL_BEACH_SAND, BLOCK_SAND, chunk_block_pick(BLOCK_STONE, BLOCK_CLAY));

				if(top) { // gravel/stone
					top = false;

					if(type == BLOCK_STONE) {
						type = chunk_block_blend(y, BLOCK_LEVEL_BEACH_SAND - (BLOCK_LEVEL_BEACH_SAND - BLOCK_HEIGHT_MIN),
							BLOCK_LEVEL_BEACH_SAND, BLOCK_SAND, BLOCK_STONE);

						if(type == BLOCK_STONE) {
							type = BLOCK_SANDSTONE;
						}
					} else if((type == BLOCK_CLAY) || (type == BLOCK_SAND)) {
						chunk_decoration_beach(glm::uvec3(position.x, y + 1, position.z), chunk); // beach decoration
					}
				}
			}

			chunk.set_block(glm::uvec3(position.x, y, position.z), type, attribute);

			if(y) {

				for(y = (position.y - 1); y >= BLOCK_HEIGHT_MIN; --y) { // block columns for y >= min
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
					type = BLOCK_STONE;
					attribute = BLOCK_ATTRIBUTES_DEFAULT;

					if(y <= (BLOCK_HEIGHT_BOUNDARY + BLOCK_DEPTH_BOUNDARY_MAX)) { // stone/boundary

						if(!bottom) {
							type = chunk_block_blend(y, BLOCK_HEIGHT_BOUNDARY,
								BLOCK_HEIGHT_BOUNDARY + BLOCK_DEPTH_BOUNDARY_MAX,
								BLOCK_STONE, BLOCK_BOUNDARY);
							bottom = (type == BLOCK_BOUNDARY);
						} else {
							type = BLOCK_BOUNDARY;
						}
					}

					if(type == BLOCK_BOUNDARY) {
						attribute &= ~BLOCK_ATTRIBUTE_BREAKABLE;
					}

					chunk.set_block(glm::uvec3(position.x, y, position.z), type, attribute);
				}

				chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_BOUNDARY, // boundary
					BLOCK_ATTRIBUTES_DEFAULT & ~BLOCK_ATTRIBUTE_BREAKABLE);

				if((position.y < BLOCK_HEIGHT_WATER)
						&& (chunk.type(glm::uvec3(position.x, BLOCK_HEIGHT_WATER + 1, position.z)) == BLOCK_AIR)) { // water
					uint32_t attributes = (BLOCK_ATTRIBUTES_DEFAULT & ~BLOCK_ATTRIBUTE_BREAKABLE);

					for(y = BLOCK_HEIGHT_WATER; y > position.y; --y) {
						chunk.set_block(glm::uvec3(position.x, y, position.z), BLOCK_WATER,
							(y == BLOCK_HEIGHT_WATER) ? attributes : (attributes | BLOCK_ATTRIBUTE_HIDDEN));
					}

					if(position.y <= (BLOCK_HEIGHT_MIN + BLOCK_DECORATION_CORAL_REEF_PAD)) { // sea decoration
						chunk_decoration_underwater_reef(glm::uvec3(position.x, y + 1, position.z), chunk);
					} else {
						chunk_decoration_underwater(glm::uvec3(position.x, y + 1, position.z), chunk);
					}
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		generator::chunk_decoration_alpine(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			bool result = false;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Chunk=%p", position.x, position.y, position.z, &chunk);

			if(chunk.type(position) == BLOCK_AIR) {
				uint8_t type = chunk_block_pick(BLOCK_AIR, BLOCK_SHRUB); // shrub

				result = (type == BLOCK_SHRUB);
				if(result) {
					chunk.set_block(position, BLOCK_SHRUB);
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		generator::chunk_decoration_beach(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			bool result = false;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Chunk=%p", position.x, position.y, position.z, &chunk);

			if(chunk.type(position) == BLOCK_AIR) {

				if(position.y == BLOCK_LEVEL_BEACH_SAND) { // sugar-cane
					uint8_t type = chunk_block_pick(BLOCK_AIR, chunk_block_pick(BLOCK_AIR, BLOCK_SUGAR_CANE));

					result = (type == BLOCK_SUGAR_CANE);
					if(result) {

						for(uint32_t iter = 0; iter < BLOCK_DECORATION_SUGAR_CANE_MAX; ++iter) {

							if(chunk.type(glm::uvec3(position.x, position.y + iter + 1, position.z)) != BLOCK_AIR) {
								break;
							} else if((iter >= BLOCK_DECORATION_SUGAR_CANE_AVERAGE)
									&& (chunk_block_pick(BLOCK_AIR, BLOCK_SUGAR_CANE) == BLOCK_AIR)) {
								break;
							}

							chunk.set_block(glm::uvec3(position.x, position.y + iter, position.z), type);
						}
					}
				} else if((position.y < BLOCK_LEVEL_BEACH_ROCKS) && (position.y >= BLOCK_LEVEL_BEACH_GRAVEL)) { // cactus
					uint8_t type = chunk_block_pick(BLOCK_AIR, chunk_block_pick(BLOCK_AIR, BLOCK_CACTUS));

					result = (type == BLOCK_CACTUS);
					if(result) {

						for(uint32_t iter = 0; iter < BLOCK_DECORATION_CACTUS_MAX; ++iter) {

							if(chunk.type(glm::uvec3(position.x, position.y + iter + 1, position.z)) != BLOCK_AIR) {
								break;
							} else if((iter >= BLOCK_DECORATION_CACTUS_AVERAGE)
									&& (chunk_block_pick(BLOCK_AIR, BLOCK_CACTUS) == BLOCK_AIR)) {
								break;
							}

							chunk.set_block(glm::uvec3(position.x, position.y + iter, position.z), type,
								BLOCK_ATTRIBUTES_DEFAULT);
						}
					}
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		generator::chunk_decoration_cloud(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk,
			__inout int32_t &height,
			__in double scale
			)
		{
			uint32_t iter = 0;
			bool result = true;
			float threshold = BLOCK_CLOUD_THRESHOLD;
			uint8_t attributes = (BLOCK_ATTRIBUTE_STATIC & ~BLOCK_ATTRIBUTE_BREAKABLE);

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Height=%u, Chunk=%p, Scale=%f", position.x, position.y, position.z,
				height, &chunk, scale);

			for(; iter < BLOCK_CLOUD_THICKNESS; ++iter, threshold -= BLOCK_CLOUD_THRESHOLD_STEP) {

				if(scale >= threshold) {
					uint32_t iter_layer = 0;

					for(; iter_layer < (BLOCK_CLOUD_THICKNESS - iter - 1); ++iter_layer) { // cloud
						chunk.set_block(glm::uvec3(position.x, position.y - iter_layer, position.z), BLOCK_CLOUD,
							attributes | BLOCK_ATTRIBUTE_HIDDEN);
					}

					chunk.set_block(glm::uvec3(position.x, position.y - iter_layer, position.z), BLOCK_CLOUD, attributes);
					height -= iter_layer;
					break;
				}
			}

			if(iter == BLOCK_CLOUD_THICKNESS) { // air
				result = false;
				chunk.set_block(position, BLOCK_AIR, attributes | BLOCK_ATTRIBUTE_HIDDEN);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		generator::chunk_decoration_grassland(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			bool result = false;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Chunk=%p", position.x, position.y, position.z, &chunk);

			if(chunk.type(position) == BLOCK_AIR) {
				uint8_t type = chunk_block_pick(BLOCK_AIR, BLOCK_GRASS_TALL);

				result = (type == BLOCK_GRASS_TALL);
				if(result) { // grass
					type = chunk_block_pick(BLOCK_GRASS_TALL, BLOCK_GRASS_SHORT);

					if((position.y >= BLOCK_DECORATION_FLOWER_HEIGHT_MIN)
							&& (position.y < BLOCK_DECORATION_FLOWER_HEIGHT_MAX)) { // flower

						type = chunk_block_pick(type, chunk_block_pick_uniform(BLOCK_FLOWER_RED, BLOCK_FLOWER_YELLOW));
					}

					chunk.set_block(position, type);
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		generator::chunk_decoration_tree_oak(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			bool result = false;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Chunk=%p", position.x, position.y, position.z, &chunk);

			if(chunk.type(position) == BLOCK_AIR) {
				uint8_t type = chunk_block_pick(BLOCK_AIR, chunk_block_pick(BLOCK_AIR, BLOCK_WOOD_OAK));

				result = (type == BLOCK_WOOD_OAK);
				if(result) { // tree
					chunk.set_spawn(position, BLOCK_WOOD_OAK);
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		generator::chunk_decoration_tree_spruce(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			bool result = false;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Chunk=%p", position.x, position.y, position.z, &chunk);

			if(chunk.type(position) == BLOCK_AIR) {
				uint8_t type = chunk_block_pick(BLOCK_AIR, chunk_block_pick(BLOCK_AIR, BLOCK_WOOD_SPRUCE));

				result = (type == BLOCK_WOOD_SPRUCE);
				if(result) { // spruce-tree
					chunk.set_spawn(position, BLOCK_WOOD_SPRUCE);
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		generator::chunk_decoration_underwater(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			bool result = false;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Chunk=%p", position.x, position.y, position.z, &chunk);

			if(chunk.type(glm::uvec3(position.x, position.y + BLOCK_DECORATION_UNDERWATER_PAD, position.z)) == BLOCK_WATER) {
				uint8_t type = chunk_block_pick(BLOCK_WATER, BLOCK_SEAGRASS_GREEN);

				result = (type == BLOCK_SEAGRASS_GREEN);
				if(result) {

					type = chunk_block_pick_uniform(BLOCK_SEAGRASS_GREEN, BLOCK_CORAL_ORANGE);
					if(type == BLOCK_SEAGRASS_GREEN) { // sea-grass
						type = chunk_block_pick(BLOCK_SEAGRASS_GREEN, BLOCK_SEAGRASS_BROWN);

						for(uint32_t iter = 0; iter < BLOCK_DECORATION_SEAGRASS_MAX; ++iter) {

							if(chunk.type(glm::uvec3(position.x, position.y + BLOCK_DECORATION_UNDERWATER_PAD + iter + 1,
									position.z)) != BLOCK_WATER) {
								break;
							} else if((iter >= BLOCK_DECORATION_SEAGRASS_AVERAGE)
									&& (chunk_block_pick(BLOCK_WATER, BLOCK_SEAGRASS_GREEN) == BLOCK_WATER)) {
								break;
							}

							chunk.set_block(glm::uvec3(position.x, position.y + iter, position.z), type);
						}
					} else { // coral
						type = chunk_block_pick(chunk_block_pick(chunk_block_pick_uniform(BLOCK_CORAL_ORANGE, BLOCK_CORAL_PINK),
							chunk_block_pick_uniform(BLOCK_CORAL_PURPLE, BLOCK_CORAL_BLUE)), BLOCK_CORAL_BROWN);
						chunk.set_block(position, type);
					}
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		generator::chunk_decoration_underwater_reef(
			__in const glm::uvec3 &position,
			__in nomic::terrain::chunk &chunk
			)
		{
			bool result = false;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Chunk=%p", position.x, position.y, position.z, &chunk);

			if(chunk.type(glm::uvec3(position.x, position.y + BLOCK_DECORATION_UNDERWATER_PAD, position.z)) == BLOCK_WATER) {
				result = true;

				uint8_t type = chunk_block_pick(BLOCK_CORAL_ORANGE, BLOCK_SEAGRASS_GREEN);
				if(type == BLOCK_CORAL_ORANGE) { // coral
					type = chunk_block_pick_uniform(BLOCK_WATER, chunk_block_pick(chunk_block_pick(chunk_block_pick_uniform(
						BLOCK_CORAL_ORANGE, BLOCK_CORAL_PINK), chunk_block_pick_uniform(BLOCK_CORAL_PURPLE, BLOCK_CORAL_BLUE)),
						BLOCK_CORAL_BROWN));

					result = (type != BLOCK_WATER);
					if(result) {
						chunk.set_block(position, type);
					}
				} else { // sea-grass
					type = chunk_block_pick(BLOCK_SEAGRASS_GREEN, BLOCK_SEAGRASS_BROWN);

					for(uint32_t iter = 0; iter < BLOCK_DECORATION_SEAGRASS_MAX; ++iter) {

						if(chunk.type(glm::uvec3(position.x, position.y + BLOCK_DECORATION_UNDERWATER_PAD + iter + 1,
								position.z)) != BLOCK_WATER) {
							break;
						} else if((iter >= BLOCK_DECORATION_SEAGRASS_AVERAGE)
								&& (chunk_block_pick(BLOCK_WATER, BLOCK_SEAGRASS_GREEN) == BLOCK_WATER)) {
							break;
						}

						chunk.set_block(glm::uvec3(position.x, position.y + iter, position.z), type);
					}
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
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
			uint32_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = m_noise_terrain.seed();

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u(%x)", result, result);
			return result;
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

			m_max = max;
			m_noise_cloud.setup(seed + seed, octaves, amplitude);
			m_noise_terrain.setup(seed, octaves, amplitude);
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
				result << " Terrain=" << m_noise_terrain.to_string(verbose)
					<< ", Cloud=" << m_noise_cloud.to_string(verbose)
					<< ", Max=" << m_max;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
