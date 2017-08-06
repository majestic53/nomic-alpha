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

#include "../../include/entity/sun.h"
#include "../../include/entity/camera.h"
#include "../../include/runtime.h"
#include "../../include/trace.h"
#include "./sun_type.h"

namespace nomic {

	namespace entity {

		#define SUN_SEGMENT_COUNT 6
		#define SUN_SEGMENT_WIDTH_COLOR 4
		#define SUN_SEGMENT_WIDTH_COORDINATE 2
		#define SUN_SEGMENT_WIDTH_VERTEX 3

		enum {
			SUN_INDEX_COLOR = 0,
			SUN_INDEX_COORDINATE,
			SUN_INDEX_VERTEX,
		};

		static const glm::vec2 SUN_COORDINATE[] = {
			{ 1.f, 1.f, }, // bottom left
			{ 1.f, 0.f, }, // top left
			{ 0.f, 0.f, }, // top right
			{ 1.f, 1.f, }, // bottom left
			{ 0.f, 0.f, }, // top right
			{ 0.f, 1.f, }, // bottom right
			};

		static const glm::vec3 SUN_VERTEX[] = {
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS, }, // bottom left corner
			{ -BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS, }, // top left corner
			{ BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS, }, // top right corner
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS, }, // bottom left corner
			{ BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS, }, // top right corner
			{ BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS, }, // bottom right corner
			};

		sun::sun(
			__in_opt float delta,
			__in_opt bool cycle
			) :
				nomic::entity::object(ENTITY_SUN),
				m_cycle(SUN_CYCLE_DEFAULT),
				m_delta(SUN_DELTA_DEFAULT),
				m_radius(SUN_RADIUS_DEFAULT)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f, Mode=%s", delta, cycle ? "Cycling" : "Fixed");

			set_cycle(cycle);
			set_delta(delta);
			setup();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		sun::sun(
			__in const sun &other
			) :
				nomic::entity::object(other),
				nomic::graphic::texture(other),
				m_color(other.m_color),
				m_color_background(other.m_color_background),
				m_cycle(other.m_cycle),
				m_delta(other.m_delta),
				m_radius(other.m_radius)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		sun::~sun(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		sun &
		sun::operator=(
			__in const sun &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				nomic::graphic::texture::operator=(other);
				m_color = other.m_color;
				m_color_background = other.m_color_background;
				m_cycle = other.m_cycle;
				m_delta = other.m_delta;
				m_radius = other.m_radius;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		glm::vec4 
		sun::color(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%f, %f, %f, %f}", m_color.x, m_color.y, m_color.z, m_color.w);
			return m_color;
		}

		glm::vec4 
		sun::color_background(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_color_background.x, m_color_background.y, m_color_background.z,
				m_color_background.w);
			return m_color_background;
		}

		bool 
		sun::cycling(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_cycle);
			return m_cycle;
		}

		float 
		sun::delta(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%f", m_delta);
			return m_delta;
		}

		void 
		sun::on_render(
			__in nomic::core::renderer &renderer,
			__in void *textures,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Textures=%p, Delta=%f", &renderer, textures, delta);

			nomic::graphic::texture::bind();
			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			arr.enable(SUN_INDEX_COLOR);
			arr.enable(SUN_INDEX_COORDINATE);
			arr.enable(SUN_INDEX_VERTEX);
			GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, SUN_SEGMENT_COUNT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		sun::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);

			if(m_cycle && runtime && camera) {
				std::vector<glm::vec4> color;
				float angle, delta = ((nomic::runtime *) runtime)->tick_cycle();

				m_color = SUN_COLOR_APOGEE;
				m_color_background = m_color;
				m_position = ((nomic::entity::camera *) camera)->position();
				m_position.y = 0.f;
				angle = (SUN_ANGLE_BEGIN - (SUN_ANGLE_OFFSET * delta));
				m_position.y += (m_radius * glm::cos(glm::radians(angle)));
				m_position.z -= (m_radius * glm::sin(glm::radians(angle)));

				if(delta <= SUN_RISE) {
					m_color = SUN_COLOR_RISE;
					m_color.y += ((SUN_COLOR_APOGEE.y - SUN_COLOR_RISE.y) * (delta / SUN_RISE));
					m_color.z += ((SUN_COLOR_APOGEE.z - SUN_COLOR_RISE.z) * (delta / SUN_RISE));
					m_color_background = m_color;
				} else if((delta > SUN_RISE) && (delta <= SUN_SET)) {
					m_color = SUN_COLOR_APOGEE;
					m_color_background = m_color;
				} else {
					m_color = SUN_COLOR_APOGEE;
					m_color_background = m_color;
					m_color.y -= ((SUN_COLOR_APOGEE.y - SUN_COLOR_SET.y) * ((delta - SUN_SET) / (1.f - SUN_SET)));
					m_color.z -= ((SUN_COLOR_APOGEE.z - SUN_COLOR_SET.z) * ((delta - SUN_SET) / (1.f - SUN_SET)));
					m_color_background.y -= ((SUN_COLOR_APOGEE.y - SUN_COLOR_SET_SKY.y)
						* ((delta - SUN_SET) / (1.f - SUN_SET)));
					m_color_background.z -= ((SUN_COLOR_APOGEE.z - SUN_COLOR_SET_SKY.z)
						* ((delta - SUN_SET) / (1.f - SUN_SET)));
				}

				for(uint32_t iter = 0; iter < SUN_SEGMENT_COUNT; ++iter) {
					color.push_back(m_color);
				}

				nomic::graphic::vao &arr = vertex_array();
				arr.bind();
				arr.set_subdata(SUN_INDEX_COLOR, 0, SUN_SEGMENT_COUNT * SUN_SEGMENT_WIDTH_COLOR * sizeof(GLfloat),
					&color[0]);
				arr.enable(SUN_INDEX_COLOR);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		sun::set_cycle(
			__in bool cycle
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Cycle=%x", cycle);

			m_cycle = cycle;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		sun::set_delta(
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f", delta);

			if((delta < SUN_DELTA_MIN) || (delta > SUN_DELTA_MAX)) {
				THROW_NOMIC_ENTITY_SUN_EXCEPTION_FORMAT(NOMIC_ENTITY_SUN_EXCEPTION_DELTA_INVALID,
					"Delta=%f (must be within range of %f <= Delta <= %f)", delta, SUN_DELTA_MIN, SUN_DELTA_MAX);
			}

			m_delta = delta;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		sun::setup(void)
		{
			std::vector<glm::vec3> vertex;
			glm::vec3 scale = glm::vec3(SUN_SCALE_DEFAULT, SUN_SCALE_DEFAULT, SUN_SCALE_DEFAULT);

			TRACE_ENTRY(LEVEL_VERBOSE);

			for(uint32_t iter = 0; iter < SUN_SEGMENT_COUNT; ++iter) {
				vertex.push_back(SUN_VERTEX[iter] * scale);
			}

			nomic::graphic::texture::set(SUN_PATH_DEFAULT);
			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, SUN_SEGMENT_COUNT * SUN_SEGMENT_WIDTH_COLOR * sizeof(GLfloat),
				GL_DYNAMIC_DRAW), SUN_INDEX_COLOR, SUN_SEGMENT_WIDTH_COLOR, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &SUN_COORDINATE[0],
				((uint8_t *) &SUN_COORDINATE[0]) + (SUN_SEGMENT_COUNT * SUN_SEGMENT_WIDTH_COORDINATE * sizeof(GLfloat))),
				GL_STATIC_DRAW), SUN_INDEX_COORDINATE, SUN_SEGMENT_WIDTH_COORDINATE, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &vertex[0],
				((uint8_t *) &vertex[0]) + (SUN_SEGMENT_COUNT * SUN_SEGMENT_WIDTH_VERTEX * sizeof(GLfloat))),
				GL_STATIC_DRAW), SUN_INDEX_VERTEX, SUN_SEGMENT_WIDTH_VERTEX, GL_FLOAT);
			arr.enable(SUN_INDEX_COLOR);
			arr.enable(SUN_INDEX_COORDINATE);
			arr.enable(SUN_INDEX_VERTEX);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		sun::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_SUN_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Texture=" << nomic::graphic::texture::to_string(verbose)
					<< ", Mode=" << (m_cycle ? "Cycling" : "Fixed")
					<< ", Color={" << m_color.x << ", " << m_color.y << ", " << m_color.z << ", " << m_color.w << "}"
					<< " (Background={" << m_color_background.x << ", " << m_color_background.y << ", " << m_color_background.z
						<< ", " << m_color_background.w << "})"
					<< ", Delta=" << m_delta
					<< ", Radius=" << m_radius;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
