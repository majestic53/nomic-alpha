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
			__in uint32_t type,
			__in GLuint handle
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Handle=%x", type, handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION(NOMIC_GRAPHIC_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(handle == HANDLE_INVALID) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%x", handle);
			}

			std::map<uint32_t, std::map<GLuint, std::pair<size_t, GLenum>>>::iterator iter = m_handle.find(type);

			result = (iter != m_handle.end());
			if(result) {
				result = (iter->second.find(handle) != iter->second.end());
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		size_t 
		manager::decrement(
			__in uint32_t type,
			__in GLuint handle
			)
		{
			size_t result = 0;
			std::map<GLuint, std::pair<size_t, GLenum>>::iterator iter_handle;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Handle=%x", type, handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION(NOMIC_GRAPHIC_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			iter_handle = find(type, handle);
			if(iter_handle->second.first <= REFERENCE_INIT) {
				destroy(type, iter_handle);

				std::map<uint32_t, std::map<GLuint, std::pair<size_t, GLenum>>>::iterator iter_type = m_handle.find(type);
				if(iter_type != m_handle.end()) {
					iter_type->second.erase(iter_handle);

					if(iter_type->second.empty()) {
						m_handle.erase(iter_type);
					}
				}
			} else {
				result = --iter_handle->second.first;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		void 
		manager::destroy(
			__in uint32_t type,
			__in std::map<GLuint, std::pair<size_t, GLenum>>::iterator iter
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

			switch(type) {
				case PRIMITIVE_CHARACTER:
				case PRIMITIVE_TEXTURE:
					GL_CHECK(LEVEL_WARNING, glDeleteTextures, HANDLE_COUNT, &iter->first);
					break;
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
					THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_TYPE_INVALID, "Type=%x", type);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::map<GLuint, std::pair<size_t, GLenum>>::iterator 
		manager::find(
			__in uint32_t type,
			__in GLuint handle
			)
		{
			std::map<GLuint, std::pair<size_t, GLenum>>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Handle=%x", type, handle);

			if(handle == HANDLE_INVALID) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%x", handle);
			}

			std::map<uint32_t, std::map<GLuint, std::pair<size_t, GLenum>>>::iterator iter = m_handle.find(type);
			if(iter == m_handle.end()) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_TYPE_NOT_FOUND, "Type=%x", type);
			}

			result = iter->second.find(handle);
			if(result == iter->second.end()) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_HANDLE_NOT_FOUND, "Handle=%x", handle);
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
			GLuint result = HANDLE_INVALID;
			std::map<uint32_t, std::map<GLuint, std::pair<size_t, GLenum>>>::iterator iter;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x", type, subtype);

			std::lock_guard<std::mutex> lock(m_mutex);

			switch(type) {
				case PRIMITIVE_CHARACTER:
				case PRIMITIVE_TEXTURE:
					GL_CHECK(LEVEL_WARNING, glGenTextures, HANDLE_COUNT, &result);
					break;
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
					THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_TYPE_INVALID, "Type=%x", type);
			}

			if(result == HANDLE_INVALID) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%x", result);
			}

			if(m_handle.find(type) == m_handle.end()) {
				m_handle.insert(std::make_pair(type, std::map<GLuint, std::pair<size_t, GLenum>>()));
			}

			iter = m_handle.find(type);
			if(iter->second.find(result) != iter->second.end()) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_MANAGER_EXCEPTION_HANDLE_DUPLICATE, "Handle=%x", result);
			}

			iter->second.insert(std::make_pair(result, std::make_pair(REFERENCE_INIT, subtype)));

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		size_t 
		manager::increment(
			__in uint32_t type,
			__in GLuint handle
			)
		{
			size_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Handle=%x", type, handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION(NOMIC_GRAPHIC_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			result = ++find(type, handle)->second.first;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		bool 
		manager::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Graphic manager initializing...");
			TRACE_MESSAGE(LEVEL_INFORMATION, "Graphic manager initialized");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Graphic manager uninitializing...");

			for(std::map<uint32_t, std::map<GLuint, std::pair<size_t, GLenum>>>::iterator iter_type = m_handle.begin();
					iter_type != m_handle.end(); ++iter_type) {

				for(std::map<GLuint, std::pair<size_t, GLenum>>::iterator iter_handle = iter_type->second.begin();
						iter_handle != iter_type->second.end(); ++iter_handle) {
					destroy(iter_type->first, iter_handle);
				}

				iter_type->second.clear();
			}

			m_handle.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Graphic manager uninitialized");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		size_t 
		manager::references(
			__in uint32_t type,
			__in GLuint handle
			)
		{
			size_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Handle=%x", type, handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_MANAGER_EXCEPTION(NOMIC_GRAPHIC_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			result = find(type, handle)->second.first;

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

					for(std::map<uint32_t, std::map<GLuint, std::pair<size_t, GLenum>>>::const_iterator iter_type = m_handle.begin();
							iter_type != m_handle.end(); ++iter_type) {

						if(iter_type != m_handle.begin()) {
							result << ", ";
						}

						result << "(" << SCALAR_AS_HEX(uint32_t, iter_type->first) << "[" << iter_type->second.size() << "]";

						if(!iter_type->second.empty()) {
							result << "={";

							for(std::map<GLuint, std::pair<size_t, GLenum>>::const_iterator iter_handle
									= iter_type->second.begin(); iter_handle != iter_type->second.end();
									++iter_handle) {

								if(iter_handle != iter_type->second.begin()) {
									result << ", ";
								}

								result << "(Handle=" << SCALAR_AS_HEX(GLuint, iter_handle->first)
									<< ", Enum=" << SCALAR_AS_HEX(GLenum, iter_handle->second.second)
									<< ", Reference=" << iter_handle->second.first << ")";
							}

							result << "}";
						}

						result << ")";
					}

					result << "}";
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
