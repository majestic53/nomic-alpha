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

#include "../../include/core/font.h"
#include "../../include/uuid/manager.h"
#include "../../include/trace.h"
#include "./font_type.h"

namespace nomic {

	namespace core {

		font::font(
			__in_opt FT_Library parent,
			__in_opt const std::string &path,
			__in_opt uint32_t size
			) :
				m_face(nullptr),
				m_size(FONT_SIZE_DEFAULT)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Parent=%p, Path[%u]=%s, Size=%u", parent, path.size(), STRING_CHECK(path), size);

			if(!path.empty()) {
				load(parent, path, size);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		font::font(
			__in const font &other
			) :
				nomic::core::id(other),
				m_face(other.m_face),
				m_size(other.m_size)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		font::~font(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			unload();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		font &
		font::operator=(
			__in const font &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::core::id::operator=(other);
				m_face = other.m_face;
				m_size = other.m_size;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		font::generate(
			__inout nomic::graphic::character &output,
			__in char value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Output=%p, Value=%x(\'%c\')", &output, (int) value,
				std::isprint(value) ? value : CHARACTER_FILL);

			if(!m_face) {
				THROW_NOMIC_CORE_FONT_EXCEPTION(NOMIC_CORE_FONT_EXCEPTION_UNLOADED);
			}

			FT_Error result = FT_Load_Char(m_face, value, FT_LOAD_RENDER);
			if(result) {
				THROW_NOMIC_CORE_FONT_EXCEPTION_FORMAT(NOMIC_CORE_FONT_EXCEPTION_EXTERNAL,
					"FT_Load_Char failed! Error=%x, Value=%x(\'%c\')", result, (int) value,
					std::isprint(value) ? value : CHARACTER_FILL);
			}

			output.set(m_face);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		font::load(
			__in FT_Library parent,
			__in const std::string &path,
			__in_opt uint32_t size
			)
		{
			FT_Error result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Parent=%p, Path[%u]=%s, Size=%u", parent, path.size(), STRING_CHECK(path), size);

			unload();

			result = FT_New_Face(parent, path.c_str(), 0, &m_face);
			if(result) {
				THROW_NOMIC_CORE_FONT_EXCEPTION_FORMAT(NOMIC_CORE_FONT_EXCEPTION_EXTERNAL, "FT_New_Face failed! Error=%x, Path[%u]=%s",
					result, path.size(), STRING_CHECK(path));
			}

			result = FT_Set_Pixel_Sizes(m_face, 0, size);
			if(result) {
				THROW_NOMIC_CORE_FONT_EXCEPTION_FORMAT(NOMIC_CORE_FONT_EXCEPTION_EXTERNAL,
					"FT_Set_Pixel_Sizes failed! Error=%x, Size=%u", result, size);
			}

			m_size = size;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		font::loaded(void) const
		{
			bool result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = (m_face != nullptr);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		uint32_t 
		font::size(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_face) {
				THROW_NOMIC_CORE_FONT_EXCEPTION(NOMIC_CORE_FONT_EXCEPTION_UNLOADED);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", m_size);
			return m_size;
		}

		std::string 
		font::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_CORE_FONT_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::core::id::to_string(verbose)
					<< ", State=" << (m_face ? "Loaded" : "Unloaded");

				if(m_face) {
					result << ", Face=" << SCALAR_AS_HEX(FT_Face, m_face)
						<< ", Size=" << m_size;
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		font::unload(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(m_face) {
				size_t references = REFERENCE_INIT;

				nomic::uuid::manager &instance = nomic::uuid::manager::acquire();
				if(instance.initialized() && instance.contains(m_id)) {
					references = instance.references(m_id);
				}

				if(references <= REFERENCE_INIT) {

					FT_Error result = FT_Done_Face(m_face);
					if(result) {
						THROW_NOMIC_CORE_FONT_EXCEPTION_FORMAT(NOMIC_CORE_FONT_EXCEPTION_EXTERNAL,
							"FT_Done_Face failed! Error=%x", result);
					}
				}

				m_face = nullptr;
				m_size = FONT_SIZE_DEFAULT;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
