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

#include <cstring>
#include "../../include/terrain/chunk.h"
#include "../../include/trace.h"
#include "./chunk_type.h"

namespace nomic {

	namespace terrain {

		enum {
			BLOCK_POSITION = 0,
			BLOCK_TYPE,
			BLOCK_ATTRIBUTES,
		};

		enum {
			SPAWN_POSITION = 0,
			SPAWN_TIMEOUT,
			SPAWN_TYPE,
		};

		chunk::chunk(
			__in_opt const glm::ivec2 &position
			) :
				m_active(false),
				m_position(position)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}", position.x, position.y);

			clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		chunk::chunk(
			__in const chunk &other
			) :
				m_active(other.m_active),
				m_position(other.m_position),
				m_queue(other.m_queue),
				m_spawn(other.m_spawn)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}", other.m_position.x, other.m_position.y);

			copy(other);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		chunk::~chunk(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		chunk &
		chunk::operator=(
			__in const chunk &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}", other.m_position.x, other.m_position.y);

			if(this != &other) {
				m_active = other.m_active;
				m_position = other.m_position;
				m_queue = other.m_queue;
				m_spawn = other.m_spawn;
				copy(other);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		uint8_t 
		chunk::attributes(
			__in const glm::uvec3 &position
			) const
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}", position.x, position.y, position.z);

			if((position.x >= CHUNK_WIDTH) || (position.y >= CHUNK_HEIGHT) || (position.z >= CHUNK_WIDTH)) {
				result = 0;
			} else {
				result = m_block_attributes[position.x][position.y][position.z];
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", (uint16_t) result);
			return result;
		}

		void 
		chunk::clear(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			memset(&m_block, BLOCK_AIR, CHUNK_BLOCK_COUNT);
			memset(&m_block_attributes, BLOCK_ATTRIBUTES_DEFAULT, CHUNK_BLOCK_COUNT);
			memset(&m_height, 0, CHUNK_BLOCK_SURFACE_COUNT);
			m_spawn.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		chunk::copy(
			__in const chunk &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			memcpy(&m_block, &other.m_block, CHUNK_BLOCK_COUNT);
			memcpy(&m_block_attributes, &other.m_block_attributes, CHUNK_BLOCK_COUNT);
			memcpy(&m_height, &other.m_height, CHUNK_BLOCK_SURFACE_COUNT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint8_t 
		chunk::height(
			__in const glm::uvec2 &position
			) const
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u}", position.x, position.y);

			if((position.x >= CHUNK_WIDTH) || (position.y >= CHUNK_HEIGHT)) {
				result = 0;
			} else {
				result = m_height[position.x][position.y];
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", (uint16_t) result);
			return result;
		}

		glm::ivec2 
		chunk::position(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}", m_position.x, m_position.y);
			return m_position;
		}

		void 
		chunk::set(
			__in const glm::uvec3 &position,
			__in uint8_t type,
			__in_opt uint8_t attributes
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Type=%x, Attributes=%x", position.x, position.y, position.z,
				type, attributes);

			m_queue.push(std::make_tuple(position, type, attributes));
			m_active = true;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint8_t 
		chunk::set_block(
			__in const glm::uvec3 &position,
			__in uint8_t type,
			__in_opt uint8_t attributes
			)
		{
			int8_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Type=%x, Attributes=%x", position.x, position.y, position.z,
				type, attributes);

			if((position.x >= CHUNK_WIDTH) || (position.y >= CHUNK_HEIGHT) || (position.z >= CHUNK_WIDTH)) {
				THROW_NOMIC_TERRAIN_CHUNK_EXCEPTION_FORMAT(NOMIC_TERRAIN_CHUNK_EXCEPTION_POSITION_INVALID,
					"Position={%u, %u, %u}", position.x, position.y, position.z);
			}

			m_block[position.x][position.y][position.z] = type;
			m_block_attributes[position.x][position.y][position.z] = attributes;

			result = (CHUNK_HEIGHT - 1);
			while(!nomic::utility::block_selectable(m_block[position.x][result][position.z])) {
				--result;
			}

			m_height[position.x][position.z] = result;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return (uint8_t) result;
		}

		void 
		chunk::set_spawn(
			__in const glm::uvec3 &position,
			__in uint8_t type,
			__in_opt uint32_t timeout
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Type=%x, Timeout=%u", position.x, position.y, position.z,
				type, timeout);

			m_spawn.push_back(std::make_tuple(position, timeout, type));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		chunk::set_position(
			__in const glm::ivec2 &position
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}", position.x, position.y);

			m_position = position;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::vector<std::pair<glm::uvec3, uint8_t>> 
		chunk::spawn(void)
		{
			std::vector<std::pair<glm::uvec3, uint8_t>> result;
			std::vector<std::vector<std::tuple<glm::uvec3, uint32_t, uint8_t>>::iterator> complete;

			TRACE_ENTRY(LEVEL_VERBOSE);

			for(std::vector<std::tuple<glm::uvec3, uint32_t, uint8_t>>::iterator iter = m_spawn.begin(); iter != m_spawn.end();
					++iter) {

				uint32_t &timeout = std::get<SPAWN_TIMEOUT>(*iter);
				if(!timeout) {
					complete.push_back(iter);
					result.push_back(std::make_pair(std::get<SPAWN_POSITION>(*iter), std::get<SPAWN_TYPE>(*iter)));
				} else {
					--timeout;
				}
			}

			for(std::vector<std::vector<std::tuple<glm::uvec3, uint32_t, uint8_t>>::iterator>::iterator iter = complete.begin();
					iter != complete.end(); ++iter) {
				m_spawn.erase(*iter);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		std::string 
		chunk::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_TERRAIN_CHUNK_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Position={" << m_position.x << ", " << m_position.y << "}"
					<< ", State=" << (m_active ? "Active" : "Inactive");
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		uint8_t 
		chunk::type(
			__in const glm::uvec3 &position
			) const
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}", position.x, position.y, position.z);

			if((position.x >= CHUNK_WIDTH) || (position.y >= CHUNK_HEIGHT) || (position.z >= CHUNK_WIDTH)) {
				result = 0;
			} else {
				result = m_block[position.x][position.y][position.z];
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		chunk::update(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(m_active) {
				m_active = false;

				while(!m_queue.empty()) {
					std::tuple<glm::uvec3, uint8_t, uint8_t> &action = m_queue.front();
					set_block(std::get<BLOCK_POSITION>(action), std::get<BLOCK_TYPE>(action), std::get<BLOCK_ATTRIBUTES>(action));
					m_queue.pop();
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
