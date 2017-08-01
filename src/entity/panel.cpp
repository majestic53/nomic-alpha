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

		#define PANEL_SEGMENT_COUNT 6
		#define PANEL_SEGMENT_WIDTH_COORDINATE 2
		#define PANEL_SEGMENT_WIDTH_VERTEX 3

		enum {
			PANEL_ATLAS_LEFT_UNSELECTED = 0,
			PANEL_ATLAS_CENTER_UNSELECTED,
			PANEL_ATLAS_RIGHT_UNSELECTED,
			PANEL_ATLAS_LEFT_SELECTED,
			PANEL_ATLAS_CENTER_SELECTED,
			PANEL_ATLAS_RIGHT_SELECTED,
		};

		enum {
			PANEL_INDEX_COORDINATE,
			PANEL_INDEX_VERTEX,
		};

		enum {
			VAO_TYPE = 0,
			VAO_BASE,
			VAO_OFFSET,
			VAO_BASE_TEXTURE,
			VAO_OFFSET_TEXTURE,
		};

		static const glm::vec2 PANEL_COORDINATE[] = {
			{ 1.f, 1.f, }, // bottom left
			{ 1.f, 0.f, }, // top left
			{ 0.f, 0.f, }, // top right
			{ 1.f, 1.f, }, // bottom left
			{ 0.f, 0.f, }, // top right
			{ 0.f, 1.f, }, // bottom right
			};

		static const glm::vec3 PANEL_VERTEX[] = {
			{ 0.f, 0.f, 0.f, }, // bottom left corner
			{ 0.f, 1.f, 0.f, }, // top left corner
			{ 1.f, 1.f, 0.f, }, // top right corner
			{ 0.f, 0.f, 0.f, }, // bottom left corner
			{ 1.f, 1.f, 0.f, }, // top right corner
			{ 1.f, 0.f, 0.f, }, // bottom right corner
			};

		panel::panel(
			__in const std::vector<uint8_t> &selection
			) :
				nomic::entity::object(ENTITY_PANEL),
				m_atlas(PANEL_ATLAS_PATH_DEFAULT, PANEL_ATLAS_DIMENSIONS_DEFAULT, PANEL_ATLAS_WIDTH_DEFAULT),
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
				m_atlas(other.m_atlas),
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
				m_atlas = other.m_atlas;
				m_index = 0;
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
#ifdef PANEL_LOOPING
			else {
				m_index = (m_selection.size() - 1);
			}
#endif // PANEL_LOOPING

			result = std::get<VAO_TYPE>(m_selection.at(m_index));

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
#ifdef PANEL_LOOPING
			else {
				m_index = 0;
			}
