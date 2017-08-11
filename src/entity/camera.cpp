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
				m_falling(false),
				m_fov(fov),
				m_jump_timeout(CAMERA_JUMP_TIMEOUT),
				m_rotation_previous(rotation.x, rotation.y),
				m_sprinting(false),
				m_velocity(0.f, 0.f, 0.f),
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
				m_falling(other.m_falling),
				m_fov(other.m_fov),
				m_jump_timeout(other.m_jump_timeout),
				m_key(other.m_key),
				m_motion(other.m_motion),
				m_position_block(other.m_position_block),
				m_position_block_previous(other.m_position_block_previous),
				m_position_chunk(other.m_position_chunk),
				m_position_chunk_previous(other.m_position_chunk_previous),
				m_rotation_previous(other.m_rotation_previous),
				m_sprinting(other.m_sprinting),
				m_velocity(other.m_velocity),
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
				m_falling = other.m_falling;
				m_fov = other.m_fov;
				m_jump_timeout = other.m_jump_timeout;
				m_key = other.m_key;
				m_motion = other.m_motion;
				m_position_block = other.m_position_block;
				m_position_block_previous = other.m_position_block_previous;
				m_position_chunk = other.m_position_chunk;
				m_position_chunk_previous = other.m_position_chunk_previous;
				m_rotation_previous = other.m_rotation_previous;
				m_sprinting = other.m_sprinting;
				m_velocity = other.m_velocity;
				m_wheel = other.m_wheel;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		glm::uvec3 
		camera::block(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%u, %u, %u}", m_position_block.x, m_position_block.y, m_position_block.z);
			return m_position_block;
		}

		bool 
		camera::block_changed(void)
		{
			bool result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = (m_position_block != m_position_block_previous);
			if(result) {
				m_position_block_previous = m_position_block;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		glm::uvec3 
		camera::block_previous(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%u, %u, %u}", m_position_block_previous.x, m_position_block_previous.y,
				m_position_block_previous.z);
			return m_position_block_previous;
		}

		glm::vec3 
		camera::block_position(void)
		{
			glm::vec3 result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result.x = (m_position_chunk.x * CHUNK_WIDTH);
			result.x += m_position_block.x;
			result.y = m_position_block.y;
			result.z = (m_position_chunk.y * CHUNK_WIDTH);
			result.z += m_position_block.z;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%f, %f, %f}", result.x, result.y, result.z);
			return result;
		}

		glm::ivec2 
		camera::chunk(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%i, %i}", m_position_chunk.x, m_position_chunk.y);
			return m_position_chunk;
		}

		bool 
		camera::chunk_changed(void)
		{
			bool result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = (m_position_chunk != m_position_chunk_previous);
			if(result) {
				m_position_chunk_previous = m_position_chunk;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		glm::ivec2 
		camera::chunk_previous(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%i, %i}", m_position_chunk_previous.x, m_position_chunk_previous.y);
			return m_position_chunk_previous;
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

			std::map<uint16_t, std::pair<uint16_t, bool>>::iterator iter = m_key.find(scancode);
			if(iter == m_key.end()) {
				m_key.insert(std::make_pair(scancode, std::make_pair(modifier, state == SDL_PRESSED)));
			} else {

				if(iter->second.first != modifier) {
					iter->second.first = modifier;
				}

				bool pressed = (state == SDL_PRESSED);
				if(iter->second.second != pressed) {
					iter->second.second = pressed;
				}
			}

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

			m_motion.x += x_relative;
			m_motion.y += y_relative;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		camera::move(
			__in const glm::ivec2 &chunk,
			__in const glm::uvec3 &block
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Chunk={%i, %i}, Block={%u, %u, %u}", chunk.x, chunk.y, block.x, block.y, block.z);

			position() = nomic::utility::block_as_position(chunk, block);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		float 
		camera::pitch(void)
		{
			float result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = m_rotation_previous.x;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%f", result);
			return result;
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
					<< ", Velocity={" << m_velocity.x << ", " << m_velocity.y << ", " << m_velocity.z << "}"
					<< ", State=" << (m_falling ? "Falling" : "Not-Falling") << ", " << (m_sprinting ? "Sprinting" : "Walking")
					<< ", FOV=" << m_fov;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		camera::update(void)
		{
			bool debug, underwater;
			glm::uvec3 position_block;
			glm::ivec2 position_chunk;
			glm::vec3 position, position_relative = glm::vec3(0.f);
			float sensitivity = CAMERA_SENSITIVITY, speed, strafe;

			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::session::manager &instance = nomic::session::manager::acquire();
			if(instance.initialized()) {
				debug = instance.debug();
				underwater = instance.underwater();

				speed = (debug ? CAMERA_SPEED_DEBUG : (!underwater ? (!m_sprinting ? CAMERA_SPEED_NORMAL
					: CAMERA_SPEED_NORMAL_SPRINT) : CAMERA_SPEED_UNDERWATER));
				strafe = (debug ? CAMERA_STRAFE_DEBUG : (!underwater ? (!m_sprinting ? CAMERA_STRAFE_NORMAL
					: CAMERA_STRAFE_NORMAL_SPRINT) : CAMERA_STRAFE_UNDERWATER));

				for(std::map<uint16_t, std::pair<uint16_t, bool>>::iterator iter = m_key.begin(); iter != m_key.end(); ++iter) {

					if(iter->second.second) {

						switch(iter->first) {
							case KEY_BACKWARD: // backward
								position_relative -= (m_rotation * speed);
								break;
							case KEY_DEBUG: // enable/disable debug mode

								if(instance.toggle_debug()) {
									m_velocity = glm::vec3(0.f);
								}

								iter->second.second = false;
								break;
							case KEY_DESCEND: // down

								if(debug) {
									position_relative.y -= speed;
								}
								break;
							case KEY_ELEVATE: // up

								if(!debug) {

									if(!underwater) {

										if(!m_falling && (m_jump_timeout >= CAMERA_JUMP_TIMEOUT)
												&& !m_velocity.y) {
											m_falling = true;
											m_jump_timeout = 0;
											m_velocity.y = CAMERA_JUMP_MAX;
										}
									} else {
										m_velocity.y = CAMERA_SWIM_MAX;
									}
								} else {
									position_relative.y += speed;
								}
								break;
							case KEY_FORWARD: // forward
								position_relative += (m_rotation * speed);
								break;
							case KEY_LEFT: // left
							case KEY_LEFT_STRAFE:
								position_relative -= (glm::normalize(glm::cross(m_rotation, m_up)) * strafe);
								break;
							case KEY_RESET: // reset camera to spawn
								position_relative = instance.spawn();
								iter->second.second = false;
								break;
							case KEY_RIGHT: // right
							case KEY_RIGHT_STRAFE:
								position_relative += (glm::normalize(glm::cross(m_rotation, m_up)) * strafe);
								break;
							case KEY_SPRINT: // sprint (enable)

								if(!debug && !m_sprinting) {
									m_sprinting = true;
								}
								break;
							default:
								break;
						}
					} else {

						switch(iter->first) {
							case KEY_SPRINT: // sprint (disable)

								if(!debug && m_sprinting) {
									m_sprinting = false;
								}
								break;
							default:
								break;
						}
					}
				}

				if(!debug) {
					position_relative.y = 0.f;
				}

				position = (m_position + position_relative);
				if(position.y < CAMERA_HEIGHT_OFFSET) {
					position.y = CAMERA_HEIGHT_OFFSET;
				} else if(position.y > CHUNK_HEIGHT) {
					position.y = CHUNK_HEIGHT;
				}

				nomic::utility::position_as_block(position, position_chunk, position_block);

				if(!debug) {
					uint8_t type;
					nomic::terrain::manager &terrain = instance.terrain();
					float drag = (!underwater ? CAMERA_DRAG_NORMAL : CAMERA_DRAG_UNDERWATER),
						gravity = (!underwater ? CAMERA_GRAVITY_NORMAL : CAMERA_GRAVITY_UNDERWATER),
						gravity_step = (!underwater ? CAMERA_GRAVITY_STEP_NORMAL : CAMERA_GRAVITY_STEP_UNDERWATER);

					if(m_jump_timeout < CAMERA_JUMP_TIMEOUT) {
						++m_jump_timeout;
					}

					if(underwater && (m_velocity.y < (CAMERA_GRAVITY_UNDERWATER + CAMERA_GRAVITY_STEP_UNDERWATER))) {
						m_velocity.y = 0.f;
					}

					if(m_velocity.y > gravity) {
						m_velocity.y += gravity_step;
					}

					type = terrain.at(position_chunk)->block_type(position_block);
					if((type != BLOCK_AIR) && (type != BLOCK_WATER)) { // top

						if(m_velocity.y > 0.f) {
							m_velocity.y = 0.f;
						}
					}

					type = terrain.at(position_chunk)->block_type(position_block + glm::uvec3(0.f, -CAMERA_HEIGHT_OFFSET, 0.f));
					if((type != BLOCK_AIR) && (type != BLOCK_WATER)) { // bottom

						if(m_velocity.y < 0.f) {
							m_falling = false;
							m_velocity.y = 0.f;
						}
					}

					// TODO: collision detect block sides

					position += m_velocity;

					if(m_velocity.x > drag) {
						m_velocity.x -= drag;
					} else if(m_velocity.x < -drag) {
						m_velocity.x += drag;
					} else {
						m_velocity.x = 0.f;
					}

					if(m_velocity.z > drag) {
						m_velocity.z -= drag;
					} else if(m_velocity.z < -drag) {
						m_velocity.z += drag;
					} else {
						m_velocity.z = 0.f;
					}

					if(position.y < CAMERA_HEIGHT_OFFSET) {
						position.y = CAMERA_HEIGHT_OFFSET;
					} else if(position.y > CHUNK_HEIGHT) {
						position.y = CHUNK_HEIGHT;
					}
				}

				m_position = position;
				m_position_block = position_block;
				m_position_chunk = position_chunk;

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
			}

			instance.release();

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
			m_view = glm::lookAt(m_position, m_position + m_rotation, m_up);

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

			if(direction == SDL_MOUSEWHEEL_FLIPPED) {
				y *= -1;
			}

			m_wheel += y;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		float 
		camera::yaw(void)
		{
			float result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = m_rotation_previous.y;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%f", result);
			return result;
		}
	}
}
