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

#include "../../include/entity/reticle.h"
#include "../../include/trace.h"
#include "./reticle_type.h"

namespace nomic {

	namespace entity {

		#define RETICLE_RATIO (DISPLAY_DEFAULT_HEIGHT / (float) DISPLAY_DEFAULT_WIDTH)
		#define RETICLE_SEGMENT_COUNT 4
		#define RETICLE_SEGMENT_WIDTH 3
		#define RETICLE_WIDTH 0.04f

		const float RETICLE_COLOR[] = {
			0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, // grey
			0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, // grey
			};

		const float RETICLE_VERTEX[] = {
			-RETICLE_WIDTH * RETICLE_RATIO, 0.f, 0.f, RETICLE_WIDTH * RETICLE_RATIO, 0.f, 0.f, // horizontal
			0.0f, -RETICLE_WIDTH, 0.f, 0.f, RETICLE_WIDTH, 0.f,  // vertical
			};

		enum {
			RETICLE_INDEX_COLOR = 0,
			RETICLE_INDEX_VERTEX,
		};

		reticle::reticle(void) :
			nomic::entity::object(ENTITY_AXIS)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::graphic::vao &arr = vertex_array();
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &RETICLE_COLOR[0],
				((uint8_t *) &RETICLE_COLOR[0]) + ((RETICLE_SEGMENT_COUNT * RETICLE_SEGMENT_WIDTH) * sizeof(GLfloat))), GL_STATIC_DRAW),
				RETICLE_INDEX_COLOR, RETICLE_SEGMENT_WIDTH, GL_FLOAT);
			arr.add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &RETICLE_VERTEX[0],
				((uint8_t *) &RETICLE_VERTEX[0]) + ((RETICLE_SEGMENT_COUNT * RETICLE_SEGMENT_WIDTH) * sizeof(GLfloat))), GL_STATIC_DRAW),
				RETICLE_INDEX_VERTEX, RETICLE_SEGMENT_WIDTH, GL_FLOAT);
			arr.enable(RETICLE_INDEX_COLOR);
			arr.enable(RETICLE_INDEX_VERTEX);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		reticle::reticle(
			__in const reticle &other
			) :
				nomic::entity::object(other)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		reticle::~reticle(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			vertex_array().disable_all();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		reticle &
		reticle::operator=(
			__in const reticle &other
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
		reticle::on_render(
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f", delta);

			vertex_array().bind();
			GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_LINES, 0, RETICLE_SEGMENT_COUNT);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		reticle::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_RETICLE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
