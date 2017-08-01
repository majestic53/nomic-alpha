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

#include "../../include/entity/block.h"
#include "../../include/trace.h"
#include "./block_type.h"

namespace nomic {

	namespace entity {

		#define BLOCK_SEGMENT_COUNT 36
		#define BLOCK_SEGMENT_WIDTH_COLOR 4
		#define BLOCK_SEGMENT_WIDTH_COORDINATE 2
		#define BLOCK_SEGMENT_WIDTH_VERTEX 3

		enum {
			BLOCK_INDEX_COLOR = 0,
			BLOCK_INDEX_COORDINATE,
			BLOCK_INDEX_VERTEX,
		};

		static const glm::vec4 BLOCK_COLOR[] = {
			{ BLOCK_COLOR_RGBA_DEFAULT }, // bottom left corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // top left corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // top right corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // bottom left corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // top right corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // bottom right corner
			};

		static const glm::vec2 BLOCK_COORDINATE[] = {
			{ 1.f, 1.f, }, // right
			{ 0.f, 1.f, },
			{ 0.f, 0.f, },
			{ 0.f, 0.f, },
			{ 1.f, 0.f, },
			{ 1.f, 1.f, },
			{ 1.f, 1.f, }, // left
			{ 0.f, 1.f, },
			{ 0.f, 0.f, },
			{ 0.f, 0.f, },
			{ 1.f, 0.f, },
			{ 1.f, 1.f, },
			{ 1.f, 1.f, }, // top
			{ 0.f, 1.f, },
			{ 0.f, 0.f, },
			{ 0.f, 0.f, },
			{ 1.f, 0.f, },
			{ 1.f, 1.f, },
			{ 1.f, 0.f, }, // bottom
			{ 1.f, 1.f, },
			{ 0.f, 0.f, },
			{ 0.f, 0.f, },
			{ 1.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 1.f, }, // back
			{ 0.f, 0.f, },
			{ 1.f, 0.f, },
			{ 1.f, 0.f, },
			{ 1.f, 1.f, },
			{ 0.f, 1.f, },
			{ 1.f, 0.f, }, // front
			{ 1.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 0.f, },
			{ 1.f, 0.f, },
			};

		static const glm::vec3 BLOCK_VERTEX[] = {
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

		block::block(
			__in_opt const std::map<uint32_t, std::string> &paths,
			__in_opt float scale,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up
			) :
				nomic::entity::object(ENTITY_BLOCK, SUBTYPE_UNDEFINED, position, rotation, up),
				m_scale(0)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,
				"Paths[%u], Scale=%f, Wrap={%x, %x}, Filter={%x, %x}, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}",
				paths.size(), scale, wrap_s, wrap_t, filter_min, filter_mag, position.x, position.y, position.z, rotation.x,
				rotation.y, rotation.z, up.x, up.y, up.z);

			if(!paths.empty()) {
				set_textures(paths, wrap_s, wrap_t, filter_min, filter_mag);
			}

