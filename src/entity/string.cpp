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

#include "../../include/entity/string.h"
#include "../../include/entity/camera.h"
#include "../../include/runtime.h"
#include "../../include/trace.h"
#include "./string_type.h"

namespace nomic {

	namespace entity {

		#define STRING_SEGMENT_COUNT 6
		#define STRING_SEGMENT_WIDTH 4

		enum {
			STRING_INDEX_VERTEX = 0,
		};

		string::string(
			__in_opt const std::string &path,
			__in_opt uint32_t size,
			__in_opt const glm::uvec2 &position,
			__in_opt const glm::vec4 &color,
			__in_opt float scale
			) :
				nomic::entity::object(ENTITY_STRING),
				m_color(color),
				m_font_id(0),
				m_manager_font(nomic::font::manager::acquire()),
				m_path(path),
				m_position(position),
				m_scale(scale),
				m_size(size),
				m_text(STRING_EMPTY)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s, Size=%u, Position={%u, %u}, Color={%f, %f, %f, %f}, Scale=%f", path.size(),
				STRING_CHECK(path), size, position.x, position.y, color.x, color.y, color.z, color.w, scale);

			setup();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		string::string(
			__in const string &other
			) :
				nomic::entity::object(other),
				m_color(other.m_color),
				m_font_id(other.m_font_id),
				m_manager_font(nomic::font::manager::acquire()),
				m_path(other.m_path),
				m_position(other.m_position),
				m_scale(other.m_scale),
				m_size(other.m_size),
				m_text(other.m_text)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		string::~string(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			teardown();
			m_manager_font.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		string &
		string::operator=(
			__in const string &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				teardown();
				nomic::entity::object::operator=(other);
				m_color = other.m_color;
				m_font_id = other.m_font_id;
				m_path = other.m_path;
				m_position = other.m_position;
				m_scale = other.m_scale;
				m_size = other.m_size;
				m_text = other.m_text;
				setup();
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		string::on_render(
			__in nomic::core::renderer &renderer,
			__in float delta
			)
		{
			float x, y;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Delta=%f", &renderer, delta);

			x = m_position.x;
			y = (m_view_dimensions.y - m_position.y);
			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			GL_CHECK(LEVEL_WARNING, glActiveTexture, GL_TEXTURE0);
			renderer.set_uniform(renderer.uniform_location(UNIFORM_COLOR), m_color);

			for(std::string::iterator iter = m_text.begin(); iter != m_text.end(); ++iter) {
				nomic::graphic::character &ch = m_manager_font.character(m_font_id, *iter);
				float xpos = x + (ch.bearing().x * m_scale), ypos = y - ((ch.dimensions().y - ch.bearing().y) * m_scale),
					w = (ch.dimensions().x * m_scale), h = (ch.dimensions().y * m_scale),
					vertices[STRING_SEGMENT_COUNT][STRING_SEGMENT_WIDTH] = {
						{ xpos, ypos + h, 0.f, 0.f }, { xpos, ypos, 0.f, 1.f },
						{ xpos + w, ypos, 1.f, 1.f }, { xpos, ypos + h, 0.f, 0.f },
						{ xpos + w, ypos, 1.f, 1.f }, { xpos + w, ypos + h, 1.f, 0.f },
						};

				ch.bind();
				arr.set_subdata(STRING_INDEX_VERTEX, 0, sizeof(vertices), vertices);

				if(std::isprint(*iter)) {
					GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, STRING_SEGMENT_COUNT);
				}

				if(*iter == CHARACTER_NEWLINE) {
					x = m_position.x;
					y -= ((ch.dimensions().y * m_scale) + STRING_VERTICAL_PAD);
				} else {
					x += ((ch.advance() >> STRING_ADVANCE_SHIFT) * m_scale);
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		string::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		string::on_view_change(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			setup();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		glm::uvec2 
		string::position(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Position={%u, %u}", m_position.x, m_position.y);
			return m_position;
		}

		float 
		string::scale(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Scale=%f", m_scale);
			return m_scale;
		}

		void 
		string::set_color(
			__in const glm::vec4 &color
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Color={%f, %f, %f, %f}", color.x, color.y, color.z, color.w);

			m_color = color;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		string::set_font(
			__in const std::string &path,
			__in_opt uint32_t size
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s, Size=%u", path.size(), STRING_CHECK(path), size);

			teardown();
			m_path = path;
			m_size = size;
			setup();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		string::set_position(
			__in const glm::uvec2 &position
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u}", m_position.x, m_position.y);

			m_position = position;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		string::set_scale(
			__in float scale
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Scale=%f", m_scale);

			m_scale = scale;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		string::setup(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_font_id) {

				if(m_manager_font.initialized()) {
					m_font_id = m_manager_font.load(m_path, m_size);
				}
			}

			nomic::graphic::vao &arr = vertex_array();
			arr.disable_all();
			arr.remove_all();
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, STRING_SEGMENT_COUNT * STRING_SEGMENT_WIDTH * sizeof(GLfloat), GL_DYNAMIC_DRAW),
				STRING_INDEX_VERTEX, STRING_SEGMENT_WIDTH, GL_FLOAT);
			arr.enable(STRING_INDEX_VERTEX);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		string::teardown(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(m_font_id) {

				if(m_manager_font.initialized() && m_manager_font.loaded(m_font_id)) {
					m_manager_font.unload(m_font_id);
				}

				m_font_id = 0;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		string::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_STRING_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Path[" << m_path.size() << "]";

				if(!m_path.empty()) {
					result << "=" << m_path;
				}

				result << ", Size=" << m_size
					<< ", Position={" << m_position.x << ", " << m_position.y << "}"
					<< ", Font=" << SCALAR_AS_HEX(uint32_t, m_font_id)
					<< ", Text[" << m_text.size() << "]=" << STRING_CHECK(m_text)
					<< ", Color={" << m_color.x << ", " << m_color.y << ", " << m_color.z << ", " << m_color.w << "}"
					<< ", Scale=" << m_scale;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
