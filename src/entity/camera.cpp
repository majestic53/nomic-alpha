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

#include "../../include/entity/camera.h"
#include "../../include/session/manager.h"
#include "../../include/trace.h"
#include "./camera_type.h"

namespace nomic {

	namespace entity {

		camera::camera(
			__in const glm::uvec2 &dimensions,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up,
			__in_opt float fov
			) :
				nomic::entity::object(ENTITY_CAMERA, SUBTYPE_UNDEFINED, position, rotation, up),
				m_dimensions(dimensions),
				m_fov(fov),
				m_rotation_previous(rotation.x, rotation.y),
				m_wheel(0)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimensions={%u, %u}, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}, fov=%f",
				dimensions.x, dimensions.y, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z,
				up.x, up.y, up.z, fov);

			update_perspective();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		camera::camera(
			__in const camera &other
			) :
				nomic::entity::object(other),
				m_dimensions(other.m_dimensions),
				m_fov(other.m_fov),
				m_key(other.m_key),
				m_motion(other.m_motion),
				m_rotation_previous(other.m_rotation_previous),
				m_wheel(other.m_wheel)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimensions={%u, %u}, Position={%f, %f, %f}, Rotation={%f, %f, %f}, Up={%f, %f, %f}, fov=%f",
				other.m_dimensions.x, other.m_dimensions.y, other.m_position.x, other.m_position.y, other.m_position.z,
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
				other.m_dimensions.x, other.m_dimensions.y, other.m_position.x, other.m_position.y, other.m_position.z,
				other.m_rotation.x, other.m_rotation.y, other.m_rotation.z,
				other.m_up.x, other.m_up.y, other.m_up.z, other.m_fov);

			if(this != &other) {
				nomic::entity::object::operator=(other);
				m_dimensions = other.m_dimensions;
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
		camera::button(
			__in uint8_t button,
			__in uint8_t state,
			__in uint8_t clicks,
			__in int32_t x,
			__in int32_t y
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Button=%x, State=%x(%s), Clicks=%u, Position={%i, %i}", button, state,
				(state == SDL_PRESSED) ? "Press" : "Release", (uint16_t) clicks, x, y);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		glm::uvec2 
		camera::dimensions(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return m_dimensions;
		}

		float 
		camera::fov(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%f", m_fov);
			return m_fov;
		}

		void 
		camera::key(
			__in uint16_t scancode,
			__in uint16_t modifier,
			__in uint8_t state
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Scancode=%x, Modifier=%x, State=%x(%s)", scancode, modifier, state,
				(state == SDL_PRESSED) ? "Press" : "Release");

			on_key(scancode, modifier, state);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		camera::motion(
			__in uint32_t state,
			__in int32_t x,
			__in int32_t y,
			__in int32_t x_relative,
			__in int32_t y_relative
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "State=%x, Position={%i, %i}, Relative={%i, %i}", state, x, y, x_relative, y_relative);

			on_motion(state, x, y, x_relative, y_relative);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		camera::on_button(
			__in uint8_t button,
			__in uint8_t state,
			__in uint8_t clicks,
			__in int32_t x,
			__in int32_t y
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Button=%x, State=%x(%s), Clicks=%u, Position={%i, %i}", button, state,
				(state == SDL_PRESSED) ? "Press" : "Release", (uint16_t) clicks, x, y);
			TRACE_EXIT(LEVEL_VERBOSE);
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
		camera::set_dimensions(
			__in const glm::uvec2 &dimensions
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimension={%u, %u}", dimensions.x, dimensions.y);

			m_dimensions = dimensions;
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

			result << NOMIC_ENTITY_CAMERA_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nomic::entity::object::to_string(verbose)
					<< ", Dimension={" << m_dimensions.x << ", " << m_dimensions.y << "}"
					<< ", FOV=" << m_fov;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		camera::update(void)
		{
			float sensitivity = CAMERA_SENSITIVITY, speed = CAMERA_SPEED, strafe = CAMERA_STRAFE;

			TRACE_ENTRY(LEVEL_VERBOSE);

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
					case KEY_DESCEND:
						m_position.y -= speed;
						break;
					case KEY_ELEVATE:
						m_position.y += speed;
						break;
					case KEY_FORWARD:
						m_position += (m_rotation * speed);
						break;
					case KEY_LEFT:
					case KEY_LEFT_STRAFE:
						m_position -= (glm::normalize(glm::cross(m_rotation, m_up)) * strafe);
						break;
					case KEY_RIGHT:
					case KEY_RIGHT_STRAFE:
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

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		camera::update_perspective(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_projection = glm::perspective(glm::radians(m_fov), m_dimensions.x / (float) m_dimensions.y,
				CAMERA_CLIP_MIN, CAMERA_CLIP_MAX);
			m_rotation = glm::normalize(glm::vec3(
				std::cos(glm::radians(m_rotation_previous.x)) * std::sin(glm::radians(m_rotation_previous.y)),
				std::sin(glm::radians(m_rotation_previous.x)),
				std::cos(glm::radians(m_rotation_previous.x)) * std::cos(glm::radians(m_rotation_previous.y))));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		glm::mat4 &
		camera::view(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_view = glm::lookAt(m_position, m_position + m_rotation, m_up);

			TRACE_EXIT(LEVEL_VERBOSE);
			return m_view;
		}

		void 
		camera::wheel(
			__in uint32_t direction,
			__in int32_t x,
			__in int32_t y
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Direction=%x(%s), Position={%i, %i}", direction,
				(direction == SDL_MOUSEWHEEL_NORMAL) ? "Normal" : "Flipped", x, y);

			on_wheel(direction, x, y);

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
