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

#include "../../include/graphic/camera.h"
#include "../../include/session/manager.h"
#include "../../include/trace.h"
#include "./camera_type.h"

namespace nomic {

	namespace graphic {

		camera::camera(
			__in const glm::uvec2 &dimension,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up,
			__in_opt float fov
			) :
				nomic::entity::object(ENTITY_CAMERA, SUBTYPE_UNDEFINED, position, rotation, up),
				m_dimension(dimension),
				m_fov(fov),
				m_rotation_previous(rotation.x, rotation.y),
				m_wheel(0)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimensions={%u, %u}, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}, fov=%f",
				dimension.x, dimension.y, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z,
				up.x, up.y, up.z, fov);

			update_perspective();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		camera::camera(
			__in const camera &other
			) :
				nomic::entity::object(other),
				m_dimension(other.m_dimension),
				m_fov(other.m_fov),
				m_key(other.m_key),
				m_motion(other.m_motion),
				m_rotation_previous(other.m_rotation_previous),
				m_wheel(other.m_wheel)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimensions={%u, %u}, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}, fov=%f",
				other.m_dimension.x, other.m_dimension.y, other.m_position.x, other.m_position.y, other.m_position.z,
				other.m_rotation.x, other.m_rotation.y, other.m_rotation.z,
				other.m_up.x, other.m_up.y, other.m_up.z, other.m_fov);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		camera::~camera(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		camera &
		camera::operator=(
			__in const camera &other
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimensions={%u, %u}, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}, fov=%f",
				other.m_dimension.x, other.m_dimension.y, other.m_position.x, other.m_position.y, other.m_position.z,
				other.m_rotation.x, other.m_rotation.y, other.m_rotation.z,
				other.m_up.x, other.m_up.y, other.m_up.z, other.m_fov);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				m_dimension = other.m_dimension;
				m_fov = other.m_fov;
				m_key = other.m_key;
				m_motion = other.m_motion;
				m_rotation_previous = other.m_rotation_previous;
				m_wheel = other.m_wheel;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		void 
		camera::on_key(
			__in uint16_t scancode,
			__in uint16_t modifier,
			__in uint8_t state
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Scancode=%x, Modifier=%x, State=%x(%s)", scancode, modifier, state,
				(state == SDL_PRESSED) ? "Press" : "Release");

			std::pair<uint16_t, uint16_t> key_pair = std::make_pair(scancode, modifier);

			std::map<std::pair<uint16_t, uint16_t>, bool>::iterator iter = m_key.find(key_pair);
			if(iter == m_key.end()) {
				m_key.insert(std::make_pair(key_pair, state == SDL_PRESSED));
			} else {
				iter->second = (state == SDL_PRESSED);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		camera::on_motion(
			__in uint32_t state,
			__in int32_t x,
			__in int32_t y,
			__in int32_t x_relative,
			__in int32_t y_relative
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "State=%x, Position={%i, %i}, Relative={%i, %i}", state, x, y, x_relative, y_relative);

			m_motion.x += x_relative;
			m_motion.y += y_relative;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		camera::on_wheel(
			__in uint32_t direction,
			__in int32_t x,
			__in int32_t y
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Direction=%x(%s), Position={%i, %i}", direction,
				(direction == SDL_MOUSEWHEEL_NORMAL) ? "Normal" : "Flipped", x, y);

			m_wheel += y;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		camera::render(
			__in float delta
			)
		{
			float sensitivity = (CAMERA_SENSITIVITY * delta), speed = (CAMERA_SPEED * delta), strafe = (CAMERA_STRAFE * delta);

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f", delta);

			nomic::event::input::poll_input();

			for(std::map<std::pair<uint16_t, uint16_t>, bool>::iterator iter = m_key.begin(); iter != m_key.end(); ++iter) {

				if(!iter->second) {
					continue;
				}

				switch(iter->first.first) {
					case KEY_BACKWARD:
						m_position -= (m_rotation * speed);
						break;
					case KEY_DEBUG: {

							nomic::session::manager &instance = nomic::session::manager::acquire();
							if(instance.initialized()) {
								instance.toggle_debug();
							}

							instance.release();
							iter->second = false;
						} break;
					case KEY_FORWARD:
						m_position += (m_rotation * speed);
						break;
					case KEY_LEFT:
						m_position -= (glm::normalize(glm::cross(m_rotation, m_up)) * strafe);
						break;
					case KEY_RIGHT:
						m_position += (glm::normalize(glm::cross(m_rotation, m_up)) * strafe);
						break;
					default:
						break;
				}
			}

			if(m_motion != glm::vec2()) {

				m_rotation_previous.x -= (m_motion.y * sensitivity);
				if(m_rotation_previous.x < CAMERA_PITCH_MIN) {
					m_rotation_previous.x = CAMERA_PITCH_MIN;
				} else if(m_rotation_previous.x > CAMERA_PITCH_MAX) {
					m_rotation_previous.x = CAMERA_PITCH_MAX;
				}

				m_rotation_previous.y -= (m_motion.x * sensitivity);
				if(m_rotation_previous.y < CAMERA_YAW_MIN) {
					m_rotation_previous.y = CAMERA_YAW_MAX;
				} else if(m_rotation_previous.y > CAMERA_YAW_MAX) {
					m_rotation_previous.y = CAMERA_YAW_MIN;
				}

				m_rotation = glm::normalize(glm::vec3(
					std::cos(glm::radians(m_rotation_previous.x)) * std::sin(glm::radians(m_rotation_previous.y)),
					std::sin(glm::radians(m_rotation_previous.x)),
					std::cos(glm::radians(m_rotation_previous.x)) * std::cos(glm::radians(m_rotation_previous.y))));
				m_motion = glm::vec2();
			}

			if(m_wheel) {

				m_fov -= m_wheel;
				if(m_fov < CAMERA_FOV_MIN) {
					m_fov = CAMERA_FOV_MIN;
				} else if(m_fov > CAMERA_FOV_MAX) {
					m_fov = CAMERA_FOV_MAX;
				}

				update_perspective();
				m_wheel = 0.f;
			}

			m_view = glm::lookAt(m_position, m_position + m_rotation, m_up);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		camera::set_dimensions(
			__in const glm::uvec2 &dimension
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimension={%u, %u}", dimension.x, dimension.y);

			m_dimension = dimension;
			update_perspective();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		camera::set_fov(
			__in float fov
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Fov=%f", fov);

			m_fov = fov;
			update_perspective();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		camera::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_CAMERA_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Dimension={" << m_dimension.x << ", " << m_dimension.y << "}"
					<< ", FOV=" << m_fov;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		camera::update_perspective(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_projection = glm::perspective(glm::radians(m_fov), m_dimension.x / (float) m_dimension.y,
				CAMERA_CLIP_MIN, CAMERA_CLIP_MAX);
			m_rotation = glm::normalize(glm::vec3(
				std::cos(glm::radians(m_rotation_previous.x)) * std::sin(glm::radians(m_rotation_previous.y)),
				std::sin(glm::radians(m_rotation_previous.x)),
				std::cos(glm::radians(m_rotation_previous.x)) * std::cos(glm::radians(m_rotation_previous.y))));
			m_view = glm::lookAt(m_position, m_position + m_rotation, m_up);

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
