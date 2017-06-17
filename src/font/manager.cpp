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

#include "../../include/font/manager.h"
#include "../../include/trace.h"
#include "./manager_type.h"

namespace nomic {

	namespace font {

		manager::manager(void) :
			m_handle(nullptr)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		manager::~manager(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		nomic::graphic::character &
		manager::character(
			__in uint32_t id,
			__in GLchar value
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x, Value=%x(\'%c\')", id, value, std::isprint(value) ? value : CHARACTER_FILL);
			TRACE_EXIT(LEVEL_VERBOSE);
			return find_character(find_font(id), value)->second;
		}

		nomic::font::character_set::iterator 
		manager::find_character(
			__in std::map<uint32_t, nomic::font::context>::iterator iter,
			__in GLchar value
			)
		{
			nomic::font::character_set::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Iter=%p, Value=%x(\'%c\')", iter, (int) value, std::isprint(value) ? value : CHARACTER_FILL);

			result = iter->second.second.find(value);
			if(result == iter->second.second.end()) {
				THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_CHARACTER_NOT_FOUND, "Value=%x(\'%c\')",
					value, std::isprint(value) ? value : CHARACTER_FILL);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		std::map<uint32_t, nomic::font::context>::iterator 
		manager::find_font(
			__in uint32_t id
			)
		{
			std::map<uint32_t, nomic::font::context>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(id == UID_INVALID) {
				THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_FONT_INVALID, "Id=%x", id);
			}

			result = m_font.find(id);
			if(result == m_font.end()) {
				THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_FONT_NOT_FOUND, "Id=%x", id);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		uint32_t 
		manager::load(
			__in const std::string &path,
			__in uint32_t size
			)
		{
			uint32_t result;
			std::map<uint32_t, nomic::font::context>::iterator iter_cont;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s, Size=%u", path.size(), STRING_CHECK(path), size);

			if(!m_initialized) {
				THROW_NOMIC_FONT_MANAGER_EXCEPTION(NOMIC_FONT_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			nomic::core::font entry;
			result = entry.get_id();

			if(m_font.find(result) != m_font.end()) {
				THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_FONT_DUPLICATE, "Id=%x", result);
			}

			m_font.insert(std::make_pair(result, std::make_pair(entry, nomic::font::character_set())));
			iter_cont = find_font(result);
			iter_cont->second.first.load(m_handle, path, size);
			GL_CHECK(LEVEL_WARNING, glPixelStorei, GL_UNPACK_ALIGNMENT, 1);

			for(uint16_t value = FONT_CHARACTER_MIN; value <= FONT_CHARACTER_MAX; ++value) {
				iter_cont->second.second.insert(std::make_pair(value, nomic::graphic::character()));

				nomic::font::character_set::iterator iter_set = iter_cont->second.second.find(value);
				if(iter_set == iter_cont->second.second.end()) {
					THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_CHARACTER_NOT_FOUND, "Value=%x(\'%c\')",
						value, std::isprint(value) ? value : CHARACTER_FILL);
				}

				iter_cont->second.first.generate(iter_set->second, value);
			}

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Font loaded. Path[%u]=%s, Size=%u, Id=%x", path.size(), STRING_CHECK(path), size,
				result);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		manager::loaded(
			__in uint32_t id
			) const
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(!m_initialized) {
				THROW_NOMIC_FONT_MANAGER_EXCEPTION(NOMIC_FONT_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(id == UID_INVALID) {
				THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_FONT_INVALID, "Id=%x", id);
			}

			result = (m_font.find(id) != m_font.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		manager::on_initialize(void)
		{
			FT_Error err;
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Font manager initializing...");

			err = FT_Init_FreeType(&m_handle);
			if(err) {
				THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_EXTERNAL, "FT_Init_FreeType failed! Error=%x",
					err);
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Font manager initialized.");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			FT_Error result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Font manager uninitializing...");

			m_font.clear();

			result = FT_Done_FreeType(m_handle);
			if(result) {
				THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_EXTERNAL, "FT_Done_FreeType failed! Error=%x",
					result);
			}

			m_handle = nullptr;

			TRACE_MESSAGE(LEVEL_INFORMATION, "Font manager uninitialized.");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::unload(
			__in uint32_t id
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(m_initialized) {

				if(id == UID_INVALID) {
					THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_FONT_INVALID, "Id=%x", id);
				}

				m_font.erase(find_font(id));
				TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Font unloaded. Id=%x", id);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::unload_all(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(m_initialized) {
				m_font.clear();
				TRACE_MESSAGE(LEVEL_INFORMATION, "All fonts unloaded.");
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		manager::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%p", verbose);

			result << NOMIC_FONT_MANAGER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << SINGLETON_CLASS(nomic::font::manager)::to_string(verbose);

				if(m_initialized) {
					result << ", Handle=" << SCALAR_AS_HEX(FT_Library, m_handle)
						<< ", Font[" << m_font.size() << "]";

					if(!m_font.empty()) {
						result << "={";

						for(std::map<uint32_t, nomic::font::context>::const_iterator iter = m_font.begin();
								iter != m_font.end(); ++iter) {

							if(iter != m_font.begin()) {
								result << ", ";
							}

							result << "{" << SCALAR_AS_HEX(uint32_t, iter->first)
								<< ", " << iter->second.first.to_string(verbose) << "}";
						}

						result << "}";
					}
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
