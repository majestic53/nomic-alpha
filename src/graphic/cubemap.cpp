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

#include "../../include/graphic/cubemap.h"
#include "../../include/graphic/bitmap.h"
#include "../../include/trace.h"
#include "./cubemap_type.h"

namespace nomic {

	namespace graphic {

		cubemap::cubemap(
			__in_opt const std::map<uint32_t, std::string> &path,
			__in_opt float scale,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum wrap_r,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			) :
				nomic::core::primitive(PRIMITIVE_CUBEMAP)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u], Scale=%f, Wrap={%x, %x, %x}, Filter={%x, %x}", path.size(), scale,
				wrap_s, wrap_t, wrap_r, filter_min, filter_mag);

			if(!path.empty()) {
				set(path, scale, wrap_s, wrap_t, wrap_r, filter_min, filter_mag);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		cubemap::cubemap(
			__in const cubemap &other
			) :
				nomic::core::primitive(other)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		cubemap::~cubemap(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);	
		}

		cubemap &
		cubemap::operator=(
			__in const cubemap &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::core::primitive::operator=(other);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		cubemap::bind(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glBindTexture, GL_TEXTURE_CUBE_MAP, m_handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		cubemap::disable(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glDisable, GL_TEXTURE_2D);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		cubemap::enable(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			GL_CHECK(LEVEL_WARNING, glEnable, GL_TEXTURE_2D);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		cubemap::set(
			__in const std::map<uint32_t, std::string> &path,
			__in_opt float scale,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum wrap_r,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u], Scale=%f, Wrap={%x, %x, %x}, Filter={%x, %x}", path.size(), scale,
				wrap_s, wrap_t, wrap_r, filter_min, filter_mag);

			if(path.size() != ((GL_TEXTURE_CUBE_MAP_NEGATIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X) + 1)) {
				THROW_NOMIC_GRAPHIC_CUBEMAP_EXCEPTION_FORMAT(NOMIC_GRAPHIC_CUBEMAP_EXCEPTION_FACE_INVALID, "Count=%u (expected: %u)",
					path.size(), (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X) + 1);
			}

			GL_CHECK(LEVEL_WARNING, glBindTexture, GL_TEXTURE_CUBE_MAP, m_handle);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter_min);
			GL_CHECK(LEVEL_WARNING, glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter_mag);

			for(uint32_t face = GL_TEXTURE_CUBE_MAP_POSITIVE_X; face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++face) {

				std::map<uint32_t, std::string>::const_iterator iter = path.find(face);
				if(iter == path.end()) {
					THROW_NOMIC_GRAPHIC_CUBEMAP_EXCEPTION_FORMAT(NOMIC_GRAPHIC_CUBEMAP_EXCEPTION_FACE_INVALID, "Face=%u", face);
				}

				nomic::graphic::bitmap image(iter->second);
				glm::uvec2 dimensions(image.width(), image.height());
				GLenum format = GL_UNSIGNED_BYTE, mode = GL_RGB;

				uint32_t depth = image.depth();
				switch(depth / CHAR_WIDTH) {
					case BITMAP_DEPTH_24:
						format = GL_UNSIGNED_BYTE;
						mode = GL_RGB;
						break;
					case BITMAP_DEPTH_32:
						format = GL_UNSIGNED_INT_8_8_8_8;
						mode = GL_RGBA;
						break;
					default:
						THROW_NOMIC_GRAPHIC_CUBEMAP_EXCEPTION_FORMAT(NOMIC_GRAPHIC_CUBEMAP_EXCEPTION_DEPTH_INVALID,
							"Depth=%u", depth);
				}

				GL_CHECK(LEVEL_WARNING, glTexImage2D, face, 0, mode, dimensions.x, dimensions.y, 0, mode, format, image.pixels());
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		cubemap::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_CUBEMAP_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::primitive::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
