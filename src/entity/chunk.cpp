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

#include "../../include/entity/chunk.h"
#include "../../include/entity/camera.h"
#include "../../include/trace.h"
#include "./chunk_type.h"

namespace nomic {

	namespace entity {

		#define CHUNK_SEGMENT_WIDTH_COLOR 4
		#define CHUNK_SEGMENT_WIDTH_COORDINATE 2
		#define CHUNK_SEGMENT_WIDTH_VERTEX 3

		enum {
			CHUNK_INDEX_COLOR = 0,
			CHUNK_INDEX_COORDINATE,
			CHUNK_INDEX_VERTEX,
		};

		static const glm::vec4 CHUNK_COLOR[] = {
			{ BLOCK_COLOR_RGBA_DEFAULT }, // bottom left corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // top left corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // top right corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // bottom left corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // top right corner
			{ BLOCK_COLOR_RGBA_DEFAULT }, // bottom right corner
			};

		static const glm::vec2 CHUNK_COORDINATE[] = {
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

		static const glm::vec3 CHUNK_VERTEX[] = {
			{ BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS }, // right
			{ BLOCK_RADIUS, -BLOCK_RADIUS, BLOCK_RADIUS },
			{ BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_RADIUS },
			{ BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_RADIUS },
			{ BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS },
			{ BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS },
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, BLOCK_RADIUS }, // left
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS },
			{ -BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS },
			{ -BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS },
			{ -BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_RADIUS },
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, BLOCK_RADIUS },
			{ -BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS }, // top
			{ BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS },
			{ BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_RADIUS },
			{ BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_RADIUS },
			{ -BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_RADIUS },
			{ -BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS },
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS }, // bottom
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, BLOCK_RADIUS },
			{ BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS },
			{ BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS },
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, BLOCK_RADIUS },
			{ BLOCK_RADIUS, -BLOCK_RADIUS, BLOCK_RADIUS },
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, BLOCK_RADIUS }, // back
			{ -BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_RADIUS },
			{ BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_RADIUS },
			{ BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_RADIUS },
			{ BLOCK_RADIUS, -BLOCK_RADIUS, BLOCK_RADIUS },
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, BLOCK_RADIUS },
			{ -BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS }, // front
			{ -BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS },
			{ BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS },
			{ BLOCK_RADIUS, -BLOCK_RADIUS, -BLOCK_RADIUS },
			{ BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS },
			{ -BLOCK_RADIUS, BLOCK_RADIUS, -BLOCK_RADIUS },
			};

		chunk::chunk(
			__in const glm::ivec2 &position,
			__in nomic::terrain::generator &generator
			) :
				nomic::entity::object(ENTITY_CHUNK, SUBTYPE_UNDEFINED, glm::vec3(position.x, 0.0, position.y)),
				nomic::terrain::chunk(position),
				m_changed(false),
				m_chunk_back(nullptr),
				m_chunk_front(nullptr),
				m_chunk_left(nullptr),
				m_chunk_right(nullptr)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}, Generator=%p", position.x, position.y, &generator);

			generator.chunk(position, *this);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		chunk::chunk(
			__in const chunk &other
			) :
				nomic::entity::object(other),
				nomic::terrain::chunk(other),
				m_changed(other.m_changed),
				m_chunk_back(other.m_chunk_back),
				m_chunk_front(other.m_chunk_front),
				m_chunk_left(other.m_chunk_left),
				m_chunk_right(other.m_chunk_right),
				m_face(other.m_face),
				m_vao_color(other.m_vao_color),
				m_vao_coordinate(other.m_vao_coordinate),
				m_vao_vertex(other.m_vao_vertex)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		chunk::~chunk(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		chunk &
		chunk::operator=(
			__in const chunk &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				nomic::terrain::chunk::operator=(other);
				m_changed = other.m_changed;
				m_chunk_back = other.m_chunk_back;
				m_chunk_front = other.m_chunk_front;
				m_chunk_left = other.m_chunk_left;
				m_chunk_right = other.m_chunk_right;
				m_face = other.m_face;
				m_vao_color = other.m_vao_color;
				m_vao_coordinate = other.m_vao_coordinate;
				m_vao_vertex = other.m_vao_vertex;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		chunk::add_face(
			__in const glm::vec3 &position,
			__in uint32_t face
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Face=%x", position.x, position.y, position.z, face);

			for(uint32_t iter = 0; iter < BLOCK_FACE_COUNT; ++iter) {
				m_vao_color.push_back(CHUNK_COLOR[iter]);
				m_vao_coordinate.push_back(CHUNK_COORDINATE[(face * BLOCK_FACE_COUNT) + iter]);
				m_vao_vertex.push_back(CHUNK_VERTEX[(face * BLOCK_FACE_COUNT) + iter] + position);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		chunk::add_face_type(
			__in uint8_t type,
			__in uint32_t face
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Face=%x", type, face);

			switch(type) {
				case BLOCK_AIR:
				case BLOCK_BOUNDARY:
				case BLOCK_DIRT:
				case BLOCK_SAND:
				case BLOCK_STONE:
				case BLOCK_WATER:
					result = type;
					break;
				case BLOCK_GRASS:

					switch(face) {
						case BLOCK_FACE_BACK:
						case BLOCK_FACE_FRONT:
						case BLOCK_FACE_LEFT:
						case BLOCK_FACE_RIGHT:
							result = BLOCK_DIRT_GRASS_SIDE;
							break;
						case BLOCK_FACE_BOTTOM:
							result = BLOCK_DIRT;
							break;
						case BLOCK_FACE_TOP:
							result = BLOCK_GRASS;
							break;
						default:
							THROW_NOMIC_ENTITY_CHUNK_EXCEPTION_FORMAT(NOMIC_ENTITY_CHUNK_EXCEPTION_FACE_INVALID,
								"Face=%x", face);
					}
					break;
				case BLOCK_SNOW:

					switch(face) {
						case BLOCK_FACE_BACK:
						case BLOCK_FACE_FRONT:
						case BLOCK_FACE_LEFT:
						case BLOCK_FACE_RIGHT:
							result = BLOCK_STONE_SNOW_SIDE;
							break;
						case BLOCK_FACE_BOTTOM:
							result = BLOCK_STONE;
							break;
						case BLOCK_FACE_TOP:
							result = BLOCK_SNOW;
							break;
						default:
							THROW_NOMIC_ENTITY_CHUNK_EXCEPTION_FORMAT(NOMIC_ENTITY_CHUNK_EXCEPTION_FACE_INVALID,
								"Face=%x", face);
					}
					break;
				default:
					THROW_NOMIC_ENTITY_CHUNK_EXCEPTION_FORMAT(NOMIC_ENTITY_CHUNK_EXCEPTION_TYPE_INVALID,
						"Type=%x", type);
			}

			m_face.push_back(result);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint8_t 
		chunk::block_attributes(
			__in const glm::uvec3 &position
			) const
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}", position.x, position.y, position.z);

			result = nomic::terrain::chunk::attributes(position);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x(%u)", (uint16_t) result, (uint16_t) result);
			return result;
		}

		uint8_t 
		chunk::block_height(
			__in const glm::uvec2 &position
			) const
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u}", position.x, position.y);

			result = nomic::terrain::chunk::height(position);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x(%u)", (uint16_t) result, (uint16_t) result);
			return result;
		}

		uint8_t 
		chunk::block_type(
			__in const glm::uvec3 &position
			) const
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}", position.x, position.y, position.z);

			result = nomic::terrain::chunk::type(position);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x(%u)", (uint16_t) result, (uint16_t) result);
			return result;
		}

		void 
		chunk::on_render(
			__in nomic::core::renderer &renderer,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Delta=%f", &renderer, delta);

			if(!m_face.empty()) {
				vertex_array().bind();

				// TODO: render vao			
				GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, m_face.size() * BLOCK_FACE_COUNT);
				// ---
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		chunk::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			int32_t dx, dz;
			glm::vec3 camera_position;
			glm::ivec2 camera_position_chunk, position_chunk;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);

			nomic::terrain::chunk::update();

			if(m_changed) {
				m_changed = false;
				rebuild(m_chunk_right, m_chunk_left, m_chunk_back, m_chunk_front);

				nomic::graphic::vao &arr = vertex_array();
				arr.disable_all();
				arr.remove_all();
				arr.clear();
				arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &m_vao_color[0],
					((uint8_t *) &m_vao_color[0]) + ((m_vao_color.size() * CHUNK_SEGMENT_WIDTH_COLOR) * sizeof(GLfloat))),
					GL_STATIC_DRAW), CHUNK_INDEX_COLOR, CHUNK_SEGMENT_WIDTH_COLOR, GL_FLOAT);
				arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &m_vao_coordinate[0],
					((uint8_t *) &m_vao_coordinate[0]) + ((m_vao_coordinate.size() * CHUNK_SEGMENT_WIDTH_COORDINATE) * sizeof(GLfloat))),
					GL_STATIC_DRAW), CHUNK_INDEX_COORDINATE, CHUNK_SEGMENT_WIDTH_COORDINATE, GL_FLOAT);
				arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &m_vao_vertex[0],
					((uint8_t *) &m_vao_vertex[0]) + ((m_vao_vertex.size() * CHUNK_SEGMENT_WIDTH_VERTEX) * sizeof(GLfloat))),
					GL_STATIC_DRAW), CHUNK_INDEX_VERTEX, CHUNK_SEGMENT_WIDTH_VERTEX, GL_FLOAT);
				arr.enable(CHUNK_INDEX_COLOR);
				arr.enable(CHUNK_INDEX_COORDINATE);
				arr.enable(CHUNK_INDEX_VERTEX);
			}

			camera_position = ((nomic::entity::camera *) camera)->position();
			camera_position_chunk = glm::ivec2(camera_position.x / CHUNK_WIDTH, camera_position.z / CHUNK_WIDTH);
			position_chunk = nomic::entity::chunk::position();
			dx = ((position_chunk.x > camera_position_chunk.x) ? (position_chunk.x - camera_position_chunk.x) :
				(camera_position_chunk.x - position_chunk.x));
			dz = ((position_chunk.y > camera_position_chunk.y) ? (position_chunk.y - camera_position_chunk.y) :
				(camera_position_chunk.y - position_chunk.y));
			show((std::abs(dx * dx) + std::abs(dz * dz)) <= (VIEW_RADIUS_RUNTIME * VIEW_RADIUS_RUNTIME));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		glm::ivec2 
		chunk::position(void) const
		{
			glm::ivec2 result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = nomic::terrain::chunk::position();

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Position={%i, %i}", result.x, result.y);
			return result;
		}

		void 
		chunk::rebuild(
			__in_opt nomic::entity::chunk *right,
			__in_opt nomic::entity::chunk *left,
			__in_opt nomic::entity::chunk *back,
			__in_opt nomic::entity::chunk *front
			)
		{
			glm::vec3 position;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Edge={%p, %p, %p, %p}", right, left, back, front);

			m_face.clear();
			m_vao_color.clear();
			m_vao_coordinate.clear();
			m_vao_vertex.clear();
			m_chunk_back = back;
			m_chunk_front = front;
			m_chunk_left = left;
			m_chunk_right = right;
			position.x = (nomic::terrain::chunk::position().x * (CHUNK_WIDTH - 1));
			position.z = (nomic::terrain::chunk::position().y * (CHUNK_WIDTH - 1));

			for(int32_t z = 0; z < CHUNK_WIDTH; ++z) {

				for(int32_t x = 0; x < CHUNK_WIDTH; ++x) {
					position.y = nomic::terrain::chunk::height(glm::uvec2(x, z));

					for(int32_t y = position.y; y >= 0; --y) {
						uint32_t count = 0;
						glm::vec3 position_relative = glm::vec3(x, y, z);

						uint32_t type = nomic::terrain::chunk::type(position_relative);
						if(type != BLOCK_AIR) {
							position.y = 0;

							if(right) {

								if(((x + 1) < CHUNK_WIDTH) && (nomic::terrain::chunk::type(glm::uvec3(x + 1, y, z))
										== BLOCK_AIR)) { // right
									add_face(position + position_relative, BLOCK_FACE_RIGHT);
									add_face_type(type, BLOCK_FACE_RIGHT);
									++count;
								} else if(((x + 1) == CHUNK_WIDTH) && (right->block_type(glm::uvec3(0, y, z))
										== BLOCK_AIR)) { // right (boundary)
									add_face(position + position_relative, BLOCK_FACE_RIGHT);
									add_face_type(type, BLOCK_FACE_RIGHT);
									++count;
								}
							}

							if(left) {

								if(((x - 1) >= 0) && (nomic::terrain::chunk::type(glm::uvec3(x - 1, y, z))
										== BLOCK_AIR)) { // left
									add_face(position + position_relative, BLOCK_FACE_LEFT);
									add_face_type(type, BLOCK_FACE_LEFT);
									++count;
								} else if(((x - 1) < 0) && (left->block_type(glm::uvec3(CHUNK_WIDTH - 1, y, z))
										== BLOCK_AIR)) { // left (boundary)
									add_face(position + position_relative, BLOCK_FACE_LEFT);
									add_face_type(type, BLOCK_FACE_LEFT);
									++count;
								}
							}

							if(((y + 1) < CHUNK_HEIGHT) && (nomic::terrain::chunk::type(glm::uvec3(x, y + 1, z))
									== BLOCK_AIR)) { // top
								add_face(position + position_relative, BLOCK_FACE_TOP);
								add_face_type(type, BLOCK_FACE_TOP);
								++count;
							}

							if(((y - 1) >= 0) && (nomic::terrain::chunk::type(glm::uvec3(x, y - 1, z))
									== BLOCK_AIR)) { // bottom
								add_face(position + position_relative, BLOCK_FACE_BOTTOM);
								add_face_type(type, BLOCK_FACE_BOTTOM);
								++count;
							}

							if(back) {

								if(((z + 1) < CHUNK_WIDTH) && (nomic::terrain::chunk::type(glm::uvec3(x, y, z + 1))
										== BLOCK_AIR)) { // back
									add_face(position + position_relative, BLOCK_FACE_BACK);
									add_face_type(type, BLOCK_FACE_BACK);
									++count;
								} else if(((z + 1) == CHUNK_WIDTH) && (back->block_type(glm::uvec3(x, y, 0))
										== BLOCK_AIR)) { // back (boundary)
									add_face(position + position_relative, BLOCK_FACE_BACK);
									add_face_type(type, BLOCK_FACE_BACK);
									++count;
								}
							}

							if(front) {
								if(((z - 1) >= 0) && (nomic::terrain::chunk::type(glm::uvec3(x, y, z - 1))
										== BLOCK_AIR)) { // front
									add_face(position + position_relative, BLOCK_FACE_FRONT);
									add_face_type(type, BLOCK_FACE_FRONT);
									++count;
								} else if(((z - 1) < 0) && (front->block_type(glm::uvec3(x, y, CHUNK_WIDTH - 1))
										== BLOCK_AIR)) { // front (boundary)
									add_face(position + position_relative, BLOCK_FACE_FRONT);
									add_face_type(type, BLOCK_FACE_FRONT);
									++count;
								}
							}

							if(!count) {
								break;
							}
						}
					}
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint8_t 
		chunk::set_block(
			__in const glm::uvec3 &position,
			__in uint8_t type,
			__in_opt uint8_t attributes
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Type=%x, Attributes=%x(%u)", position.x, position.y, position.z,
				type, attributes);

			result = nomic::terrain::chunk::set_block(position, type, attributes);
			m_changed = true;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x(%u)", (uint16_t) result, (uint16_t) result);
			return result;
		}

		void 
		chunk::teardown(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_face.clear();
			m_chunk_back = nullptr;
			m_chunk_front = nullptr;
			m_chunk_left = nullptr;
			m_chunk_right = nullptr;
			m_vao_color.clear();
			m_vao_coordinate.clear();
			m_vao_vertex.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		chunk::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_CHUNK_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Chunk=" << nomic::terrain::chunk::to_string(verbose)
					<< ", Face[" << m_face.size() << "]"
					<< ", State=" << (m_changed ? "Changed" : "Unchanged");
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		chunk::update(
			__in_opt nomic::entity::chunk *right,
			__in_opt nomic::entity::chunk *left,
			__in_opt nomic::entity::chunk *back,
			__in_opt nomic::entity::chunk *front
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Edge={%p, %p, %p, %p}", right, left, back, front);

			m_chunk_back = back;
			m_chunk_front = front;
			m_chunk_left = left;
			m_chunk_right = right;
			m_changed = true;

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
