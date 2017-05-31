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

#include "../../include/graphic/program.h"
#include "../../include/trace.h"
#include "./program_type.h"

namespace nomic {

	namespace graphic {

		program::program(void) :
			nomic::core::primitive(PRIMITIVE_PROGRAM)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		program::program(
			__in const program &other
			) :
				nomic::core::primitive(other),
				m_attribute(other.m_attribute),
				m_shader(other.m_shader)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		program::~program(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		program &
		program::operator=(
			__in const program &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::core::primitive::operator=(other);
				m_attribute = other.m_attribute;
				m_shader = other.m_shader;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		program::add_attribute(
			__in GLuint index,
			__in const std::string &name
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%x, Name[%u]=%s", index, name.size(), STRING_CHECK(name));

			if(name.empty()) {
				THROW_NOMIC_GRAPHIC_PROGRAM_EXCEPTION_FORMAT(NOMIC_GRAPHIC_PROGRAM_INVALID_ATTRIBUTE, "Index=%x, Name[%u]=%s",
					index, name.size(), STRING_CHECK(name));
			}

			if(m_attribute.find(index) != m_attribute.end()) {
				THROW_NOMIC_GRAPHIC_PROGRAM_EXCEPTION_FORMAT(NOMIC_GRAPHIC_PROGRAM_DUPLICATE_ATTRIBUTE, "Index=%x, Name[%u]=%s",
					index, name.size(), STRING_CHECK(name));
			}

			m_attribute.insert(std::make_pair(index, name));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::add_shader(
			__in const nomic::graphic::shader &shader
			)
		{
			GLuint handle = shader.handle();

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%x", handle);

			if(m_shader.find(handle) != m_shader.end()) {
				THROW_NOMIC_GRAPHIC_PROGRAM_EXCEPTION_FORMAT(NOMIC_GRAPHIC_PROGRAM_DUPLICATE_SHADER, "Handle=%x", handle);
			}

			m_shader.insert(std::make_pair(handle, shader));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		size_t 
		program::attribute_count(void)
		{
			size_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = m_attribute.size();

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		bool 
		program::contains_attribute(
			__in GLuint index
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%x", index);

			result = (m_attribute.find(index) != m_attribute.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		program::contains_shader(
			__in GLuint handle
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%x", handle);

			result = (m_shader.find(handle) != m_shader.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		std::map<GLuint, std::string>::iterator 
		program::find_attribute(
			__in GLuint index
			)
		{
			std::map<GLuint, std::string>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%x", index);

			result = m_attribute.find(index);
			if(result == m_attribute.end()) {
				THROW_NOMIC_GRAPHIC_PROGRAM_EXCEPTION_FORMAT(NOMIC_GRAPHIC_PROGRAM_ATTRIBUTE_NOT_FOUND, "Index=%x", index);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		std::map<GLuint, nomic::graphic::shader>::iterator 
		program::find_shader(
			__in GLuint handle
			)
		{
			std::map<GLuint, nomic::graphic::shader>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%x", handle);

			result = m_shader.find(handle);
			if(result == m_shader.end()) {
				THROW_NOMIC_GRAPHIC_PROGRAM_EXCEPTION_FORMAT(NOMIC_GRAPHIC_PROGRAM_SHADER_NOT_FOUND, "Handle=%x", handle);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		void 
		program::link(void)
		{
			std::string buffer;
			GLint complete, length;

			TRACE_ENTRY(LEVEL_VERBOSE);

			for(std::map<GLuint, std::string>::iterator iter = m_attribute.begin(); iter != m_attribute.end(); ++iter) {
				GL_CHECK(LEVEL_WARNING, glBindAttribLocation, m_handle, iter->first, (const GLchar *) &iter->second[0]);
			}

			for(std::map<GLuint, nomic::graphic::shader>::iterator iter = m_shader.begin(); iter != m_shader.end(); ++iter) {
				GL_CHECK(LEVEL_WARNING, glAttachShader, m_handle, iter->second.handle());
			}

			GL_CHECK(LEVEL_WARNING, glLinkProgram, m_handle);

			GL_CHECK(LEVEL_WARNING, glGetProgramiv, m_handle, GL_LINK_STATUS, &complete);
			if(!complete) {

				GL_CHECK(LEVEL_WARNING, glGetProgramiv, m_handle, GL_INFO_LOG_LENGTH, &length);
				if(length > 0) {
					buffer.resize(length, 0);
					GL_CHECK(LEVEL_WARNING, glGetProgramInfoLog, m_handle, length, &length, (GLchar *) &buffer[0]);
				}

				THROW_NOMIC_GRAPHIC_PROGRAM_EXCEPTION_FORMAT(NOMIC_GRAPHIC_PROGRAM_EXCEPTION_EXTERNAL, "%s", STRING_CHECK(buffer));
			}

			for(std::map<GLuint, nomic::graphic::shader>::iterator iter = m_shader.begin(); iter != m_shader.end(); ++iter) {
				GL_CHECK(LEVEL_WARNING, glDetachShader, m_handle, iter->second.handle());
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::remove_all_attributes(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_attribute.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::remove_all_shaders(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_shader.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::remove_attribute(
			__in GLuint index
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%x", index);

			m_attribute.erase(find_attribute(index));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::remove_shader(
			__in GLuint handle
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%x", handle);

			m_shader.erase(find_shader(handle));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in GLboolean value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value=%x", location, value);

			GL_CHECK(LEVEL_WARNING, glUniform1ui, location, (GLuint) value);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in GLint value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value=%i", location, value);

			GL_CHECK(LEVEL_WARNING, glUniform1i, location, value);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in GLuint value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value=%u", location, value);

			GL_CHECK(LEVEL_WARNING, glUniform1ui, location, value);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in GLfloat value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value=%f", location, value);

			GL_CHECK(LEVEL_WARNING, glUniform1f, location, value);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::ivec2 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value={%i, %i}", location, value.x, value.y);

			GL_CHECK(LEVEL_WARNING, glUniform2iv, location, HANDLE_COUNT, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::ivec3 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value={%i, %i, %i}", location, value.x, value.y, value.z);

			GL_CHECK(LEVEL_WARNING, glUniform3iv, location, HANDLE_COUNT, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::ivec4 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value={%i, %i, %i, %i}", location, value.x, value.y, value.z, value.w);

			GL_CHECK(LEVEL_WARNING, glUniform4iv, location, HANDLE_COUNT, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::uvec2 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value={%u, %u}", location, value.x, value.y);

			GL_CHECK(LEVEL_WARNING, glUniform2uiv, location, HANDLE_COUNT, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::uvec3 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value={%u, %u, %u}", location, value.x, value.y, value.z);

			GL_CHECK(LEVEL_WARNING, glUniform3uiv, location, HANDLE_COUNT, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::uvec4 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value={%u, %u, %u, %u}", location, value.x, value.y, value.z, value.w);

			GL_CHECK(LEVEL_WARNING, glUniform4uiv, location, HANDLE_COUNT, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::vec2 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value={%f, %f}", location, value.x, value.y);

			GL_CHECK(LEVEL_WARNING, glUniform2fv, location, HANDLE_COUNT, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::vec3 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value={%f, %f, %f}", location, value.x, value.y, value.z);

			GL_CHECK(LEVEL_WARNING, glUniform3fv, location, HANDLE_COUNT, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::vec4 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value={%f, %f, %f, %f}", location, value.x, value.y, value.z, value.w);

			GL_CHECK(LEVEL_WARNING, glUniform4fv, location, HANDLE_COUNT, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		program::set_uniform(
			__in GLint location,
			__in const glm::mat4 &value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Location=%i, Value=%p", location, &value);

			GL_CHECK(LEVEL_WARNING, glUniformMatrix4fv, location, HANDLE_COUNT, GL_FALSE, glm::value_ptr(value));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		size_t 
		program::shader_count(void)
		{
			size_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = m_shader.size();

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		std::string 
		program::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_PROGRAM_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::primitive::to_string(verbose)
					<< ", Attribute[" << m_attribute.size() << "]";

				if(!m_attribute.empty()) {
					result << "={";

					for(std::map<GLuint, std::string>::const_iterator iter = m_attribute.begin(); iter != m_attribute.end();
							++iter) {

						if(iter != m_attribute.begin()) {
							result << ", ";
						}

						result << "(" << SCALAR_AS_HEX(GLuint, iter->first) << ", " << STRING_CHECK(iter->second) << ")";
					}

					result << "}";
				}

				result << ", Shader[" << m_shader.size() << "]";

				if(!m_shader.empty()) {
					result << "={";

					for(std::map<GLuint, nomic::graphic::shader>::const_iterator iter = m_shader.begin(); iter != m_shader.end();
							++iter) {

						if(iter != m_shader.begin()) {
							result << ", ";
						}

						result << "(" << iter->second.to_string(verbose) << ")";
					}

					result << "}";
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		GLint 
		program::uniform_location(
			__in const std::string &name
			)
		{
			GLint result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Name[%u]=%s", name.size(), STRING_CHECK(name));

			if(name.empty()) {
				THROW_NOMIC_GRAPHIC_PROGRAM_EXCEPTION_FORMAT(NOMIC_GRAPHIC_PROGRAM_INVALID_LOCATION, "Name[%u]=%s",
					name.size(), STRING_CHECK(name));
			}

			GL_CHECK_RESULT(LEVEL_WARNING, glGetUniformLocation, result, m_handle, (const GLchar *) &name[0]);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%i", result);
			return result;
		}

		void 
		program::use(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glUseProgram, m_handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
