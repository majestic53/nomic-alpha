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

#include "../../include/entity/selector.h"
#include "../../include/trace.h"
#include "./selector_type.h"

namespace nomic {

	namespace entity {

		#define SELECTOR_SEGMENT_COUNT 8
		#define SELECTOR_SEGMENT_COUNT_COLOR 6
		#define SELECTOR_SEGMENT_WIDTH_COLOR 4
		#define SELECTOR_SEGMENT_WIDTH_VERTEX 3

		enum {
			SELECTOR_INDEX_COLOR = 0,
			SELECTOR_INDEX_VERTEX,
		};

		static const glm::vec3 SELECTOR_VERTEX[] = {
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f }, // right
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, 0.f }, // left
			{ 0.f, 0.f, 0.f },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, 0.f, 0.f },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f }, // top
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, 0.f }, // bottom
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ 0.f, 0.f, 0.f },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ 0.f, 0.f, 0.f },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH }, // back
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f }, // front
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ 0.f, 0.f, 0.f },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ 0.f, 0.f, 0.f },
			};

		static const glm::vec3 SELECTOR_VERTEX_COLOR[] = {
			{ BLOCK_WIDTH, 0.f, 0.f, }, // right
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH, },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f, },
			{ BLOCK_WIDTH, 0.f, 0.f, },
			{ 0.f, 0.f, BLOCK_WIDTH, }, // left
			{ 0.f, 0.f, 0.f, },
			{ 0.f, BLOCK_WIDTH, 0.f, },
			{ 0.f, BLOCK_WIDTH, 0.f, },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH, },
			{ 0.f, 0.f, BLOCK_WIDTH, },
			{ 0.f, BLOCK_WIDTH, 0.f, }, // top
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f, },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH, },
			{ 0.f, BLOCK_WIDTH, 0.f, },
			{ 0.f, 0.f, 0.f, }, // bottom
			{ 0.f, 0.f, BLOCK_WIDTH, },
			{ BLOCK_WIDTH, 0.f, 0.f, },
			{ BLOCK_WIDTH, 0.f, 0.f, },
			{ 0.f, 0.f, BLOCK_WIDTH, },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH, },
			{ 0.f, 0.f, BLOCK_WIDTH, }, // back
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH, },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH, },
			{ 0.f, 0.f, BLOCK_WIDTH, },
			{ 0.f, BLOCK_WIDTH, 0.f, }, // front
			{ 0.f, 0.f, 0.f, },
			{ BLOCK_WIDTH, 0.f, 0.f, },
			{ BLOCK_WIDTH, 0.f, 0.f, },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f, },
			{ 0.f, BLOCK_WIDTH, 0.f, },
			};

		selector::selector(
			__in_opt const glm::vec4 &color,
			__in_opt float scale,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up
			) :
				nomic::entity::object(ENTITY_SELECTOR, SUBTYPE_UNDEFINED, position, rotation, up),
				m_color(color),
				m_face(BLOCK_FACE_TOP),
				m_scale(scale),
				m_show_bounds(true)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,
				"Color={%f, %f, %f, %f}, Scale=%u, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}",
				color.x, color.y, color.z, color.w, scale, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z,
				up.x, up.y, up.z);

			setup();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		selector::selector(
			__in const selector &other
			) :
				nomic::entity::object(other),
				m_color(other.m_color),
				m_face(other.m_face),
				m_scale(other.m_scale),
				m_show_bounds(other.m_show_bounds)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			setup();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		selector::~selector(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		selector &
		selector::operator=(
			__in const selector &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				m_color = other.m_color;
				m_face = other.m_face;
				m_scale = other.m_scale;
				m_show_bounds = other.m_show_bounds;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		selector::on_render(
			__in nomic::core::renderer &renderer,
			__in void *textures,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Textures=%p, Delta=%f", &renderer, textures, delta);

			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, SELECTOR_SEGMENT_COUNT, SELECTOR_SEGMENT_COUNT_COLOR);

#ifdef SELECTOR_SHOW_BOUNDS
			if(m_show_bounds) {
				GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_LINES, 0, SELECTOR_SEGMENT_COUNT);
			}
#endif // SELECTOR_SHOW_BOUNDS

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		selector::reconfigure(void)
		{
			std::vector<glm::vec4> color;
			std::vector<glm::vec3> vertex;
			glm::vec3 scale(m_scale, m_scale, m_scale);
			glm::vec4 color_inner = glm::vec4(m_color.x, m_color.y, m_color.z, SELECTOR_COLOR_ALPHA);

			TRACE_ENTRY(LEVEL_VERBOSE);

			for(uint32_t iter = 0; iter < SELECTOR_SEGMENT_COUNT; ++iter) {
				color.push_back(m_color);
				vertex.push_back(SELECTOR_VERTEX[(m_face * SELECTOR_SEGMENT_COUNT) + iter] * scale);
			}

			for(uint32_t iter = 0; iter < SELECTOR_SEGMENT_COUNT_COLOR; ++iter) {
				color.push_back(color_inner);
				vertex.push_back(SELECTOR_VERTEX_COLOR[(m_face * SELECTOR_SEGMENT_COUNT_COLOR) + iter] * scale);
			}

			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			arr.set_subdata(SELECTOR_INDEX_COLOR, 0, SELECTOR_SEGMENT_COUNT * SELECTOR_SEGMENT_WIDTH_COLOR
				* SELECTOR_SEGMENT_COUNT_COLOR * SELECTOR_SEGMENT_WIDTH_COLOR * sizeof(GLfloat),
				&color[0]);
			arr.set_subdata(SELECTOR_INDEX_VERTEX, 0, SELECTOR_SEGMENT_COUNT * SELECTOR_SEGMENT_WIDTH_VERTEX
				* SELECTOR_SEGMENT_COUNT_COLOR * SELECTOR_SEGMENT_WIDTH_VERTEX * sizeof(GLfloat),
				&vertex[0]);
			arr.enable(SELECTOR_INDEX_COLOR);
			arr.enable(SELECTOR_INDEX_VERTEX);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		selector::setup(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, SELECTOR_SEGMENT_COUNT * SELECTOR_SEGMENT_WIDTH_COLOR
				* SELECTOR_SEGMENT_COUNT_COLOR * SELECTOR_SEGMENT_WIDTH_COLOR * sizeof(GLfloat),
				GL_DYNAMIC_DRAW), SELECTOR_INDEX_COLOR, SELECTOR_SEGMENT_WIDTH_COLOR, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, SELECTOR_SEGMENT_COUNT * SELECTOR_SEGMENT_WIDTH_VERTEX
				* SELECTOR_SEGMENT_COUNT_COLOR * SELECTOR_SEGMENT_WIDTH_VERTEX * sizeof(GLfloat),
				GL_DYNAMIC_DRAW), SELECTOR_INDEX_VERTEX, SELECTOR_SEGMENT_WIDTH_VERTEX, GL_FLOAT);
			arr.enable(SELECTOR_INDEX_COLOR);
			arr.enable(SELECTOR_INDEX_VERTEX);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		selector::set_color(
			__in const glm::vec4 &color
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Color={%f, %f, %f, %f}", color.x, color.y, color.z, color.w);

			m_color = color;
			reconfigure();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		selector::set_face(
			__in uint8_t face
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Face=%x(%s)", face, BLOCK_FACE_STRING(face));

			if(((face < BLOCK_FACE_MIN) || (face > BLOCK_FACE_MAX)) && (face != BLOCK_FACE_UNDEFINED)) {
				THROW_NOMIC_ENTITY_SELECTOR_EXCEPTION_FORMAT(NOMIC_ENTITY_SELECTOR_EXCEPTION_FACE_INVALID,
					"Face=%x", face);
			}

			m_face = face;
			if(m_face != BLOCK_FACE_UNDEFINED) {
				reconfigure();
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		selector::set_scale(
			__in float scale
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Scale=%f", scale);

			m_scale = scale;
			reconfigure();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		selector::show_bounds(
			__in bool show
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Show=%x", show);

			m_show_bounds = show;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		selector::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_SELECTOR_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Color={" << m_color.x << ", " << m_color.y << ", " << m_color.z << ", " << m_color.w << "}"
					<< ", Face=" << SCALAR_AS_HEX(uint8_t, m_face) << "(" << BLOCK_FACE_STRING(m_face) << ")"
					<< ", Scale=" << m_scale
					<< ", State=" << (m_show_bounds ? "Bounds" : "No-Bounds");
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
