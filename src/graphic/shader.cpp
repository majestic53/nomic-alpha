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

#include "../../include/graphic/shader.h"
#include "../../include/trace.h"
#include "./shader_type.h"

namespace nomic {

	namespace graphic {

		shader::shader(
			__in GLenum type,
			__in_opt const std::string &input,
			__in_opt bool is_file
			) :
				nomic::core::primitive(PRIMITIVE_SHADER, type)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Input[%u]=%p, File=%x", type, input.size(), &input[0], is_file);

			if(!input.empty()) {
				set(input, is_file);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		shader::shader(
			__in const shader &other
			) :
				nomic::core::primitive(other)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		shader::~shader(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		shader &
		shader::operator=(
			__in const shader &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::core::primitive::operator=(other);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Return=%p", this);
			return *this;
		}

		void 
		shader::set(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			GLchar *source;			
			std::string buffer;
			GLint complete, length;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Input[%u]=%p, File=%x", input.size(), &input[0], is_file);

			if(is_file) {
				buffer = nomic::utility::read_file(input);
			} else {
				buffer = input;
			}

			length = buffer.size();
			source = (GLchar *) &buffer[0];
			GL_CHECK(LEVEL_WARNING, glShaderSource, m_handle, HANDLE_COUNT, (const GLchar **) &source, &length);
			GL_CHECK(LEVEL_WARNING, glCompileShader, m_handle);

			GL_CHECK(LEVEL_WARNING, glGetShaderiv, m_handle, GL_COMPILE_STATUS, &complete);
			if(!complete) {
				buffer.clear();

				GL_CHECK(LEVEL_WARNING, glGetShaderiv, m_handle, GL_INFO_LOG_LENGTH, &length);
				if(length > 0) {
					buffer.resize(length, 0);
					GL_CHECK(LEVEL_WARNING, glGetShaderInfoLog, m_handle, length, &length, (GLchar *) &buffer[0]);
				}

				if(is_file) {
					THROW_NOMIC_GRAPHIC_SHADER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_SHADER_EXCEPTION_EXTERNAL, "Path[%u]=%s, %s",
						input.size(), &input[0], STRING_CHECK(buffer));
				} else {
					THROW_NOMIC_GRAPHIC_SHADER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_SHADER_EXCEPTION_EXTERNAL, "%s",
						STRING_CHECK(buffer));
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		shader::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_SHADER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::primitive::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
