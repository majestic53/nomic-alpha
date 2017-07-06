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

#include "../../include/terrain/manager.h"
#include "../../include/trace.h"
#include "./manager_type.h"

namespace nomic {

	namespace terrain {

		manager::manager(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);	
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		manager::~manager(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		nomic::entity::chunk *
		manager::at(
			__in const glm::ivec2 &position,
			__in_opt bool generate
			)
		{
			nomic::entity::chunk *result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}, Generate=%x", position.x, position.y, generate);

			if(!m_initialized) {
				THROW_NOMIC_TERRAIN_MANAGER_EXCEPTION(NOMIC_TERRAIN_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(generate) {
				std::map<std::pair<int32_t, int32_t>, nomic::entity::chunk *>::iterator iter;

				nomic::entity::chunk *entry = new nomic::entity::chunk(position, m_generator);
				if(!entry) {
					THROW_NOMIC_TERRAIN_MANAGER_EXCEPTION_FORMAT(NOMIC_TERRAIN_MANAGER_EXCEPTION_ALLOCATE, "Position={%i, %i}",
						position.x, position.y);
				}

				iter = m_chunk.find(std::make_pair(position.x, position.y));
				if(iter == m_chunk.end()) {
					m_chunk.insert(std::make_pair(std::make_pair(position.x, position.y), entry));
				} 
			}

			result = find(position)->second;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", result);
			return result;
		}

		bool 
		manager::contains(
			__in const glm::ivec2 &position
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}", position.x, position.y);

			if(!m_initialized) {
				THROW_NOMIC_TERRAIN_MANAGER_EXCEPTION(NOMIC_TERRAIN_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			result = (m_chunk.find(std::make_pair(position.x, position.y)) != m_chunk.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		std::map<std::pair<int32_t, int32_t>, nomic::entity::chunk *>::iterator 
		manager::find(
			__in const glm::ivec2 &position
			)
		{
			std::map<std::pair<int32_t, int32_t>, nomic::entity::chunk *>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}", position.x, position.y);

			result = m_chunk.find(std::make_pair(position.x, position.y));
			if(result == m_chunk.end()) {
				THROW_NOMIC_TERRAIN_MANAGER_EXCEPTION_FORMAT(NOMIC_TERRAIN_MANAGER_EXCEPTION_NOT_FOUND, "Position={%i, %i}",
					position.x, position.y);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		nomic::terrain::generator &
		manager::generator(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return m_generator;
		}

		bool 
		manager::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Terrain manager initializing...");
			TRACE_MESSAGE(LEVEL_INFORMATION, "Terrain manager initialized.");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Terrain manager uninitializing...");

			for(std::map<std::pair<int32_t, int32_t>, nomic::entity::chunk *>::iterator iter = m_chunk.begin(); iter != m_chunk.end();
					++iter) {

				if(iter->second) {
					delete iter->second;
					iter->second = nullptr;
				}
			}

			m_chunk.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Terrain manager uninitialized.");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		manager::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_TERRAIN_MANAGER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << SINGLETON_CLASS(nomic::terrain::manager)::to_string(verbose);

				if(m_initialized) {
					result << ", Chunk[" << m_chunk.size() << "]"
						<< ", Generator=" << m_generator.to_string(verbose);
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
