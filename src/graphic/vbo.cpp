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

#include "../../include/graphic/vbo.h"
#include "../../include/trace.h"
#include "./vbo_type.h"

namespace nomic {

	namespace graphic {

		vbo::vbo(
			__in GLenum target
			) :
				nomic::core::primitive(PRIMITIVE_VBO, target),
				m_usage(GL_STATIC_DRAW)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Target=%x", target);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		vbo::vbo(
			__in GLenum target,
			__in uint32_t size,
			__in GLenum usage
			) :
				nomic::core::primitive(PRIMITIVE_VBO, target),
				m_usage(GL_STATIC_DRAW)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Target=%x, Size=%u, Usage=%x", target, size, usage);

			set(size, usage);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		vbo::vbo(
			__in GLenum target,
			__in const std::vector<uint8_t> &data,
			__in GLenum usage
			) :
				nomic::core::primitive(PRIMITIVE_VBO, target),
				m_usage(GL_STATIC_DRAW)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Target=%x, Data[%u]=%p, Usage=%x", target, data.size(), &data[0], usage);

			set(data, usage);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		vbo::vbo(
			__in const vbo &other
			) :
				nomic::core::primitive(other),
				m_data(other.m_data),
				m_usage(other.m_usage)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Target=%x, Data[%u]=%p, Usage=%x", other.m_subtype, other.m_data.size(), &other.m_data[0],
				other.m_usage);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		vbo::~vbo(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		vbo &
		vbo::operator=(
			__in const vbo &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Target=%x, Data[%u]=%p, Usage=%x", other.m_subtype, other.m_data.size(), &other.m_data[0],
				other.m_usage);

			if(this != &other) {
				nomic::core::primitive::operator=(other);
				m_data = other.m_data;
				m_usage = other.m_usage;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		vbo::bind(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glBindBuffer, m_subtype, m_handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		vbo::set(
			__in uint32_t size,
			__in GLenum usage
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Size=%u, Usage=%x", size, usage);

			bind();
			GL_CHECK(LEVEL_WARNING, glBufferData, m_subtype, size, nullptr, usage);
			m_data.clear();
			m_usage = usage;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		vbo::set(
			__in const std::vector<uint8_t> &data,
			__in GLenum usage
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Data[%u]=%p, Usage=%x", data.size(), &data[0], usage);

			bind();
			GL_CHECK(LEVEL_WARNING, glBufferData, m_subtype, data.size(), &data[0], usage);
			m_data = data;
			m_usage = usage;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		vbo::set_subdata(
			__in GLintptr offset,
			__in GLsizeiptr size,
			__in const GLvoid *data
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Offset=%p, Data[%u]=%p", index, offset, size, data);

			bind();
			GL_CHECK(LEVEL_WARNING, glBufferSubData, m_subtype, offset, size, data);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		vbo::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_VBO_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::primitive::to_string(verbose)
					<< ", Data[" << m_data.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_data[0])
					<< ", Usage=" << SCALAR_AS_HEX(GLenum, m_usage);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
