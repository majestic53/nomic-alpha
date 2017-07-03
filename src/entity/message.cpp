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

#include <algorithm>
#include "../../include/entity/message.h"
#include "../../include/entity/camera.h"
#include "../../include/trace.h"
#include "./message_type.h"

namespace nomic {

	namespace entity {

		message::message(
			__in_opt const std::string &path,
			__in_opt uint32_t size,
			__in_opt const glm::vec4 &color,
			__in_opt float scale
			) :
				nomic::entity::string(path, size, MESSAGE_POSITION_DEFAULT, color, scale),
				m_changed(true)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s, Size=%u, Color={%f, %f, %f, %f}, Scale=%f", path.size(),
				STRING_CHECK(path), size, color.x, color.y, color.z, color.w, scale);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		message::message(
			__in const message &other
			) :
				nomic::entity::string(other),
				m_changed(other.m_changed)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		message::~message(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		message &
		message::operator=(
			__in const message &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::entity::string::operator=(other);
				m_changed = other.m_changed;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		message::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			nomic::entity::camera *camera_ref = (nomic::entity::camera *) camera;
			glm::uvec2 position = MESSAGE_POSITION_DEFAULT, offset = glm::uvec2(0, 0);

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);

			if(m_changed) {
				m_changed = false;

				if(camera_ref) {
					position = camera_ref->dimensions();
					position /= 2;

					for(std::string::iterator iter = m_text.begin(); iter != m_text.end(); ++iter) {
						nomic::graphic::character &ch = m_manager_font.character(m_font_id, *iter);
						offset.x += ((ch.advance() >> STRING_ADVANCE_SHIFT) * m_scale);
						offset.y = std::max(offset.y, (uint32_t) (ch.dimensions().y * m_scale));
					}

					offset /= 2;
					position.x -= offset.x;
					position.y += offset.y;
				}

				nomic::entity::string::set_position(position);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string &
		message::text(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_changed = true;

			TRACE_EXIT(LEVEL_VERBOSE);
			return m_text;
		}

		std::string 
		message::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_MESSAGE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::string::to_string(verbose)
					<< ", State=" << (m_changed ? "Changed" : "Unchanged");
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
