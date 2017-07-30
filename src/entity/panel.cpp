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

#include "../../include/entity/panel.h"
#include "../../include/trace.h"
#include "./panel_type.h"

namespace nomic {

	namespace entity {

		panel::panel(
			__in const std::vector<uint8_t> &selection
			) :
				nomic::entity::object(ENTITY_PANEL),
				m_index(0)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Selection[%u]=%p", selection.size(), &selection);

			setup(selection);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		panel::panel(
			__in const panel &other
			) :
				nomic::entity::object(other),
				m_index(0)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			setup(other.m_selection);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		panel::~panel(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		panel &
		panel::operator=(
			__in const panel &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				setup(other.m_selection);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		uint8_t 
		panel::index(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_index);
			return m_index;
		}

		uint8_t 
		panel::move_left(void)
		{
			uint8_t previous, result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			previous = m_index;

			if(m_index > 0) {
				--m_index;
			}

			result = m_selection.at(m_index);

			if(m_index != previous) {
				reconfigure();
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		uint8_t 
		panel::move_right(void)
		{
			uint8_t previous, result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			previous = m_index;

			if(m_index < (m_selection.size() - 1)) {
				++m_index;
			}

			result = m_selection.at(m_index);

			if(m_index != previous) {
				reconfigure();
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		panel::on_render(
			__in nomic::core::renderer &renderer,
			__in void *textures,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Textures=%p, Delta=%f", &renderer, textures, delta);

			// TODO: render panel

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		panel::reconfigure(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			// TODO: rebuild panel

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint8_t 
		panel::selected(void)
		{
			uint8_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			if(m_index >= m_selection.size()) {
				THROW_NOMIC_ENTITY_PANEL_EXCEPTION_FORMAT(NOMIC_ENTITY_PANEL_EXCEPTION_INDEX_INVALID,
					"Index=%u", m_index);
			}

			result = m_selection.at(m_index);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		panel::set_selected(
			__in uint8_t index
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Index=%u", index);

			if(m_index >= m_selection.size()) {
				THROW_NOMIC_ENTITY_PANEL_EXCEPTION_FORMAT(NOMIC_ENTITY_PANEL_EXCEPTION_INDEX_INVALID,
					"Index=%u", index);
			}

			m_index = index;
			reconfigure();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		panel::setup(
			__in const std::vector<uint8_t> &selection
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Selection[%u]=%p", selection.size(), &selection);

			if(selection.empty()) {
				THROW_NOMIC_ENTITY_PANEL_EXCEPTION(NOMIC_ENTITY_PANEL_EXCEPTION_EMPTY);
			}

			if(selection.size() > PANEL_INDEX_MAX) {
				THROW_NOMIC_ENTITY_PANEL_EXCEPTION_FORMAT(NOMIC_ENTITY_PANEL_EXCEPTION_FULL,
					"Selection[%u] (can be at most %u)", selection.size(), PANEL_INDEX_MAX);
			}

			m_selection = selection;

			// TODO: build panel and push into vao

			m_index = PANEL_INDEX_DEFAULT;
			set_selected(m_index);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		panel::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_PANEL_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Selection[" << m_selection.size() << "]";

				if(!m_selection.empty()) {
					uint8_t count = 0;
					result << "={";

					for(std::vector<uint8_t>::const_iterator iter = m_selection.begin(); iter != m_selection.end();
							++count, ++iter) {

						if(iter != m_selection.begin()) {
							result << ", ";
						}

						if(count == m_index) {
							result << "[";
						}

						result << SCALAR_AS_HEX(uint8_t, *iter);

						if(count == m_index) {
							result << "]";
						}
					}

					result << "}";
				}

				result << ", Index=" << m_index;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
