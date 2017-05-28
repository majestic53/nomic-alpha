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

#include "../../include/graphic/manager.h"
#include "../../include/trace.h"
#include "./manager_type.h"

namespace nomic {

	namespace graphic {

		enum {
			HANDLE_REFERENCES = 0,
			HANDLE_SUBTYPE,
			HANDLE_TYPE,
		};

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

		bool 
		manager::contains(
			__in GLuint handle
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%x", handle);

			result = (m_handle.find(handle) != m_handle.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		size_t 
		manager::decrement(
			__in GLuint handle
			)
		{
			size_t result = 0;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%x", handle);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION(NOMIC_GRAPHIC_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			std::map<GLuint, std::tuple<size_t, GLenum, uint32_t>>::iterator iter = find(handle);
			if(std::get<HANDLE_REFERENCES>(iter->second) <= REFERENCE_INIT) {
				destroy(iter);
				m_handle.erase(iter);
			} else {
				result = --std::get<HANDLE_REFERENCES>(iter->second);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		void 
		manager::destroy(
			__in std::map<GLuint, std::tuple<size_t, GLenum, uint32_t>>::iterator iter
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(iter == m_handle.end()) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_INVALID_HANDLE, "Handle=%x", iter->first);
			}

			uint32_t type = std::get<HANDLE_TYPE>(iter->second);
			switch(type) {
				case PRIMITIVE_PROGRAM:
					GL_CHECK(LEVEL_WARNING, glDeleteProgram, iter->first);
					break;
				case PRIMITIVE_SHADER:
					GL_CHECK(LEVEL_WARNING, glDeleteShader, iter->first);
					break;
				case PRIMITIVE_VAO:
					GL_CHECK(LEVEL_WARNING, glDeleteVertexArrays, HANDLE_COUNT, &iter->first);
					break;
				case PRIMITIVE_VBO:
					GL_CHECK(LEVEL_WARNING, glDeleteBuffers, HANDLE_COUNT, &iter->first);
					break;
				default:
					THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_INVALID_TYPE, "Type=%x", type);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::map<GLuint, std::tuple<size_t, GLenum, uint32_t>>::iterator 
		manager::find(
			__in GLuint handle
			)
		{
			std::map<GLuint, std::tuple<size_t, GLenum, uint32_t>>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%x", handle);

			result = m_handle.find(handle);
			if(result == m_handle.end()) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_NOT_FOUND, "Handle=%x", handle);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		GLuint 
		manager::generate(
			__in uint32_t type,
			__in_opt GLenum subtype
			)
		{
			GLuint result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x", type, subtype);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION(NOMIC_GRAPHIC_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			switch(type) {
				case PRIMITIVE_PROGRAM:
					GL_CHECK_RESULT(LEVEL_WARNING, glCreateProgram, result);
					break;
				case PRIMITIVE_SHADER:
					GL_CHECK_RESULT(LEVEL_WARNING, glCreateShader, result, subtype);
					break;
				case PRIMITIVE_VAO:
					GL_CHECK(LEVEL_WARNING, glGenVertexArrays, HANDLE_COUNT, &result);
					break;
				case PRIMITIVE_VBO:
					GL_CHECK(LEVEL_WARNING, glGenBuffers, HANDLE_COUNT, &result);
					break;
				default:
					THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_INVALID_TYPE, "Type=%x", type);
			}

			m_handle.insert(std::make_pair(result, std::make_tuple(REFERENCE_INIT, subtype, type)));

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		size_t 
		manager::increment(
			__in GLuint handle
			)
		{
			size_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%x", handle);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION(NOMIC_GRAPHIC_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			result = ++std::get<HANDLE_REFERENCES>(find(handle)->second);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		bool 
		manager::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Graphic manager initialized");

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			for(std::map<GLuint, std::tuple<size_t, GLenum, uint32_t>>::iterator iter = m_handle.begin(); iter != m_handle.end();
					++iter) {
				destroy(iter);
			}

			m_handle.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Graphic manager uninitialized");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		size_t 
		manager::references(
			__in GLuint handle
			)
		{
			size_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%x", handle);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION(NOMIC_GRAPHIC_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			result = std::get<HANDLE_REFERENCES>(find(handle)->second);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		std::string 
		manager::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_MANAGER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << SINGLETON_CLASS(nomic::graphic::manager)::to_string(verbose)
					<< ", Handle[" << m_handle.size() << "]";

				if(!m_handle.empty()) {
					result << "={";

					for(std::map<GLuint, std::tuple<size_t, GLenum, uint32_t>>::const_iterator iter = m_handle.begin();
							iter != m_handle.end(); ++iter) {

						if(iter != m_handle.begin()) {
							result << ", ";
						}

						result << "(" << SCALAR_AS_HEX(GLuint, iter->first)
							<< ", Type=" << SCALAR_AS_HEX(uint32_t, std::get<HANDLE_TYPE>(iter->second))
							<< ", Subtype=" << SCALAR_AS_HEX(GLenum, std::get<HANDLE_SUBTYPE>(iter->second))
							<< ", References=" << std::get<HANDLE_REFERENCES>(iter->second) << ")";
					}

					result << "}";
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
