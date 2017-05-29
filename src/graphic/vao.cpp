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

#include "../../include/graphic/vao.h"
#include "../../include/trace.h"
#include "./vao_type.h"

namespace nomic {

	namespace graphic {

		vao::vao(void) :
			nomic::core::primitive(PRIMITIVE_VAO)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		vao::vao(
			__in const vao &other
			) :
				nomic::core::primitive(other),
				m_vbo(other.m_vbo)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		vao::~vao(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		vao &
		vao::operator=(
			__in const vao &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::core::primitive::operator=(other);
				m_vbo = other.m_vbo;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		vao::add(
			__in const nomic::graphic::vbo &vbo,
			__in GLuint index,
			__in GLint size,
			__in GLenum type,
			__in_opt GLboolean normalized,
			__in_opt GLsizei stride,
			__in_opt const GLvoid *pointer
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Vbo=%p, Index=%x, Size=%u, Type=%x, Normalized=%x, Stride=%u, Pointer=%p",
				&vbo, index, size, type, normalized, stride, pointer);

			if(m_vbo.find(index) != m_vbo.end()) {
				THROW_NOMIC_GRAPHIC_VAO_EXCEPTION_FORMAT(NOMIC_GRAPHIC_VAO_EXCEPTION_DUPLICATE, "Index=%x", index);
			}

			bind();
			vbo.bind();
			GL_CHECK(LEVEL_WARNING, glVertexAttribPointer, index, size, type, normalized, stride, pointer);
			m_vbo.insert(std::make_pair(index, vbo));
			enable(index);			

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		vao::bind(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glBindVertexArray, m_handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		vao::contains(
			__in GLuint index
			) const
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%x", index);

			result = (m_vbo.find(index) != m_vbo.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		vao::disable(
			__in GLuint index
			) const
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%x", index);

			if(m_vbo.find(index) == m_vbo.end()) {
				THROW_NOMIC_GRAPHIC_VAO_EXCEPTION_FORMAT(NOMIC_GRAPHIC_VAO_EXCEPTION_NOT_FOUND, "Index=%x", index);
			}

			GL_CHECK(LEVEL_WARNING, glDisableVertexAttribArray, index);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		vao::disable_all(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			for(std::map<GLuint, nomic::graphic::vbo>::const_iterator iter = m_vbo.begin(); iter != m_vbo.end(); ++iter) {
				GL_CHECK(LEVEL_WARNING, glDisableVertexAttribArray, iter->first);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		vao::enable(
			__in GLuint index
			) const
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%x", index);

			if(m_vbo.find(index) == m_vbo.end()) {
				THROW_NOMIC_GRAPHIC_VAO_EXCEPTION_FORMAT(NOMIC_GRAPHIC_VAO_EXCEPTION_NOT_FOUND, "Index=%x", index);
			}

			GL_CHECK(LEVEL_WARNING, glEnableVertexAttribArray, index);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		vao::enable_all(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			for(std::map<GLuint, nomic::graphic::vbo>::const_iterator iter = m_vbo.begin(); iter != m_vbo.end(); ++iter) {
				GL_CHECK(LEVEL_WARNING, glEnableVertexAttribArray, iter->first);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::map<GLuint, nomic::graphic::vbo>::iterator 
		vao::find(
			__in GLuint index
			)
		{
			std::map<GLuint, nomic::graphic::vbo>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%x", index);

			result = m_vbo.find(index);
			if(result == m_vbo.end()) {
				THROW_NOMIC_GRAPHIC_VAO_EXCEPTION_FORMAT(NOMIC_GRAPHIC_VAO_EXCEPTION_NOT_FOUND, "Index=%x", index);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		void 
		vao::remove(
			__in GLuint index
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%x", index);

			m_vbo.erase(find(index));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		vao::remove_all(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_vbo.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		vao::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_VAO_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::primitive::to_string(verbose)
					<< ", Vbo[" << m_vbo.size() << "]";

				if(!m_vbo.empty()) {
					result << "={";

					for(std::map<GLuint, nomic::graphic::vbo>::const_iterator iter = m_vbo.begin(); iter != m_vbo.end(); ++iter) {

						if(iter != m_vbo.begin()) {
							result << ", ";
						}

						result << "(" << SCALAR_AS_HEX(GLuint, iter->first) << ", " << iter->second.to_string(verbose) << ")";
					}

					result << "}";
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