#endif // PANEL_LOOPING

			result = std::get<VAO_TYPE>(m_selection.at(m_index));

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

			if(!m_selection.empty()) {
				nomic::graphic::vao &arr = vertex_array();
				arr.bind();
				arr.enable(PANEL_INDEX_COORDINATE);
				arr.enable(PANEL_INDEX_VERTEX);

				if(textures) {
					uint32_t count = 0;
					nomic::graphic::atlas *texture_ref = (nomic::graphic::atlas *) textures;

					for(std::vector<panel_data>::iterator iter = m_selection.begin(); iter != m_selection.end();
							++count, ++iter) {
						uint32_t type = PANEL_ATLAS_CENTER_UNSELECTED;

						if(m_selection.size() > 1) {

							if(iter == m_selection.begin()) {
								type = PANEL_ATLAS_RIGHT_UNSELECTED;
							} else if(iter == (m_selection.end() - 1)) {
								type = PANEL_ATLAS_LEFT_UNSELECTED;
							}
						}

						if(count == m_index) {
							type += PANEL_ATLAS_LEFT_SELECTED;
						}

						m_atlas.enable(type);
						GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, std::get<VAO_BASE>(*iter),
							std::get<VAO_OFFSET>(*iter));
					}

					for(std::vector<panel_data>::iterator iter = m_selection.begin(); iter != m_selection.end();
							++count, ++iter) {
						texture_ref->enable(std::get<VAO_TYPE>(*iter));
						GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, std::get<VAO_BASE_TEXTURE>(*iter),
							std::get<VAO_OFFSET_TEXTURE>(*iter));
					}
				}
			}

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

			result = std::get<VAO_TYPE>(m_selection.at(m_index));

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

			m_selection.clear();

			for(std::vector<uint8_t>::const_iterator iter = selection.begin(); iter != selection.end(); ++iter) {
				m_selection.push_back(std::make_tuple(*iter, 0, 0, 0, 0));
			}

			setup_vertex_array();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		panel::setup(
			__in const std::vector<panel_data> &selection
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
			setup_vertex_array();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		panel::setup_vertex_array(void)
		{
			float ratio;
			uint32_t base = 0;
			std::vector<glm::vec3> vertex;
			std::vector<glm::vec2> coordinate;

			TRACE_ENTRY(LEVEL_VERBOSE);

			ratio = (m_view_dimensions.y / (float) m_view_dimensions.x);

			for(uint32_t iter = 0; iter < m_selection.size(); ++iter) {
				coordinate.insert(coordinate.end(), &PANEL_COORDINATE[0], &PANEL_COORDINATE[0] + PANEL_SEGMENT_COUNT);

				for(uint32_t iter_segment = 0; iter_segment < PANEL_SEGMENT_COUNT; ++iter_segment) {
					vertex.push_back(PANEL_VERTEX[iter_segment] * glm::vec3(PANEL_WIDTH * ratio, PANEL_WIDTH, 0.f));

					if(iter) {
						vertex.back() += glm::vec3(iter * ratio * PANEL_WIDTH, 0.f, 0.f);
					}

					vertex.back() -= glm::vec3((m_selection.size() * ratio * PANEL_WIDTH) / 2.f,
						1.f - PANEL_PADDING_LOWER, 0.f);
				}

				nomic::entity::panel_data &data = m_selection.at(iter);
				std::get<VAO_BASE>(data) = base;
				std::get<VAO_OFFSET>(data) = PANEL_SEGMENT_COUNT;
				base += PANEL_SEGMENT_COUNT;
			}

			for(uint32_t iter = 0; iter < m_selection.size(); ++iter) {
				coordinate.insert(coordinate.end(), &PANEL_COORDINATE[0], &PANEL_COORDINATE[0] + PANEL_SEGMENT_COUNT);

				for(uint32_t iter_segment = 0; iter_segment < PANEL_SEGMENT_COUNT; ++iter_segment) {
					vertex.push_back(PANEL_VERTEX[iter_segment] * glm::vec3(PANEL_WIDTH_TEXTURE * ratio, PANEL_WIDTH_TEXTURE, 0.f));

					if(iter) {
						vertex.back() += glm::vec3(iter * ratio * PANEL_WIDTH, 0.f, 0.f);
					}

					vertex.back() -= glm::vec3(((m_selection.size() * ratio * PANEL_WIDTH) / 2.f)
						- ((PANEL_WIDTH * ratio) / 2.f) + ((PANEL_WIDTH_TEXTURE * ratio) / 2.f),
						(1.f - PANEL_PADDING_LOWER) - (PANEL_WIDTH / 2.f) + (PANEL_WIDTH_TEXTURE / 2.f), 0.f);
				}

				nomic::entity::panel_data &data = m_selection.at(iter);
				std::get<VAO_BASE_TEXTURE>(data) = base;
				std::get<VAO_OFFSET_TEXTURE>(data) = PANEL_SEGMENT_COUNT;
				base += PANEL_SEGMENT_COUNT;
			}

			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &coordinate[0],
				((uint8_t *) &coordinate[0]) + (coordinate.size() * PANEL_SEGMENT_WIDTH_COORDINATE * sizeof(GLfloat))),
				GL_STATIC_DRAW), PANEL_INDEX_COORDINATE, PANEL_SEGMENT_WIDTH_COORDINATE, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &vertex[0],
				((uint8_t *) &vertex[0]) + (vertex.size() * PANEL_SEGMENT_WIDTH_VERTEX * sizeof(GLfloat))),
				GL_STATIC_DRAW), PANEL_INDEX_VERTEX, PANEL_SEGMENT_WIDTH_VERTEX, GL_FLOAT);

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
					<< ", Atlas=" << m_atlas.to_string(verbose)
					<< ", Selection[" << m_selection.size() << "]";

				if(!m_selection.empty()) {
					uint8_t count = 0;
					result << "={";

					for(std::vector<panel_data>::const_iterator iter = m_selection.begin();
							iter != m_selection.end(); ++count, ++iter) {

						if(iter != m_selection.begin()) {
							result << ", ";
						}

						if(count == m_index) {
							result << "[";
						}

						result << SCALAR_AS_HEX(uint8_t, std::get<VAO_TYPE>(*iter))
							<< ", {" << std::get<VAO_BASE>(*iter) << ", " << std::get<VAO_OFFSET>(*iter) << "}";

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
