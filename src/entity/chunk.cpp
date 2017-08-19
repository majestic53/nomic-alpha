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
#include "../../include/graphic/atlas.h"
#include "../../include/trace.h"
#include "./chunk_type.h"

namespace nomic {

	namespace entity {

		#define CHUNK_SEGMENT_WIDTH_COORDINATE 2
		#define CHUNK_SEGMENT_WIDTH_NORMAL 3
		#define CHUNK_SEGMENT_WIDTH_VERTEX 3

		enum {
			CHUNK_INDEX_COORDINATE = 0,
			CHUNK_INDEX_NORMAL,
			CHUNK_INDEX_VERTEX,
		};

		enum {
			VAO_COORDINATES = 0,
			VAO_NORMAL,
			VAO_VERTEX,
			VAO_BASE,
			VAO_OFFSET,
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

		static const glm::vec2 CHUNK_COORDINATE_ROTATED[] = {
			{ 1.f, 0.f, }, // right
			{ 1.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 0.f, },
			{ 1.f, 0.f, },
			{ 1.f, 0.f, }, // left
			{ 1.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 0.f, },
			{ 1.f, 0.f, },
			{ 1.f, 0.f, }, // top
			{ 1.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 0.f, },
			{ 1.f, 0.f, },
			{ 0.f, 0.f, }, // bottom
			{ 1.f, 0.f, },
			{ 0.f, 1.f, },
			{ 0.f, 1.f, },
			{ 1.f, 0.f, },
			{ 1.f, 1.f, },
			{ 1.f, 1.f, }, // back
			{ 0.f, 1.f, },
			{ 0.f, 0.f, },
			{ 0.f, 0.f, },
			{ 1.f, 0.f, },
			{ 1.f, 1.f, },
			{ 0.f, 0.f, }, // front
			{ 1.f, 0.f, },
			{ 1.f, 1.f, },
			{ 1.f, 1.f, },
			{ 0.f, 1.f, },
			{ 0.f, 0.f, },
			};

		static const glm::vec3 CHUNK_NORMAL[] = {
			{ 1.f, 0.f, 0.f }, // right
			{ -1.f, 0.f, 0.f }, // left
			{ 0.f, 1.f, 0.f }, // top
			{ 0.f, -1.f, 0.f }, // bottom
			{ 0.f, 0.f, 1.f }, // back
			{ 0.f, 0.f, -1.f }, // front
			};

		static const glm::vec3 CHUNK_NORMAL_DECORATION[] = {
			{ 1.f, 0.f, 1.f }, // right
			{ -1.f, 0.f, 1.f }, // left
			};

