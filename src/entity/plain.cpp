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

#include "../../include/entity/plain.h"
#include "../../include/trace.h"
#include "./plain_type.h"

namespace nomic {

	namespace entity {

		#define PLAIN_SEGMENT_COUNT 6
		#define PLAIN_SEGMENT_WIDTH_COLOR 4
		#define PLAIN_SEGMENT_WIDTH_COORDINATE 2
		#define PLAIN_SEGMENT_WIDTH_VERTEX 3

		enum {
			PLAIN_INDEX_COLOR = 0,
			PLAIN_INDEX_COORDINATE,
			PLAIN_INDEX_VERTEX,
		};

		static const glm::vec4 PLAIN_COLOR[] = {
			{ PLAIN_COLOR_RGBA_DEFAULT, }, // bottom left corner
			{ PLAIN_COLOR_RGBA_DEFAULT, }, // top left corner
			{ PLAIN_COLOR_RGBA_DEFAULT, }, // top right corner
			{ PLAIN_COLOR_RGBA_DEFAULT, }, // bottom left corner
			{ PLAIN_COLOR_RGBA_DEFAULT, }, // top right corner
			{ PLAIN_COLOR_RGBA_DEFAULT, }, // bottom right corner
			};

		static const glm::vec2 PLAIN_COORDINATE[] = {
			{ 1.f, 1.f, }, // bottom left
			{ 1.f, 0.f, }, // top left
			{ 0.f, 0.f, }, // top right
			{ 1.f, 1.f, }, // bottom left
			{ 0.f, 0.f, }, // top right
			{ 0.f, 1.f, }, // bottom right
			};