			set_scale(scale);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		block::block(
			__in const block &other
			) :
				nomic::entity::object(other),
				m_face(other.m_face),
				m_scale(other.m_scale)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		block::~block(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		block &
		block::operator=(
			__in const block &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				m_face = other.m_face;
				m_scale = other.m_scale;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		block::on_render(
			__in nomic::core::renderer &renderer,
			__in void *textures,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Textures=%p, Delta=%f", &renderer, textures, delta);

			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			arr.enable(BLOCK_INDEX_COLOR);
			arr.enable(BLOCK_INDEX_COORDINATE);
			arr.enable(BLOCK_INDEX_VERTEX);

			if(!m_face.empty()) {

				for(uint32_t iter = BLOCK_FACE_MIN; iter <= BLOCK_FACE_MAX; ++iter) {
					m_face.at(iter).bind();
					GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, iter * BLOCK_FACE_COUNT, BLOCK_FACE_COUNT);
				}
			} else {
				GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, BLOCK_SEGMENT_COUNT);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		block::reconfigure(void)
		{
			glm::vec3 scale;
			std::vector<glm::vec3> vertex;
			std::vector<glm::vec4> color;
			std::vector<glm::vec2> coordinate;

			TRACE_ENTRY(LEVEL_VERBOSE);

			for(uint32_t iter = BLOCK_FACE_MIN; iter <= BLOCK_FACE_MAX; ++iter) {

				for(uint32_t face = BLOCK_FACE_MIN; face <= BLOCK_FACE_MAX; ++face) {
					color.push_back(BLOCK_COLOR[face]);
					coordinate.push_back(BLOCK_COORDINATE[(iter * BLOCK_FACE_COUNT) + face]);
				}
			}

			scale = glm::vec3(m_scale, m_scale, m_scale);

			for(uint32_t iter = 0; iter < BLOCK_SEGMENT_COUNT; ++iter) {
				vertex.push_back(BLOCK_VERTEX[iter] * scale);
			}

			nomic::graphic::vao &arr = vertex_array();
			arr.disable_all();
			arr.remove_all();
			arr.clear();
			arr.bind();
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &color[0],
				((uint8_t *) &color[0]) + (BLOCK_SEGMENT_COUNT * BLOCK_SEGMENT_WIDTH_COLOR * sizeof(GLfloat))),
				GL_STATIC_DRAW), BLOCK_INDEX_COLOR, BLOCK_SEGMENT_WIDTH_COLOR, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &coordinate[0],
				((uint8_t *) &coordinate[0]) + (BLOCK_SEGMENT_COUNT * BLOCK_SEGMENT_WIDTH_COORDINATE * sizeof(GLfloat))),
				GL_STATIC_DRAW), BLOCK_INDEX_COORDINATE, BLOCK_SEGMENT_WIDTH_COORDINATE, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &vertex[0],
				((uint8_t *) &vertex[0]) + (BLOCK_SEGMENT_COUNT * BLOCK_SEGMENT_WIDTH_VERTEX * sizeof(GLfloat))),
				GL_STATIC_DRAW), BLOCK_INDEX_VERTEX, BLOCK_SEGMENT_WIDTH_VERTEX, GL_FLOAT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		block::set_scale(
			__in float scale
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Scale=%f", scale);

			if(scale < BLOCK_SCALE_MIN) {
				THROW_NOMIC_ENTITY_BLOCK_EXCEPTION_FORMAT(NOMIC_ENTITY_BLOCK_EXCEPTION_SCALE_INVALID, "Scale=%f", scale);
			}

			m_scale = scale;
			reconfigure();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		block::set_textures(
			__in_opt const std::map<uint32_t, std::string> &paths,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Paths[%u], Wrap={%x, %x}, Filter={%x, %x}", paths.size(), wrap_s, wrap_t,
				filter_min, filter_mag);

			m_face.clear();

			for(uint32_t face = BLOCK_FACE_MIN; face <= BLOCK_FACE_MAX; ++face) {

				std::map<uint32_t, std::string>::const_iterator iter = paths.find(face);
				if(iter == paths.end()) {
					THROW_NOMIC_ENTITY_BLOCK_EXCEPTION_FORMAT(NOMIC_ENTITY_BLOCK_EXCEPTION_FACE_INVALID, "Face=%x", face);
				}

				m_face.push_back(nomic::graphic::texture());
				m_face.back().set(iter->second, wrap_s, wrap_t, filter_min, filter_mag);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		block::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_BLOCK_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Face[" << m_face.size() << "]";

				if(!m_face.empty()) {
					result << "={";

					for(std::vector<nomic::graphic::texture>::const_iterator iter = m_face.begin(); iter != m_face.end();
							++iter) {

						if(iter != m_face.begin()) {
							result << ", ";
						}

						result << iter->to_string(verbose);
					}

					result << "}";
				}

				result << ", Scale=" << m_scale;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