		static const glm::vec3 CHUNK_VERTEX[] = {
			{ BLOCK_WIDTH, 0.f, 0.f }, // right
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ 0.f, 0.f, BLOCK_WIDTH }, // left
			{ 0.f, 0.f, 0.f },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, 0.f }, // top
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ 0.f, 0.f, 0.f }, // bottom
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ 0.f, 0.f, BLOCK_WIDTH }, // back
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, 0.f }, // front
			{ 0.f, 0.f, 0.f },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f },
			{ 0.f, BLOCK_WIDTH, 0.f },
			};

		static const glm::vec3 CHUNK_VERTEX_DECORATION[] = {
			{ BLOCK_WIDTH, 0.f, 0.f }, // right
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH }, // left
			{ 0.f, 0.f, 0.f },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			};

		static const glm::vec3 CHUNK_VERTEX_SMALL[] = {
			{ BLOCK_WIDTH - BLOCK_SMALL_OFFSET, 0.f, 0.f }, // right
			{ BLOCK_WIDTH - BLOCK_SMALL_OFFSET, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH - BLOCK_SMALL_OFFSET, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH - BLOCK_SMALL_OFFSET, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH - BLOCK_SMALL_OFFSET, BLOCK_WIDTH, 0.f },
			{ BLOCK_WIDTH - BLOCK_SMALL_OFFSET, 0.f, 0.f },
			{ BLOCK_SMALL_OFFSET, 0.f, BLOCK_WIDTH }, // left
			{ BLOCK_SMALL_OFFSET, 0.f, 0.f },
			{ BLOCK_SMALL_OFFSET, BLOCK_WIDTH, 0.f },
			{ BLOCK_SMALL_OFFSET, BLOCK_WIDTH, 0.f },
			{ BLOCK_SMALL_OFFSET, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_SMALL_OFFSET, 0.f, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, 0.f }, // top
			{ BLOCK_WIDTH, BLOCK_WIDTH, 0.f },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH },
			{ 0.f, BLOCK_WIDTH, 0.f },
			{ 0.f, 0.f, 0.f }, // bottom
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ BLOCK_WIDTH, 0.f, 0.f },
			{ 0.f, 0.f, BLOCK_WIDTH },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH },
			{ 0.f, 0.f, BLOCK_WIDTH - BLOCK_SMALL_OFFSET }, // back
			{ 0.f, BLOCK_WIDTH, BLOCK_WIDTH - BLOCK_SMALL_OFFSET },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH - BLOCK_SMALL_OFFSET },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH - BLOCK_SMALL_OFFSET },
			{ BLOCK_WIDTH, 0.f, BLOCK_WIDTH - BLOCK_SMALL_OFFSET },
			{ 0.f, 0.f, BLOCK_WIDTH - BLOCK_SMALL_OFFSET },
			{ 0.f, BLOCK_WIDTH, BLOCK_SMALL_OFFSET }, // front
			{ 0.f, 0.f, BLOCK_SMALL_OFFSET },
			{ BLOCK_WIDTH, 0.f, BLOCK_SMALL_OFFSET },
			{ BLOCK_WIDTH, 0.f, BLOCK_SMALL_OFFSET },
			{ BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_SMALL_OFFSET },
			{ 0.f, BLOCK_WIDTH, BLOCK_SMALL_OFFSET },
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
				m_face(other.m_face)
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
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		chunk::add_face(
			__in const glm::vec3 &position,
			__in uint32_t face,
			__in uint32_t attributes,
			__in std::map<uint8_t, chunk_data>::iterator entry
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Face=%x, Attributes=%x", position.x, position.y, position.z, face,
				attributes);

			std::vector<glm::vec2> &coordinate = std::get<VAO_COORDINATES>(entry->second);
			std::vector<glm::vec3> &normal = std::get<VAO_NORMAL>(entry->second);
			std::vector<glm::vec3> &vertex = std::get<VAO_VERTEX>(entry->second);

			for(uint32_t iter = 0; iter < BLOCK_FACE_COUNT; ++iter) {

				if(attributes & BLOCK_ATTRIBUTE_ROTATABLE) {

					if((attributes & BLOCK_ATTRIBUTE_ROTATED_RIGHT) || (attributes & BLOCK_ATTRIBUTE_ROTATED_LEFT)) {

						if((face == BLOCK_FACE_BOTTOM) || (face == BLOCK_FACE_TOP)
								|| (face == BLOCK_FACE_BACK) || (face == BLOCK_FACE_FRONT)) {
							coordinate.push_back(CHUNK_COORDINATE_ROTATED[(face * BLOCK_FACE_COUNT) + iter]);
						} else {
							coordinate.push_back(CHUNK_COORDINATE[(face * BLOCK_FACE_COUNT) + iter]);
						}
					} else if((attributes & BLOCK_ATTRIBUTE_ROTATED_BACK) || (attributes & BLOCK_ATTRIBUTE_ROTATED_FRONT)) {

						if((face == BLOCK_FACE_LEFT) || (face == BLOCK_FACE_RIGHT)) {
							coordinate.push_back(CHUNK_COORDINATE_ROTATED[(face * BLOCK_FACE_COUNT) + iter]);
						} else {
							coordinate.push_back(CHUNK_COORDINATE[(face * BLOCK_FACE_COUNT) + iter]);
						}
					} else {
						coordinate.push_back(CHUNK_COORDINATE[(face * BLOCK_FACE_COUNT) + iter]);
					}
				} else {
					coordinate.push_back(CHUNK_COORDINATE[(face * BLOCK_FACE_COUNT) + iter]);
				}

				if((entry->first == BLOCK_CLOUD) && (face == BLOCK_FACE_BOTTOM)) {
					normal.push_back(CHUNK_NORMAL[BLOCK_FACE_TOP]);
				} else {
					normal.push_back(CHUNK_NORMAL[face]);
				}

				vertex.push_back(CHUNK_VERTEX[(face * BLOCK_FACE_COUNT) + iter] + position);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		chunk::add_face_decoration(
			__in const glm::vec3 &position,
			__in uint32_t face,
			__in uint32_t attributes,
			__in std::map<uint8_t, chunk_data>::iterator entry
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Face=%x, Attributes=%x", position.x, position.y, position.z, face,
				attributes);

			std::vector<glm::vec2> &coordinate = std::get<VAO_COORDINATES>(entry->second);
			std::vector<glm::vec3> &normal = std::get<VAO_NORMAL>(entry->second);
			std::vector<glm::vec3> &vertex = std::get<VAO_VERTEX>(entry->second);

			if(face > BLOCK_FACE_LEFT) {
				THROW_NOMIC_ENTITY_CHUNK_EXCEPTION_FORMAT(NOMIC_ENTITY_CHUNK_EXCEPTION_FACE_INVALID, "Face=%x", face);
			}

			for(uint32_t iter = 0; iter < BLOCK_FACE_COUNT; ++iter) {
				coordinate.push_back(CHUNK_COORDINATE[(face * BLOCK_FACE_COUNT) + iter]);
				normal.push_back(CHUNK_NORMAL_DECORATION[face]);
				vertex.push_back(CHUNK_VERTEX_DECORATION[(face * BLOCK_FACE_COUNT) + iter] + position);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		chunk::add_face_small(
			__in const glm::vec3 &position,
			__in uint32_t face,
			__in uint32_t attributes,
			__in std::map<uint8_t, chunk_data>::iterator entry
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}, Face=%x, Attributes=%x", position.x, position.y, position.z, face,
				attributes);

			std::vector<glm::vec2> &coordinate = std::get<VAO_COORDINATES>(entry->second);
			std::vector<glm::vec3> &normal = std::get<VAO_NORMAL>(entry->second);
			std::vector<glm::vec3> &vertex = std::get<VAO_VERTEX>(entry->second);

			for(uint32_t iter = 0; iter < BLOCK_FACE_COUNT; ++iter) {
				coordinate.push_back(CHUNK_COORDINATE[(face * BLOCK_FACE_COUNT) + iter]);
				normal.push_back(CHUNK_NORMAL[face]);
				vertex.push_back(CHUNK_VERTEX_SMALL[(face * BLOCK_FACE_COUNT) + iter] + position);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::map<uint8_t, chunk_data>::iterator 
		chunk::add_face_type(
			__in uint8_t type,
			__in uint32_t face,
			__inout uint8_t &attributes
			)
		{
			std::map<uint8_t, chunk_data>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Face=%x, Attributes=%x", type, face, attributes);

			type = nomic::utility::block_face_type(type, face, attributes);

			result = m_face.find(type);
			if(result == m_face.end()) {
				m_face.insert(std::make_pair(type, chunk_data()));

				result = m_face.find(type);
				if(result == m_face.end()) {
					THROW_NOMIC_ENTITY_CHUNK_EXCEPTION_FORMAT(NOMIC_ENTITY_CHUNK_EXCEPTION_TYPE_INVALID,
						"Type=%x", type);
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		void 
		chunk::add_tree(
			__in const glm::uvec3 &position,
			__in uint8_t type,
			__in uint32_t attributes
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%u, %u, %u}, Type=%x, Attributes=%x", position.x, position.y, position.z,
				type, attributes);

			switch(type) {
				case BLOCK_WOOD_OAK: // oak

					// TODO: generate oak tree
					set_block(position, BLOCK_WOOD_OAK);
					// ---

					break;
				case BLOCK_WOOD_SPRUCE: // spruce

					// TODO: generate spruce tree
					set_block(position, BLOCK_WOOD_SPRUCE);
					// ---

					break;
				default:
					THROW_NOMIC_ENTITY_CHUNK_EXCEPTION_FORMAT(NOMIC_ENTITY_CHUNK_EXCEPTION_TYPE_INVALID,
						"Type=%x", type);
			}

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
			__in void *textures,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Textures=%p, Delta=%f", &renderer, textures, delta);

			if(!m_face.empty()) {
				nomic::graphic::vao &arr = vertex_array();
				arr.bind();
				arr.enable(CHUNK_INDEX_COORDINATE);
				arr.enable(CHUNK_INDEX_NORMAL);
				arr.enable(CHUNK_INDEX_VERTEX);

				if(textures) {
					nomic::graphic::atlas *texture_ref = (nomic::graphic::atlas *) textures;

					for(std::map<uint8_t, chunk_data>::iterator iter = m_face.begin(); iter != m_face.end(); ++iter) {

						bool decoration = nomic::utility::block_decoration(iter->first);
						if(decoration) {
							GL_CHECK(LEVEL_WARNING, glDisable, GL_CULL_FACE);
						}

						if(!nomic::utility::block_transparent(iter->first)) {
							texture_ref->enable(iter->first);
							GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, std::get<VAO_BASE>(iter->second),
								std::get<VAO_OFFSET>(iter->second));
						}

						if(decoration) {
							GL_CHECK(LEVEL_WARNING, glEnable, GL_CULL_FACE);
						}
					}
				} else {
					GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, m_face.size() * BLOCK_FACE_COUNT);
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		chunk::on_render_transparent(
			__in nomic::core::renderer &renderer,
			__in void *textures,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Textures=%p, Delta=%f", &renderer, textures, delta);

			if(!m_face.empty()) {
				nomic::graphic::vao &arr = vertex_array();
				arr.bind();
				arr.enable(CHUNK_INDEX_COORDINATE);
				arr.enable(CHUNK_INDEX_NORMAL);
				arr.enable(CHUNK_INDEX_VERTEX);

				if(textures) {
					nomic::graphic::atlas *texture_ref = (nomic::graphic::atlas *) textures;

					for(std::map<uint8_t, chunk_data>::iterator iter = m_face.begin(); iter != m_face.end(); ++iter) {

						bool decoration = nomic::utility::block_decoration(iter->first);
						if(decoration) {
							GL_CHECK(LEVEL_WARNING, glDisable, GL_CULL_FACE);
						}

						if(nomic::utility::block_transparent(iter->first)) {
							texture_ref->enable(iter->first);
							GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, std::get<VAO_BASE>(iter->second),
								std::get<VAO_OFFSET>(iter->second));
						}

						if(decoration) {
							GL_CHECK(LEVEL_WARNING, glEnable, GL_CULL_FACE);
						}
					}
				} else {
					GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, m_face.size() * BLOCK_FACE_COUNT);
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		chunk::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			bool shown = true;
			std::vector<std::pair<glm::uvec3, uint8_t>> spawn;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);

			spawn = nomic::terrain::chunk::spawn();
			if(!spawn.empty()) {
				m_changed = true;

				for(std::vector<std::pair<glm::uvec3, uint8_t>>::iterator iter = spawn.begin(); iter != spawn.end(); ++iter) {
					uint8_t attributes = BLOCK_ATTRIBUTES_DEFAULT, type = iter->second;

					switch(type) {
						case BLOCK_WOOD_OAK:
						case BLOCK_WOOD_SPRUCE:
							add_tree(iter->first, type, attributes);
							break;
						default:
							set_block(iter->first, type, attributes);
							break;
					}
				}

				spawn.clear();
			}

			nomic::terrain::chunk::update();

			if(m_changed) {
				std::vector<glm::vec2> coordinate;
				std::vector<glm::vec3> normal, vertex;

				m_changed = false;
				rebuild(m_chunk_right, m_chunk_left, m_chunk_back, m_chunk_front);

				for(std::map<uint8_t, chunk_data>::iterator iter = m_face.begin(); iter != m_face.end(); ++iter) {
					std::vector<glm::vec2> &iter_coordinate = std::get<VAO_COORDINATES>(iter->second);
					std::vector<glm::vec3> &iter_normal = std::get<VAO_NORMAL>(iter->second);
					std::vector<glm::vec3> &iter_vertex = std::get<VAO_VERTEX>(iter->second);

					coordinate.insert(coordinate.end(), iter_coordinate.begin(), iter_coordinate.end());
					normal.insert(normal.end(), iter_normal.begin(), iter_normal.end());
					std::get<VAO_BASE>(iter->second) = vertex.size();
					std::get<VAO_OFFSET>(iter->second) = iter_vertex.size();
					vertex.insert(vertex.end(), iter_vertex.begin(), iter_vertex.end());

					std::get<VAO_COORDINATES>(iter->second).clear();
					std::get<VAO_NORMAL>(iter->second).clear();
					std::get<VAO_VERTEX>(iter->second).clear();
				}

				nomic::graphic::vao &arr = vertex_array();
				arr.disable_all();
				arr.remove_all();
				arr.clear();
				arr.bind();
				arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &coordinate[0],
					((uint8_t *) &coordinate[0]) + (coordinate.size() * CHUNK_SEGMENT_WIDTH_COORDINATE * sizeof(GLfloat))),
					GL_STATIC_DRAW), CHUNK_INDEX_COORDINATE, CHUNK_SEGMENT_WIDTH_COORDINATE, GL_FLOAT);
				arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &normal[0],
					((uint8_t *) &normal[0]) + (normal.size() * CHUNK_SEGMENT_WIDTH_NORMAL * sizeof(GLfloat))),
					GL_STATIC_DRAW), CHUNK_INDEX_NORMAL, CHUNK_SEGMENT_WIDTH_NORMAL, GL_FLOAT);
				arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &vertex[0],
					((uint8_t *) &vertex[0]) + (vertex.size() * CHUNK_SEGMENT_WIDTH_VERTEX * sizeof(GLfloat))),
					GL_STATIC_DRAW), CHUNK_INDEX_VERTEX, CHUNK_SEGMENT_WIDTH_VERTEX, GL_FLOAT);
				arr.enable(CHUNK_INDEX_COORDINATE);
				arr.enable(CHUNK_INDEX_NORMAL);
				arr.enable(CHUNK_INDEX_VERTEX);
			}

