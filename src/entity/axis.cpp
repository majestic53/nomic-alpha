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

#include "../../include/entity/axis.h"
#include "../../include/trace.h"
#include "./axis_type.h"

namespace nomic {

	namespace entity {

		#define AXIS_SEGMENT_COUNT 18
		#define AXIS_SEGMENT_WIDTH 3

		enum {
			AXIS_INDEX_COLOR = 0,
			AXIS_INDEX_VERTEX,
		};

		static const float AXIS_COLOR[] = {
			1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, // red x
			0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, // green y
			0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, // blue z
			};

		static const float AXIS_VERTEX[] = {
			0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.75f, 0.25f, 0.f, 1.f, 0.f, 0.f, 0.75f, -0.25f, 0.f, // x
			0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.25f, 0.75f, 0.f, 0.f, 1.f, 0.f, -0.25f, 0.75f, 0.f, // y
			0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.25f, 0.75f, 0.f, 0.f, 1.f, 0.f, -0.25f, 0.75f, // z
			};

		axis::axis(void) :
			nomic::entity::object(ENTITY_AXIS)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::vao &arr = vertex_array();
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &AXIS_COLOR[0],
				((uint8_t *) &AXIS_COLOR[0]) + ((AXIS_SEGMENT_COUNT * AXIS_SEGMENT_WIDTH) * sizeof(GLfloat))), GL_STATIC_DRAW),
				AXIS_INDEX_COLOR, AXIS_SEGMENT_WIDTH, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &AXIS_VERTEX[0],
				((uint8_t *) &AXIS_VERTEX[0]) + ((AXIS_SEGMENT_COUNT * AXIS_SEGMENT_WIDTH) * sizeof(GLfloat))), GL_STATIC_DRAW),
				AXIS_INDEX_VERTEX, AXIS_SEGMENT_WIDTH, GL_FLOAT);
			arr.enable(AXIS_INDEX_COLOR);
			arr.enable(AXIS_INDEX_VERTEX);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		axis::axis(
			__in const axis &other
			) :
				nomic::entity::object(other)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		axis::~axis(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			vertex_array().disable_all();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		axis &
		axis::operator=(
			__in const axis &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::object::operator=(other);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		axis::on_render(
			__in nomic::core::renderer &renderer,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Delta=%f", &renderer, delta);

			vertex_array().bind();
			GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_LINES, 0, AXIS_SEGMENT_COUNT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		axis::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_AXIS_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