		static const glm::vec3 PLAIN_VERTEX[] = {
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS, }, // bottom left corner
			{ -BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS, }, // top left corner
			{ BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS, }, // top right corner
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS, }, // bottom left corner
			{ BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS, }, // top right corner
			{ BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS, }, // bottom right corner
			};

		plain::plain(
			__in_opt const std::string &path,
			__in_opt const glm::vec2 dimensions,
			__in_opt float scale,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up
			) :
				nomic::entity::object(ENTITY_PLAIN, SUBTYPE_UNDEFINED, position, rotation, up),
				m_color(PLAIN_COLOR_DEFAULT),
				m_dimensions(dimensions),
				m_scale(scale),
				m_texture_set(false)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,
				"Path[%u]=%s, Dimension={%f, %f}, Scale=%f, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}",
				path.size(), STRING_CHECK(path), dimensions.x, dimensions.y, scale, position.x, position.y, position.z,
				rotation.x, rotation.y, rotation.z, up.x, up.y, up.z);

			if(!path.empty()) {
				set_texture(path, wrap_s, wrap_t, filter_min, filter_mag);
			} else {
				clear_texture();
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		plain::plain(
			__in const std::vector<uint8_t> &data,
			__in const glm::uvec2 &data_dimensions,
			__in uint32_t data_depth,
			__in_opt const glm::vec2 dimensions,
			__in_opt float scale,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up
			) :
				nomic::entity::object(ENTITY_PLAIN, SUBTYPE_UNDEFINED, position, rotation, up),
				m_color(PLAIN_COLOR_DEFAULT),
				m_dimensions(dimensions),
				m_scale(scale),
				m_texture_set(false)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,
				"Data[%u]=%p, {%u, %u}, %u, Dimension={%f, %f}, Scale=%f, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}",
				data.size(), &data[0], data_dimensions.x, data_dimensions.y, (int) data_depth, dimensions.x, dimensions.y, scale,
				position.x, position.y, position.z, rotation.x, rotation.y, rotation.z, up.x, up.y, up.z);

			set_texture(data, data_dimensions, data_depth, wrap_s, wrap_t, filter_min, filter_mag);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		plain::plain(
			__in const glm::vec4 &color,
			__in_opt const glm::vec2 dimensions,
			__in_opt float scale,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up
			) :
				nomic::entity::object(ENTITY_PLAIN, SUBTYPE_UNDEFINED, position, rotation, up),
				m_color(color),
				m_dimensions(dimensions),
				m_scale(scale),
				m_texture_set(false)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,
				"Color={%f, %f, %f, %f}, Dimension={%f, %f}, Scale=%f, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}",
				color.x, color.y, color.z, color.w, dimensions.x, dimensions.y, scale, position.x, position.y, position.z,
				rotation.x, rotation.y, rotation.z, up.x, up.y, up.z);

			clear_texture();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		plain::plain(
			__in const plain &other
			) :
				nomic::entity::object(other),
				nomic::graphic::texture(other),
				m_color(other.m_color),
				m_dimensions(other.m_dimensions),
				m_scale(other.m_scale),
				m_texture_set(other.m_texture_set)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		plain::~plain(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		plain &
		plain::operator=(
			__in const plain &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				nomic::graphic::texture::operator=(other);
				m_color = other.m_color;
				m_dimensions = other.m_dimensions;
				m_scale = other.m_scale;
				m_texture_set = other.m_texture_set;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		plain::clear_texture(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_texture_set = false;
			reconfigure();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		glm::vec2 
		plain::dimensions(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%f, %f}", m_dimensions.x, m_dimensions.y);
			return m_dimensions;
		}

		void 
		plain::on_render(
			__in nomic::core::renderer &renderer,
			__in void *textures,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Textures=%p, Delta=%f", &renderer, textures, delta);

			if(m_texture_set) {
				nomic::graphic::texture::bind();
			}

			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			arr.enable(PLAIN_INDEX_COLOR);
			arr.enable(PLAIN_INDEX_COORDINATE);
			arr.enable(PLAIN_INDEX_VERTEX);
			GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, PLAIN_SEGMENT_COUNT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		plain::reconfigure(void)
		{
			std::vector<glm::vec4> color;
			std::vector<glm::vec3> vertex;

			TRACE_ENTRY(LEVEL_VERBOSE);

			vertex = std::vector<glm::vec3>((glm::vec3 *) &PLAIN_VERTEX[0],
					(glm::vec3 *) &PLAIN_VERTEX[0] + (sizeof(glm::vec3) * PLAIN_SEGMENT_COUNT));

			for(std::vector<glm::vec3>::iterator iter = vertex.begin(); iter != vertex.end(); ++iter) {
				iter->x *= (m_dimensions.x * m_scale);
				iter->y *= (m_dimensions.y * m_scale);
			}

			color = std::vector<glm::vec4>((glm::vec4 *) &PLAIN_COLOR[0],
					(glm::vec4 *) &PLAIN_COLOR[0] + (sizeof(glm::vec4) * PLAIN_SEGMENT_COUNT));

			for(std::vector<glm::vec4>::iterator iter = color.begin(); iter != color.end(); ++iter) {
				iter->x = m_color.x;
				iter->y = m_color.y;
				iter->z = m_color.z;
				iter->w = m_color.w;
			}

			nomic::graphic::vao &arr = vertex_array();
			arr.disable_all();
			arr.remove_all();
			arr.clear();
			arr.bind();
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &color[0],
				((uint8_t *) &color[0]) + (PLAIN_SEGMENT_COUNT * PLAIN_SEGMENT_WIDTH_COLOR * sizeof(GLfloat))),
				GL_STATIC_DRAW), PLAIN_INDEX_COLOR, PLAIN_SEGMENT_WIDTH_COLOR, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &PLAIN_COORDINATE[0],
				((uint8_t *) &PLAIN_COORDINATE[0]) + (PLAIN_SEGMENT_COUNT * PLAIN_SEGMENT_WIDTH_COORDINATE * sizeof(GLfloat))),
				GL_STATIC_DRAW), PLAIN_INDEX_COORDINATE, PLAIN_SEGMENT_WIDTH_COORDINATE, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &vertex[0],
				((uint8_t *) &vertex[0]) + (PLAIN_SEGMENT_COUNT * PLAIN_SEGMENT_WIDTH_VERTEX * sizeof(GLfloat))),
				GL_STATIC_DRAW), PLAIN_INDEX_VERTEX, PLAIN_SEGMENT_WIDTH_VERTEX, GL_FLOAT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		plain::set_color(
			__in const glm::vec4 &color
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Color={%f, %f, %f, %f}", color.x, color.y, color.z, color.w);

			m_color = color;
			reconfigure();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		plain::set_dimensions(
			__in const glm::vec2 dimensions,
			__in_opt float scale
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimension={%f, %f}, Scale=%f", dimensions.x, dimensions.y, scale);

			m_dimensions = dimensions;
			m_scale = scale;
			reconfigure();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		plain::set_position(
			__in const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}",
				position.x, position.y, position.z, rotation.x, rotation.y, rotation.z, up.x, up.y, up.z);

			m_position = position;
			m_rotation = rotation,
			m_up = up;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		plain::set_texture(
			__in const std::string &path,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s", path.size(), STRING_CHECK(path));

			m_texture_set = true;
			nomic::graphic::texture::set(path, wrap_s, wrap_t, filter_min, filter_mag);
			reconfigure();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		plain::set_texture(
			__in const std::vector<uint8_t> &data,
			__in const glm::uvec2 &data_dimensions,
			__in uint32_t data_depth,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Data[%u]=%p, {%u, %u}, %u", data.size(), &data[0], data_dimensions.x, data_dimensions.y,
				data_depth);

			m_texture_set = true;
			nomic::graphic::texture::set(data, data_dimensions, data_depth, wrap_s, wrap_t, filter_min, filter_mag);
			reconfigure();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		plain::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_PLAIN_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Dimension={" << m_dimensions.x << ", " << m_dimensions.y << "}"
					<< ", Scale =" << m_scale
					<< ", Mode=" << (m_texture_set ? "Texture" : "Color");

				if(m_texture_set) {
					result << ", Texture=" << nomic::graphic::texture::to_string(verbose);
				} else {
					result << ", Color={" << m_color.x << ", " << m_color.y << ", " << m_color.z << ", " << m_color.w << "}";
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
