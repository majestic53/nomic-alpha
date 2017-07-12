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

#include "../../include/graphic/atlas.h"
#include "../../include/trace.h"
#include "./atlas_type.h"

namespace nomic {

	namespace graphic {

		atlas::atlas(
			__in_opt const std::string &path,
			__in_opt const glm::uvec2 &dimensions,
			__in_opt uint8_t width
			) :
				m_dimensions(ATLAS_DIMENSIONS_DEFAULT),
				m_width(ATLAS_WIDTH_DEFAULT)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s, Dimensions={%u, %u}, Width=%u", path.size(), STRING_CHECK(path),
				dimensions.x, dimensions.y, width);

			if(!path.empty()) {
				load(path, dimensions, width);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		atlas::atlas(
			__in const atlas &other
			) :
				m_bitmap(other.m_bitmap),
				m_dimensions(other.m_dimensions),
				m_texture(other.m_texture),
				m_width(other.m_width)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		atlas::~atlas(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			destroy();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		atlas &
		atlas::operator=(
			__in const atlas &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				m_bitmap = other.m_bitmap;
				m_dimensions = other.m_dimensions;
				m_texture = other.m_texture;
				m_width = other.m_width;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		atlas::destroy(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_texture.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		atlas::disable(
			__in uint8_t type
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

			if(type >= m_texture.size()) {
				THROW_NOMIC_GRAPHIC_ATLAS_EXCEPTION_FORMAT(NOMIC_GRAPHIC_ATLAS_EXCEPTION_TYPE_INVALID, "Type=%x", type);
			}

			m_texture.at(type).disable();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		atlas::enable(
			__in uint8_t type
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

			if(type >= m_texture.size()) {
				THROW_NOMIC_GRAPHIC_ATLAS_EXCEPTION_FORMAT(NOMIC_GRAPHIC_ATLAS_EXCEPTION_TYPE_INVALID, "Type=%x", type);
			}

			m_texture.at(type).enable();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		atlas::load(
			__in const std::string &path,
			__in_opt const glm::uvec2 &dimensions,
			__in_opt uint8_t width
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s, Dimensions={%u, %u}, Width=%u", path.size(), STRING_CHECK(path),
				dimensions.x, dimensions.y, width);

			if(!dimensions.x || !dimensions.y) {
				THROW_NOMIC_GRAPHIC_ATLAS_EXCEPTION_FORMAT(NOMIC_GRAPHIC_ATLAS_EXCEPTION_DIMENSIONS_INVALID,
					"Dimensions={%u, %u}", dimensions.x, dimensions.y);
			}

			if(!width) {
				THROW_NOMIC_GRAPHIC_ATLAS_EXCEPTION_FORMAT(NOMIC_GRAPHIC_ATLAS_EXCEPTION_WIDTH_INVALID,
					"Width=%u", width);
			}

			m_bitmap.load(path);
			if((m_bitmap.width() % dimensions.x) || (m_bitmap.height() % dimensions.y)) {
				THROW_NOMIC_GRAPHIC_ATLAS_EXCEPTION_FORMAT(NOMIC_GRAPHIC_ATLAS_EXCEPTION_DIMENSIONS_MISMATCH,
					"Dimensions={%u, %u}", dimensions.x, dimensions.y);
			}

			if(m_bitmap.width() != (m_dimensions.x * m_width)) {
				THROW_NOMIC_GRAPHIC_ATLAS_EXCEPTION_FORMAT(NOMIC_GRAPHIC_ATLAS_EXCEPTION_WIDTH_MISMATCH,
					"Width=%u", width);
			}

			m_dimensions = dimensions;
			m_width = width;

			// TODO: create textures

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		atlas::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_ATLAS_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Bitmap=" << m_bitmap.to_string(verbose)
					<< ", Texture[" << m_texture.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_texture[0])
					<< ", Dimensions={" << m_dimensions.x << ", " << m_dimensions.x << "}"
					<< ", Width=" << m_width;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