#ifdef VIEW_SELECTIVE_SHOW
			glm::vec3 camera_position = ((nomic::entity::camera *) camera)->position();
			glm::ivec2 camera_position_chunk = glm::ivec2(camera_position.x / CHUNK_WIDTH, camera_position.z / CHUNK_WIDTH);
			glm::ivec2 position_chunk = nomic::entity::chunk::position();
			int32_t dx = (position_chunk.x - camera_position_chunk.x), dz = (position_chunk.y - camera_position_chunk.y);
			shown = ((std::abs(dx * dx) + std::abs(dz * dz)) <= (VIEW_RADIUS_RUNTIME * VIEW_RADIUS_RUNTIME));
#endif // VIEW_SELECTIVE_SHOW
			show(shown);

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
			m_chunk_back = back;
			m_chunk_front = front;
			m_chunk_left = left;
			m_chunk_right = right;
			position.x = (nomic::terrain::chunk::position().x * (CHUNK_WIDTH - 1));
			position.z = (nomic::terrain::chunk::position().y * (CHUNK_WIDTH - 1));

			for(int32_t z = 0; z < CHUNK_WIDTH; ++z) {

				for(int32_t x = 0; x < CHUNK_WIDTH; ++x) {

					for(int32_t y = (CHUNK_HEIGHT - 1); y >= 0; --y) {
						uint8_t attributes;
						uint32_t count = 0, type;
						glm::vec3 position_relative = glm::vec3(x, y, z);

						type = nomic::terrain::chunk::type(position_relative);
						attributes = nomic::terrain::chunk::attributes(position_relative);

						if((type != BLOCK_AIR) && !(attributes & BLOCK_ATTRIBUTE_HIDDEN)) {
							std::map<uint8_t, chunk_data>::iterator face_iter;

							if(nomic::utility::block_decoration(type)) { // decoration
								face_iter = add_face_type(type, BLOCK_FACE_RIGHT, attributes); // right-to-left
								add_face_decoration(position + position_relative, BLOCK_FACE_RIGHT, attributes,
									face_iter);
								++count;
								face_iter = add_face_type(type, BLOCK_FACE_LEFT, attributes); // left-to-right
								add_face_decoration(position + position_relative, BLOCK_FACE_LEFT, attributes,
									face_iter);
								++count;
							} else {
								bool small = nomic::utility::block_small(type);

								if((type == BLOCK_CLOUD) || (type == BLOCK_WATER)) { // cloud/water

									if(right) {

										if((x + 1) < CHUNK_WIDTH) { // right

											if(nomic::terrain::chunk::type(glm::uvec3(x + 1, y, z))
													== BLOCK_AIR) {
												face_iter = add_face_type(type, BLOCK_FACE_RIGHT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_RIGHT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_RIGHT, attributes,
														face_iter);
												}

												++count;
											}
										} else if((x + 1) == CHUNK_WIDTH) { // right (boundary)

											if(right->block_type(glm::uvec3(0, y, z)) == BLOCK_AIR) {
												face_iter = add_face_type(type, BLOCK_FACE_RIGHT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_RIGHT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_RIGHT, attributes,
														face_iter);
												}

												++count;
											}
										}
									}

									if(left) {

										if((x - 1) >= 0) { // left

											if(nomic::terrain::chunk::type(glm::uvec3(x - 1, y, z))
													== BLOCK_AIR) {
												face_iter = add_face_type(type, BLOCK_FACE_LEFT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_LEFT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_LEFT, attributes,
														face_iter);
												}

												++count;
											}
										} else if((x - 1) < 0) { // left (boundary)

											if(left->block_type(glm::uvec3(CHUNK_WIDTH - 1, y, z))
													== BLOCK_AIR) {
												face_iter = add_face_type(type, BLOCK_FACE_LEFT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_LEFT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_LEFT, attributes,
														face_iter);
												}

												++count;
											}
										}
									}

									if(back) {

										if((z + 1) < CHUNK_WIDTH) { // back

											if(nomic::terrain::chunk::type(glm::uvec3(x, y, z + 1))
													== BLOCK_AIR) {
												face_iter = add_face_type(type, BLOCK_FACE_BACK,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_BACK, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_BACK, attributes,
														face_iter);
												}

												++count;
											}
										} else if((z + 1) == CHUNK_WIDTH) { // back (boundary)

											if(back->block_type(glm::uvec3(x, y, 0)) == BLOCK_AIR) {
												face_iter = add_face_type(type, BLOCK_FACE_BACK,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_BACK, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_BACK, attributes,
														face_iter);
												}

												++count;
											}
										}
									}

									if(front) {

										if((z - 1) >= 0) { // front

											if(nomic::terrain::chunk::type(glm::uvec3(x, y, z - 1))
													== BLOCK_AIR) {
												face_iter = add_face_type(type, BLOCK_FACE_FRONT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_FRONT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_FRONT, attributes,
														face_iter);
												}

												++count;
											}
										} else if((z - 1) < 0) { // front (boundary)

											if(front->block_type(glm::uvec3(x, y, CHUNK_WIDTH - 1))
													== BLOCK_AIR) {
												face_iter = add_face_type(type, BLOCK_FACE_FRONT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_FRONT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_FRONT, attributes,
														face_iter);
												}

												++count;
											}
										}
									}
								} else { // other

									if(right) {

										if((x + 1) < CHUNK_WIDTH) { // right

											if(nomic::utility::block_transparent(
													nomic::terrain::chunk::type(
													glm::uvec3(x + 1, y, z)))) {
												face_iter = add_face_type(type, BLOCK_FACE_RIGHT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_RIGHT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_RIGHT, attributes,
														face_iter);
												}

												++count;
											}
										} else if((x + 1) == CHUNK_WIDTH) { // right (boundary)

											if(nomic::utility::block_transparent(right->block_type(
													glm::uvec3(0, y, z)))) {
												face_iter = add_face_type(type, BLOCK_FACE_RIGHT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_RIGHT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_RIGHT, attributes,
														face_iter);
												}

												++count;
											}
										}
									}

									if(left) {

										if((x - 1) >= 0) { // left

											if(nomic::utility::block_transparent(
													nomic::terrain::chunk::type(
													glm::uvec3(x - 1, y, z)))) {
												face_iter = add_face_type(type, BLOCK_FACE_LEFT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_LEFT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_LEFT, attributes,
														face_iter);
												}

												++count;
											}
										} else if((x - 1) < 0) { // left (boundary)

											if(nomic::utility::block_transparent(left->block_type(
													glm::uvec3(CHUNK_WIDTH - 1, y, z)))) {
												face_iter = add_face_type(type, BLOCK_FACE_LEFT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_LEFT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_LEFT, attributes,
														face_iter);
												}

												++count;
											}
										}
									}

									if(back) {

										if((z + 1) < CHUNK_WIDTH) { // back

											if(nomic::utility::block_transparent(
													nomic::terrain::chunk::type(glm::uvec3(
													x, y, z + 1)))) {
												face_iter = add_face_type(type, BLOCK_FACE_BACK,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_BACK, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_BACK, attributes,
														face_iter);
												}

												++count;
											}
										} else if((z + 1) == CHUNK_WIDTH) { // back (boundary)

											if(nomic::utility::block_transparent(back->block_type(
													glm::uvec3(x, y, 0)))) {
												face_iter = add_face_type(type, BLOCK_FACE_BACK,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_BACK, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_BACK, attributes,
														face_iter);
												}

												++count;
											}
										}
									}

									if(front) {

										if((z - 1) >= 0) { // front

											if(nomic::utility::block_transparent(
													nomic::terrain::chunk::type(
													glm::uvec3(x, y, z - 1)))) {
												face_iter = add_face_type(type, BLOCK_FACE_FRONT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_FRONT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_FRONT, attributes,
														face_iter);
												}

												++count;
											}
										} else if((z - 1) < 0) { // front (boundary)

											if(nomic::utility::block_transparent(front->block_type(
													glm::uvec3(x, y, CHUNK_WIDTH - 1)))) {
												face_iter = add_face_type(type, BLOCK_FACE_FRONT,
													attributes);

												if(small) {
													add_face_small(position + position_relative,
														BLOCK_FACE_FRONT, attributes,
														face_iter);
												} else {
													add_face(position + position_relative,
														BLOCK_FACE_FRONT, attributes,
														face_iter);
												}

												++count;
											}
										}
									}
								}

								if((y - 1) >= 0) { // bottom

									if(nomic::utility::block_transparent(nomic::terrain::chunk::type(
											glm::uvec3(x, y - 1, z)))) {
										face_iter = add_face_type(type, BLOCK_FACE_BOTTOM, attributes);

										if(small) {
											add_face_small(position + position_relative, BLOCK_FACE_BOTTOM,
												attributes, face_iter);
										} else {
											add_face(position + position_relative, BLOCK_FACE_BOTTOM,
												attributes, face_iter);
										}

										++count;
									}
								}

								if((type != BLOCK_CLOUD) && ((y + 1) < CHUNK_HEIGHT)) { // top

									if(nomic::utility::block_transparent(nomic::terrain::chunk::type(
											glm::uvec3(x, y + 1, z)))) {
										face_iter = add_face_type(type, BLOCK_FACE_TOP, attributes);

										if(small) {
											add_face_small(position + position_relative, BLOCK_FACE_TOP,
												attributes, face_iter);
										} else {
											add_face(position + position_relative, BLOCK_FACE_TOP,
												attributes, face_iter);
										}

										++count;
									}
								}
							}

							if(!count) {
								continue;
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

			m_chunk_back = nullptr;
			m_chunk_front = nullptr;
			m_chunk_left = nullptr;
			m_chunk_right = nullptr;
			m_face.clear();

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
		chunk::update(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_changed = true;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		chunk::update(
			__in nomic::entity::chunk *right,
			__in nomic::entity::chunk *left,
			__in nomic::entity::chunk *back,
			__in nomic::entity::chunk *front
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
