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

#include "../../include/graphic/character.h"
#include "../../include/trace.h"
#include "./character_type.h"

namespace nomic {

	namespace graphic {

		character::character(void) :
			nomic::core::primitive(PRIMITIVE_CHARACTER),
			m_advance(0)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		character::character(
			__in FT_Face face,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			) :
				nomic::core::primitive(PRIMITIVE_CHARACTER),
				m_advance(0)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Face=%p, Dimension={%i, %i}, Bearing={%i, %i}, Advance=%u, Wrap={%x, %x}, Filter={%x, %x}",
				face, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap_left, face->glyph->bitmap_left,
				face->glyph->advance.x, wrap_s, wrap_t, filter_min, filter_mag);

			set(face, wrap_s, wrap_t, filter_min, filter_mag);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		character::character(
			__in const character &other
			) :
				nomic::core::primitive(other),
				m_advance(other.m_advance),
				m_bearing(other.m_bearing),
				m_dimension(other.m_dimension)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimension={%i, %i}, Bearing={%i, %i}, Advance=%u", other.m_dimension.x, other.m_dimension.y,
				other.m_bearing.x, other.m_bearing.y, other.m_advance);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		character::~character(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		character &
		character::operator=(
			__in const character &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimension={%i, %i}, Bearing={%i, %i}, Advance=%u", other.m_dimension.x, other.m_dimension.y,
				other.m_bearing.x, other.m_bearing.y, other.m_advance);

			if(this != &other) {
				nomic::core::primitive::operator=(other);
				m_advance = other.m_advance;
				m_bearing = other.m_bearing;
				m_dimension = other.m_dimension;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		uint32_t 
		character::advance(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", m_advance);
			return m_advance;
		}

		glm::ivec2 
		character::bearing(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%i, %i}", m_bearing.x, m_bearing.y);
			return m_bearing;
		}

		void 
		character::bind(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glBindTexture, GL_TEXTURE_2D, m_handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		glm::ivec2 
		character::dimension(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%i, %i}", m_dimension.x, m_dimension.y);
			return m_dimension;
		}

		void 
		character::disable(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glDisable, GL_TEXTURE_2D);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		character::enable(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glEnable, GL_TEXTURE_2D);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		int32_t 
		character::height(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%i", m_dimension.y);
			return m_dimension.y;
		}

		int32_t 
		character::left(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%i", m_bearing.x);
			return m_bearing.x;
		}

		void 
		character::set(
			__in FT_Face face,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Face=%p, Dimension={%i, %i}, Bearing={%i, %i}, Advance=%u, Wrap={%x, %x}, Filter={%x, %x}",
				face, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap_left, face->glyph->bitmap_top,
				face->glyph->advance.x, wrap_s, wrap_t, filter_min, filter_mag);

			if(!face) {
				THROW_NOMIC_GRAPHIC_CHARACTER_EXCEPTION_FORMAT(NOMIC_GRAPHIC_CHARACTER_EXCEPTION_INVALID, "Face=%p", face);
			}

			m_advance = face->glyph->advance.x;
			m_bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
			m_dimension = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
			GL_CHECK(LEVEL_WARNING, glBindTexture, GL_TEXTURE_2D, m_handle);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
			GL_CHECK(LEVEL_WARNING, glTexImage2D, GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			GL_CHECK(LEVEL_WARNING, glGenerateMipmap, GL_TEXTURE_2D);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		character::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_CHARACTER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::primitive::to_string(verbose)
					<< ", Dimension={" << m_dimension.x << ", " << m_dimension.y << "}"
					<< ", Bearing={" << m_bearing.x << ", " << m_bearing.y << "}"
					<< ", Advance=" << m_advance;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		int32_t 
		character::top(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%i", m_bearing.y);
			return m_bearing.y;
		}

		int32_t 
		character::width(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%i", m_dimension.x);
			return m_dimension.x;
		}
	}
}
