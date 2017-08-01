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

#include "../../include/entity/skybox.h"
#include "../../include/runtime.h"
#include "../../include/trace.h"
#include "./skybox_type.h"

namespace nomic {

	namespace entity {

		#define SKYBOX_SEGMENT_COUNT 36
		#define SKYBOX_SEGMENT_WIDTH 3

		enum {
			SKYBOX_INDEX_VERTEX = 0,
		};

		static const glm::vec3 SKYBOX_VERTEX[] = {
			{ -SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS, }, //right
			{ -SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS, }, // left
			{ -SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS, }, // top
			{ SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS, }, // bottom
			{ -SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS, }, // back
			{ SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS, }, // front
			{ -SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS, },
			{ -SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS, },
			{ SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS, },
			};

		skybox::skybox(
			__in_opt const std::map<uint32_t, std::string> &path,
			__in_opt float scale,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum wrap_r,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			) :
				nomic::entity::object(ENTITY_SKYBOX),
				nomic::graphic::cubemap(path, scale, wrap_s, wrap_t, wrap_r, filter_min, filter_mag)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u], Scale=%f, Wrap={%x, %x, %x}, Filter={%x, %x}", path.size(), scale,
				wrap_s, wrap_t, wrap_r, filter_min, filter_mag);

			setup();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		skybox::skybox(
			__in const skybox &other
			) :
				nomic::entity::object(other),
				nomic::graphic::cubemap(other)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			setup();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		skybox::~skybox(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		skybox &
		skybox::operator=(
			__in const skybox &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				nomic::graphic::cubemap::operator=(other);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		skybox::on_render(
			__in nomic::core::renderer &renderer,
			__in void *textures,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Textures=%p, Delta=%f", &renderer, textures, delta);

			nomic::graphic::cubemap::bind();
			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			arr.enable(SKYBOX_INDEX_VERTEX);
			GL_CHECK(LEVEL_WARNING, glEnable, GL_TEXTURE_CUBE_MAP_SEAMLESS);
			GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, SKYBOX_SEGMENT_COUNT);
			GL_CHECK(LEVEL_WARNING, glDisable, GL_TEXTURE_CUBE_MAP_SEAMLESS);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		skybox::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);

			if(runtime) {
				show(!((nomic::runtime *) runtime)->session().underwater());
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		skybox::set(
			__in const std::map<uint32_t, std::string> &path,
			__in_opt float scale,
			__in_opt GLenum wrap_s,
			__in_opt GLenum wrap_t,
			__in_opt GLenum wrap_r,
			__in_opt GLenum filter_min,
			__in_opt GLenum filter_mag
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u], Scale=%f, Wrap={%x, %x, %x}, Filter={%x, %x}", path.size(), scale,
				wrap_s, wrap_t, wrap_r, filter_min, filter_mag);

			nomic::graphic::cubemap::set(path, scale, wrap_s, wrap_t, wrap_r, filter_min, filter_mag);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		skybox::setup(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::vao &arr = vertex_array();
			arr.bind();
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &SKYBOX_VERTEX[0],
				((uint8_t *) &SKYBOX_VERTEX[0]) + (SKYBOX_SEGMENT_COUNT * SKYBOX_SEGMENT_WIDTH * sizeof(GLfloat))), GL_STATIC_DRAW),
				SKYBOX_INDEX_VERTEX, SKYBOX_SEGMENT_WIDTH, GL_FLOAT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		skybox::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_SKYBOX_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Cubemap=" << nomic::graphic::cubemap::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
