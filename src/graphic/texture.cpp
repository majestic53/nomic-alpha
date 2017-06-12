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

#include <climits>
#include "../../include/graphic/texture.h"
#include "../../include/graphic/bitmap.h"
#include "../../include/trace.h"
#include "./texture_type.h"

namespace nomic {

	namespace graphic {

		texture::texture(
			__in_opt const std::string &path,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			) :
				nomic::core::primitive(PRIMITIVE_TEXTURE),
				m_depth(0),
				m_mode(0)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s, Wrap={%x, %x}, Filter={%x, %x}", path.size(), STRING_CHECK(path),
				wrap_s, wrap_t, filter_min, filter_mag);

			if(!path.empty()) {
				set(path, wrap_s, wrap_t, filter_min, filter_mag);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		texture::texture(
			__in const texture &other
			) :
				nomic::core::primitive(other),
				m_depth(other.m_depth),
				m_dimension(other.m_dimension),
				m_mode(other.m_mode)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		texture::~texture(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		texture &
		texture::operator=(
			__in const texture &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::core::primitive::operator=(other);
				m_depth = other.m_depth;
				m_dimension = other.m_dimension;
				m_mode = other.m_mode;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		texture::bind(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glBindTexture, GL_TEXTURE_2D, m_handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint8_t 
		texture::depth(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", m_depth);
			return m_depth;
		}

		glm::uvec2 
		texture::dimension(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%u, %u}", m_dimension.x, m_dimension.y);
			return m_dimension;
		}

		void 
		texture::disable(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glDisable, GL_TEXTURE_2D);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		texture::enable(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glEnable, GL_TEXTURE_2D);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint32_t 
		texture::height(void) const
		{
			uint32_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = m_dimension.y;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		GLenum 
		texture::mode(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", m_mode);
			return m_mode;
		}

		void 
		texture::set(
			__in const std::string &path,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			)
		{
			nomic::graphic::bitmap image;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s, Wrap={%x, %x}, Filter={%x, %x}", path.size(), STRING_CHECK(path),
				wrap_s, wrap_t, filter_min, filter_mag);

			image.load(path);

			m_depth = image.depth();
			switch(m_depth / CHAR_WIDTH) {
				case BITMAP_DEPTH_24:
					m_mode = GL_RGB;
					break;
				case BITMAP_DEPTH_32:
					m_mode = GL_RGBA;
					break;
				default:
					THROW_NOMIC_GRAPHIC_TEXTURE_EXCEPTION_FORMAT(NOMIC_GRAPHIC_TEXTURE_EXCEPTION_DEPTH_INVALID,
						"Depth=%u", m_depth);
			}

			m_dimension = glm::uvec2(image.width(), image.height());
			GL_CHECK(LEVEL_WARNING, glBindTexture, GL_TEXTURE_2D, m_handle);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
			GL_CHECK(LEVEL_WARNING, glTexImage2D, GL_TEXTURE_2D, 0, m_mode, m_dimension.x, m_dimension.y, 0, m_mode,
				GL_UNSIGNED_INT_8_8_8_8, image.pixels());
			GL_CHECK(LEVEL_WARNING, glGenerateMipmap, GL_TEXTURE_2D);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		texture::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_TEXTURE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::primitive::to_string(verbose)
					<< ", Depth=" << m_depth
					<< ", Dimension={" << m_dimension.x << ", " << m_dimension.y << "}"
					<< ", Mode=" << SCALAR_AS_HEX(GLenum, m_mode);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		uint32_t 
		texture::width(void) const
		{
			uint32_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = m_dimension.x;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}
	}
}
