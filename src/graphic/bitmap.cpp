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

#include "../../include/graphic/bitmap.h"
#include "../../include/trace.h"
#include "./bitmap_type.h"

namespace nomic {

	namespace graphic {

		bitmap::bitmap(
			__in_opt const std::string &path
			) :
				m_surface(nullptr)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s", path.size(), STRING_CHECK(path));

			if(!path.empty()) {
				load(path);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bitmap::bitmap(
			__in const bitmap &other
			) :
				m_surface(nullptr)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s", other.m_path.size(), STRING_CHECK(other.m_path));

			if(!other.m_path.empty()) {
				load(other.m_path);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bitmap::~bitmap(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			destroy();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bitmap &
		bitmap::operator=(
			__in const bitmap &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s", other.m_path.size(), STRING_CHECK(other.m_path));

			if((this != &other) && !other.m_path.empty()) {
				load(other.m_path);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		uint8_t 
		bitmap::depth(void) const
		{
			uint8_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_surface) {
				THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION(NOMIC_GRAPHIC_BITMAP_EXCEPTION_UNLOADED);
			}

			result = m_surface->format->BitsPerPixel;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		void 
		bitmap::destroy(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(m_surface) {
				SDL_FreeSurface(m_surface);
				m_surface = nullptr;
			}

			m_path.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint32_t 
		bitmap::height(void) const
		{
			uint32_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_surface) {
				THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION(NOMIC_GRAPHIC_BITMAP_EXCEPTION_UNLOADED);
			}

			result = m_surface->h;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		void 
		bitmap::load(
			__in const std::string &path
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s", path.size(), STRING_CHECK(path));

			destroy();

			m_surface = SDL_LoadBMP(&path[0]);
			if(!m_surface) {
				THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION_FORMAT(NOMIC_GRAPHIC_BITMAP_EXCEPTION_EXTERNAL, "%s", SDL_GetError());
			}

			m_path = path;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		bitmap::loaded(void) const
		{
			bool result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = (m_surface != nullptr);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		std::string 
		bitmap::path(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return m_path;
		}

		uint32_t 
		bitmap::pitch(void) const
		{
			uint32_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_surface) {
				THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION(NOMIC_GRAPHIC_BITMAP_EXCEPTION_UNLOADED);
			}

			result = m_surface->pitch;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		uint32_t 
		bitmap::pixel(
			__in uint32_t x,
			__in uint32_t y
			) const
		{
			size_t index;
			uint8_t depth;
			uint32_t result = 0;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u}", x, y);

			if(!m_surface) {
				THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION(NOMIC_GRAPHIC_BITMAP_EXCEPTION_UNLOADED);
			}

			depth = m_surface->format->BytesPerPixel;

			index = ((y * (m_surface->pitch / depth)) + x);
			if(index > (m_surface->w * m_surface->h)) {
				THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION_FORMAT(NOMIC_GRAPHIC_BITMAP_EXCEPTION_POSITION_INVALID,
					"Position={%u, %u}", x, y);
			}

			switch(depth) {
				case BITMAP_DEPTH_8:
					result = ((uint8_t *) m_surface->pixels)[index];
					break;
				case BITMAP_DEPTH_16:
					result = ((uint16_t *) m_surface->pixels)[index];
					break;
				case BITMAP_DEPTH_24:
					memcpy(&result, &((uint8_t *) m_surface->pixels)[index * depth], depth);
					break;
				case BITMAP_DEPTH_32:
					result = ((uint32_t *) m_surface->pixels)[index];
					break;
				default:
					THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION_FORMAT(NOMIC_GRAPHIC_BITMAP_EXCEPTION_DEPTH_INVALID,
						"Depth=%u", depth);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		void *
		bitmap::pixels(void) const
		{
			void *result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_surface) {
				THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION(NOMIC_GRAPHIC_BITMAP_EXCEPTION_UNLOADED);
			}

			result = m_surface->pixels;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", result);
			return result;
		}

		SDL_Surface *
		bitmap::surface(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_surface) {
				THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION(NOMIC_GRAPHIC_BITMAP_EXCEPTION_UNLOADED);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", m_surface);
			return m_surface;
		}

		std::string 
		bitmap::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_BITMAP_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " State=" << (m_surface ? "Loaded" : "Unloaded");

				if(m_surface) {
					result << ", Surface=" << SCALAR_AS_HEX(uintptr_t, m_surface) << ", Path[" << m_path.size() << "]";

					if(!m_path.empty()) {
						result << "=" << m_path;
					}
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		uint32_t 
		bitmap::width(void) const
		{
			uint32_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_surface) {
				THROW_NOMIC_GRAPHIC_BITMAP_EXCEPTION(NOMIC_GRAPHIC_BITMAP_EXCEPTION_UNLOADED);
			}

			result = m_surface->w;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}
	}
}
