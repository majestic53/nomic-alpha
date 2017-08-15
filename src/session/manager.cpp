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

#include <map>
#include <tuple>
#include "../../include/session/manager.h"
#include "../../include/core/renderer.h"
#include "../../include/entity/axis.h"
#include "../../include/entity/block.h"
#include "../../include/entity/diagnostic.h"
#include "../../include/entity/message.h"
#include "../../include/entity/panel.h"
#include "../../include/entity/plain.h"
#include "../../include/entity/reticle.h"
#include "../../include/entity/selector.h"
#include "../../include/entity/skybox.h"
#include "../../include/entity/sun.h"
#include "../../include/graphic/vao.h"
#include "../../include/runtime.h"
#include "../../include/trace.h"
#include "./manager_type.h"

namespace nomic {

	namespace session {

		enum {
			RENDERER_SHADER_VERTEX = 0,
			RENDERER_SHADER_FRAGMENT,
			RENDERER_MODE,
			RENDERER_BLEND_ENABLED,
			RENDERER_BLEND_DFACTOR,
			RENDERER_BLEND_SFACTOR,
			RENDERER_CULL_ENABLED,
			RENDERER_CULL_MODE,
			RENDERER_DEPTH_ENABLED,
			RENDERER_DEPTH_MODE,
		};

		typedef std::tuple<std::string, std::string, uint32_t, bool, uint32_t, uint32_t, bool, uint32_t, bool, uint32_t> renderer_config;

		static const renderer_config BACKDROP_RENDERER_CONFIGURATION = {
			"./asset/shader/vert_plain.glsl", "./asset/shader/frag_plain.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
			RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, RENDERER_CULL_MODE_DEFAULT,
			RENDERER_DEPTH_DEFAULT, RENDERER_DEPTH_MODE_DEFAULT
			};

		static const renderer_config CHUNK_RENDERER_CONFIGURATION = {
			"./asset/shader/vert_chunk.glsl", "./asset/shader/frag_chunk.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
			RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_MODE_DEFAULT, GL_FRONT,
			RENDERER_DEPTH_DEFAULT, RENDERER_DEPTH_MODE_DEFAULT
			};

		// debug objects
		enum {
			DEBUG_OBJECT_AXIS = 0,
			DEBUG_OBJECT_BLOCK,
			DEBUG_OBJECT_DIAGNOSTIC,
		};

		#define DEBUG_OBJECT_MAX DEBUG_OBJECT_DIAGNOSTIC

		static const std::vector<renderer_config> DEBUG_RENDERER_CONFIGURATION = {
			{ "./asset/shader/vert_axis.glsl", "./asset/shader/frag_axis.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, RENDERER_CULL_MODE_DEFAULT,
				RENDERER_DEPTH_DEFAULT, RENDERER_DEPTH_MODE_DEFAULT }, // axis
			{ "./asset/shader/vert_block.glsl", "./asset/shader/frag_block.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, false, RENDERER_CULL_MODE_DEFAULT,
				RENDERER_DEPTH_DEFAULT, RENDERER_DEPTH_MODE_DEFAULT }, // block
			{ "./asset/shader/vert_string.glsl", "./asset/shader/frag_string.glsl", RENDER_ORTHOGONAL, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, RENDERER_CULL_MODE_DEFAULT,
				RENDERER_DEPTH_DEFAULT, RENDERER_DEPTH_MODE_DEFAULT }, // diagnostic
			};

		static const std::map<uint32_t, std::string> DEBUG_BLOCK_FACE = {
			{ BLOCK_FACE_RIGHT, "./asset/texture/default/debug.bmp" },
			{ BLOCK_FACE_LEFT, "./asset/texture/default/debug.bmp" },
			{ BLOCK_FACE_TOP, "./asset/texture/default/debug.bmp" },
			{ BLOCK_FACE_BOTTOM, "./asset/texture/default/debug.bmp" },
			{ BLOCK_FACE_BACK, "./asset/texture/default/debug.bmp" },
			{ BLOCK_FACE_FRONT, "./asset/texture/default/debug.bmp" },
			};

		#define DEBUG_BLOCK_SCALE 0.2f

		// entity background objects
		enum {
			ENTITY_OBJECT_BACKGROUND_SKYBOX = 0,
			ENTITY_OBJECT_BACKGROUND_SUN,
		};

		#define ENTITY_OBJECT_BACKGROUND_MAX ENTITY_OBJECT_BACKGROUND_SUN

		static const std::vector<renderer_config> ENTITY_RENDERER_BACKGROUND_CONFIGURATION = {
			{ "./asset/shader/vert_skybox.glsl", "./asset/shader/frag_skybox.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, RENDERER_CULL_MODE_DEFAULT,
				false, RENDERER_DEPTH_MODE_DEFAULT }, // skybox
			{ "./asset/shader/vert_sun.glsl", "./asset/shader/frag_sun.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, GL_FRONT,
				false, RENDERER_DEPTH_MODE_DEFAULT }, // sun
			};

		static const std::map<uint32_t, std::string> ENTITY_SKYBOX_FACE = {
			{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, "./asset/texture/default/skybox_side.bmp" },
			{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "./asset/texture/default/skybox_side.bmp" },
			{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "./asset/texture/default/skybox_top.bmp" },
			{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "./asset/texture/default/skybox_bottom.bmp" },
			{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "./asset/texture/default/skybox_side.bmp" },
			{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "./asset/texture/default/skybox_side.bmp" },
			};

		// entity foreground objects
		enum {
			ENTITY_OBJECT_FOREGROUND_PANEL = 0,
			ENTITY_OBJECT_FOREGROUND_RETICLE,
			ENTITY_OBJECT_FOREGROUND_SELECTOR,
		};

		#define ENTITY_OBJECT_FOREGROUND_MAX ENTITY_OBJECT_FOREGROUND_SELECTOR

		static const std::vector<renderer_config> ENTITY_RENDERER_FOREGROUND_CONFIGURATION = {
			{ "./asset/shader/vert_panel.glsl", "./asset/shader/frag_panel.glsl", RENDER_ORTHOGONAL, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, GL_FRONT,
				false, RENDERER_DEPTH_MODE_DEFAULT }, // panel
			{ "./asset/shader/vert_reticle.glsl", "./asset/shader/frag_reticle.glsl", RENDER_ORTHOGONAL, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, RENDERER_CULL_MODE_DEFAULT,
				false, RENDERER_DEPTH_MODE_DEFAULT }, // reticle
			{ "./asset/shader/vert_selector.glsl", "./asset/shader/frag_selector.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, GL_FRONT,
				false, RENDERER_DEPTH_MODE_DEFAULT }, // selector
			};

		static const std::vector<uint8_t> FOREGROUND_PANEL_SELECTION = {
			BLOCK_DIRT,
			BLOCK_GRASS,
			BLOCK_STONE,
			BLOCK_COBBLESTONE,
			BLOCK_SAND,
			BLOCK_GLASS,
			BLOCK_PLANK,
			BLOCK_WOOD,
			BLOCK_LEAVES,
			};

		static const std::map<SDL_GLattr, GLint> SDL_ATTRIBUTE = {
			{ SDL_GL_ACCELERATED_VISUAL, GL_ATTRIBUTE_ACCELERATE_VISUAL },
			{ SDL_GL_BLUE_SIZE, GL_ATTRIBUTE_COLOR_SIZE },
			{ SDL_GL_CONTEXT_MAJOR_VERSION, GL_ATTRIBUTE_MAJOR_VERSION },
			{ SDL_GL_CONTEXT_MINOR_VERSION, GL_ATTRIBUTE_MINOR_VERSION },
			{ SDL_GL_DEPTH_SIZE, GL_ATTRIBUTE_DEPTH_SIZE },
			{ SDL_GL_DOUBLEBUFFER, GL_ATTRIBUTE_DOUBLEBUFFER },
			{ SDL_GL_GREEN_SIZE, GL_ATTRIBUTE_COLOR_SIZE },
			{ SDL_GL_RED_SIZE, GL_ATTRIBUTE_COLOR_SIZE },
			};

		static const std::map<GLenum, std::map<GLenum, GLenum>> GRAPHIC_ATTRIBUTE = {
			{ GL_LINE_SMOOTH, { std::pair<GLenum, GLenum>(GL_LINE_SMOOTH_HINT, GL_NICEST) } },
			};

		manager::manager(void) :
			m_atlas(nullptr),
			m_block_selected(false),
			m_block_selected_face(BLOCK_FACE_UNDEFINED),
			m_camera(nullptr),
			m_chunk_renderer(nullptr),
			m_clouds(false),
			m_debug(SESSION_DEBUG_DEFAULT),
			m_manager_display(nomic::graphic::display::acquire()),
			m_manager_entity(nomic::entity::manager::acquire()),
			m_manager_font(nomic::font::manager::acquire()),
			m_manager_graphic(nomic::graphic::manager::acquire()),
			m_manager_render(nomic::render::manager::acquire()),
			m_manager_terrain(nomic::terrain::manager::acquire()),
			m_runtime(nullptr),
			m_underwater(false)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		manager::~manager(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_manager_display.release();
			m_manager_entity.release();
			m_manager_font.release();
			m_manager_graphic.release();
			m_manager_render.release();
			m_manager_terrain.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		glm::vec3 
		manager::ambient_position(void)
		{
			glm::vec3 result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			nomic::entity::sun *sun = (nomic::entity::sun *) m_entity_object_background.at(ENTITY_OBJECT_BACKGROUND_SUN);
			if(sun) {
				result = sun->position();
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%f, %f, %f}", result.x, result.y, result.z);
			return result;
		}

		bool 
		manager::block_selected(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_block_selected);
			return m_block_selected;
		}

		nomic::entity::camera *
		manager::camera(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", m_camera);
			return m_camera;
		}

		bool 
		manager::clouds(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_clouds);
			return m_clouds;
		}

		bool 
		manager::debug(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_debug);
			return m_debug;
		}

		bool 
		manager::determine_block_decoration(
			__in uint8_t type
			)
		{
			bool result = false;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

			switch(type) {
				case BLOCK_CORAL_BLUE:
				case BLOCK_CORAL_BROWN:
				case BLOCK_CORAL_ORANGE:
				case BLOCK_CORAL_PINK:
				case BLOCK_CORAL_PURPLE:
				case BLOCK_FLOWER_RED:
				case BLOCK_FLOWER_YELLOW:
				case BLOCK_GRASS_SHORT:
				case BLOCK_GRASS_TALL:
				case BLOCK_SEAGRASS_GREEN:
				case BLOCK_SEAGRASS_BROWN:
				case BLOCK_SHRUB:
					result = true;
					break;
				default:
					break;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		manager::determine_block_selectable(
			__in uint8_t type
			)
		{
			bool result = true;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

			switch(type) {
				case BLOCK_AIR:
				case BLOCK_CLOUD:
				case BLOCK_CORAL_BLUE:
				case BLOCK_CORAL_BROWN:
				case BLOCK_CORAL_ORANGE:
				case BLOCK_CORAL_PINK:
				case BLOCK_CORAL_PURPLE:
				case BLOCK_FLOWER_RED:
				case BLOCK_FLOWER_YELLOW:
				case BLOCK_GRASS_SHORT:
				case BLOCK_GRASS_TALL:
				case BLOCK_SEAGRASS_GREEN:
				case BLOCK_SEAGRASS_BROWN:
				case BLOCK_SHRUB:
				case BLOCK_WATER:
					result = false;
					break;
				default:
					break;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::generate_chunks_runtime(void)
		{
			glm::vec3 position;
			glm::ivec2 position_chunk;

			TRACE_ENTRY(LEVEL_VERBOSE);

			position = m_camera->position();
			position_chunk = glm::ivec2(position.x / CHUNK_WIDTH, position.z / CHUNK_WIDTH);

			for(int32_t z = -VIEW_RADIUS_RUNTIME; z < VIEW_RADIUS_RUNTIME; ++z) {

				for(int32_t x = -VIEW_RADIUS_RUNTIME; x < VIEW_RADIUS_RUNTIME; ++x) {
					glm::ivec2 position_chunk_offset = glm::ivec2(position_chunk.x + x, position_chunk.y + z);

					if(!m_manager_terrain.contains(position_chunk_offset)) {

						nomic::entity::chunk *entry = m_manager_terrain.at(position_chunk_offset);
						if(!entry) {
							THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE,
								"Position={%i, %i}", position_chunk_offset.x, position_chunk_offset.y);
						}

						entry->enable(false);
						entry->show(false);
						entry->register_renderer(m_chunk_renderer->type());
					}
				}
			}

			for(int32_t z = -VIEW_RADIUS_RUNTIME; z < VIEW_RADIUS_RUNTIME; ++z) {

				for(int32_t x = -VIEW_RADIUS_RUNTIME; x < VIEW_RADIUS_RUNTIME; ++x) {
					glm::ivec2 position_chunk_offset = glm::ivec2(position_chunk.x + x, position_chunk.y + z);
					nomic::entity::chunk *chunk_ref = m_manager_terrain.at(glm::ivec2(position_chunk_offset.x,
						position_chunk_offset.y), false);

					if(!chunk_ref || chunk_ref->enabled()) {
						continue;
					}

					uint32_t count = 0;
					nomic::entity::chunk *back = nullptr, *front = nullptr, *left = nullptr, *right = nullptr;

					if(m_manager_terrain.contains(glm::ivec2(position_chunk_offset.x + 1, position_chunk_offset.y))) { // right
						right = m_manager_terrain.at(glm::ivec2(position_chunk_offset.x + 1, position_chunk_offset.y), false);
						++count;
					}

					if(m_manager_terrain.contains(glm::ivec2(position_chunk_offset.x - 1, position_chunk_offset.y))) { // left
						left = m_manager_terrain.at(glm::ivec2(position_chunk_offset.x - 1, position_chunk_offset.y), false);
						++count;
					}

					if(m_manager_terrain.contains(glm::ivec2(position_chunk_offset.x, position_chunk_offset.y + 1))) { // back
						back = m_manager_terrain.at(glm::ivec2(position_chunk_offset.x, position_chunk_offset.y + 1), false);
						++count;
					}

					if(m_manager_terrain.contains(glm::ivec2(position_chunk_offset.x, position_chunk_offset.y - 1))) { // front
						front = m_manager_terrain.at(glm::ivec2(position_chunk_offset.x, position_chunk_offset.y - 1), false);
						++count;
					}

					if(count > CHUNK_ADJOIN_MIN) {
						chunk_ref->update(right, left, back, front);
						chunk_ref->enable(true);
					} else {
						chunk_ref->enable(false);
					}
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::generate_chunks_spawn(void)
		{
			nomic::entity::message *message = nullptr;
			std::vector<nomic::entity::plain *> backdrop;
			nomic::entity::diagnostic *diagnostic = nullptr;
			nomic::graphic::bitmap *backdrop_texture = nullptr;
			uint32_t completed = 0, current, previous = 0, total = (VIEW_WIDTH * VIEW_WIDTH);
			nomic::core::renderer *backdrop_renderer = nullptr, *diagnostic_renderer = nullptr, *message_renderer = nullptr;

			TRACE_ENTRY(LEVEL_VERBOSE);

			const renderer_config &config = DEBUG_RENDERER_CONFIGURATION.at(DEBUG_OBJECT_DIAGNOSTIC);

			diagnostic_renderer = new nomic::core::renderer(RENDERER_SPAWN_DIAGNOSTIC);
			if(!diagnostic_renderer) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE);
			}

			diagnostic_renderer->set_shaders(std::get<RENDERER_SHADER_VERTEX>(config), std::get<RENDERER_SHADER_FRAGMENT>(config));
			diagnostic_renderer->set_mode(std::get<RENDERER_MODE>(config));
			diagnostic_renderer->set_blend(std::get<RENDERER_BLEND_ENABLED>(config), std::get<RENDERER_BLEND_SFACTOR>(config),
				std::get<RENDERER_BLEND_DFACTOR>(config));
			diagnostic_renderer->set_cull(std::get<RENDERER_CULL_ENABLED>(config), std::get<RENDERER_CULL_MODE>(config));
			diagnostic_renderer->set_depth(std::get<RENDERER_DEPTH_ENABLED>(config), std::get<RENDERER_DEPTH_MODE>(config));

			diagnostic = new nomic::entity::diagnostic(false);
			if(!diagnostic) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE);
			}

			diagnostic->enable(false);
			diagnostic->show(false);
			diagnostic->register_renderer(diagnostic_renderer->type());

			message_renderer = new nomic::core::renderer(RENDERER_SPAWN_MESSAGE);
			if(!message_renderer) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE);
			}

			message_renderer->set_shaders(std::get<RENDERER_SHADER_VERTEX>(config), std::get<RENDERER_SHADER_FRAGMENT>(config));
			message_renderer->set_mode(std::get<RENDERER_MODE>(config));
			message_renderer->set_blend(std::get<RENDERER_BLEND_ENABLED>(config), std::get<RENDERER_BLEND_SFACTOR>(config),
				std::get<RENDERER_BLEND_DFACTOR>(config));
			message_renderer->set_cull(std::get<RENDERER_CULL_ENABLED>(config), std::get<RENDERER_CULL_MODE>(config));
			message_renderer->set_depth(std::get<RENDERER_DEPTH_ENABLED>(config), std::get<RENDERER_DEPTH_MODE>(config));

			message = new nomic::entity::message;
			if(!message) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE);
			}

			message->enable(false);
			message->show(false);
			message->register_renderer(message_renderer->type());

			backdrop_renderer = new nomic::core::renderer(RENDERER_SPAWN_BACKDROP);
			if(!backdrop_renderer) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE);
			}

			backdrop_renderer->set_shaders(std::get<RENDERER_SHADER_VERTEX>(BACKDROP_RENDERER_CONFIGURATION),
				std::get<RENDERER_SHADER_FRAGMENT>(BACKDROP_RENDERER_CONFIGURATION));
			backdrop_renderer->set_mode(std::get<RENDERER_MODE>(BACKDROP_RENDERER_CONFIGURATION));
			backdrop_renderer->set_blend(std::get<RENDERER_BLEND_ENABLED>(BACKDROP_RENDERER_CONFIGURATION),
				std::get<RENDERER_BLEND_SFACTOR>(BACKDROP_RENDERER_CONFIGURATION),
				std::get<RENDERER_BLEND_DFACTOR>(BACKDROP_RENDERER_CONFIGURATION));
			backdrop_renderer->set_cull(std::get<RENDERER_CULL_ENABLED>(BACKDROP_RENDERER_CONFIGURATION),
				std::get<RENDERER_CULL_MODE>(BACKDROP_RENDERER_CONFIGURATION));
			backdrop_renderer->set_depth(std::get<RENDERER_DEPTH_ENABLED>(BACKDROP_RENDERER_CONFIGURATION),
				std::get<RENDERER_DEPTH_MODE>(BACKDROP_RENDERER_CONFIGURATION));

			backdrop_texture = new nomic::graphic::bitmap(BACKDROP_PATH_DEFAULT);
			if(!backdrop_texture) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE);
			}

			const void *backdrop_texture_data_ref = backdrop_texture->pixels();
			if(backdrop_texture_data_ref) {
				uint8_t backdrop_texture_depth = (backdrop_texture->depth() / CHAR_WIDTH);
				glm::uvec2 backdrop_texture_dimensions = glm::uvec2(backdrop_texture->width(), backdrop_texture->height());
				std::vector<uint8_t> backdrop_texture_data((uint8_t *) backdrop_texture_data_ref,
					((uint8_t *) backdrop_texture_data_ref) + (backdrop_texture_dimensions.x
						* backdrop_texture_dimensions.y * backdrop_texture_depth));

				for(int32_t y = -BACKDROP_HEIGHT_DEFAULT; y < BACKDROP_HEIGHT_DEFAULT; ++y) {

					for(int32_t x = -BACKDROP_WIDTH_DEFAULT; x < BACKDROP_WIDTH_DEFAULT; ++x) {

						backdrop.push_back(new nomic::entity::plain(backdrop_texture_data, backdrop_texture_dimensions,
							backdrop_texture_depth));
						if(!backdrop.back()) {
							THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE,
								"Position={%i, %i}", x, y);
						}

						glm::vec3 position = m_camera->position();
						position.x += (x + BLOCK_RADIUS);
						position.y += (y + BLOCK_RADIUS);
						position.z += BACKDROP_OFFSET_DEFAULT;
						backdrop.back()->position() = position;
						backdrop.back()->show(true);
						backdrop.back()->register_renderer(backdrop_renderer->type());
					}
				}
			}

			diagnostic->enable(true);
			diagnostic->show(true);

			message->enable(true);
			message->show(true);
			message->text() = "Generating chunk objects...";

			update();
			render();

			for(int32_t z = -VIEW_RADIUS_SPAWN; z < VIEW_RADIUS_SPAWN; ++z) {

				for(int32_t x = -VIEW_RADIUS_SPAWN; x < VIEW_RADIUS_SPAWN; ++x, ++completed) {

					current = (100 * (completed / (float) total));
					if(current && (current != previous)) {
						std::stringstream stream;
						stream << "Generating chunk objects... " << current << "%";
						message->text() = stream.str();
						update();
						render();
						previous = current;
					}

					nomic::entity::chunk *entry = m_manager_terrain.at(glm::ivec2(x, z));
					if(!entry) {
						THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE,
							"Position={%i, %i}", x, z);
					}

					entry->enable(false);
					entry->show(false);
					entry->register_renderer(m_chunk_renderer->type());
				}
			}

			completed = 0;

			for(int32_t z = -VIEW_RADIUS_SPAWN; z < VIEW_RADIUS_SPAWN; ++z) {

				for(int32_t x = -VIEW_RADIUS_SPAWN; x < VIEW_RADIUS_SPAWN; ++x, ++completed) {

					current = (100 * (completed / (float) total));
					if(current && (current != previous)) {
						std::stringstream stream;
						stream << "Joining chunk objects... " << current << "%";
						message->text() = stream.str();
						update();
						render();
						previous = current;
					}

					nomic::entity::chunk *chunk_ref = m_manager_terrain.at(glm::ivec2(x, z), false);
					if(!chunk_ref || chunk_ref->enabled()) {
						continue;
					}

					uint32_t count = 0;
					nomic::entity::chunk *back = nullptr, *front = nullptr, *left = nullptr, *right = nullptr;

					if(m_manager_terrain.contains(glm::ivec2(x + 1, z))) { // right
						right = m_manager_terrain.at(glm::ivec2(x + 1, z), false);
						++count;
					}

					if(m_manager_terrain.contains(glm::ivec2(x - 1, z))) { // left
						left = m_manager_terrain.at(glm::ivec2(x - 1, z), false);
						++count;
					}

					if(m_manager_terrain.contains(glm::ivec2(x, z + 1))) { // back
						back = m_manager_terrain.at(glm::ivec2(x, z + 1), false);
						++count;
					}

					if(m_manager_terrain.contains(glm::ivec2(x, z - 1))) { // front
						front = m_manager_terrain.at(glm::ivec2(x, z - 1), false);
						++count;
					}

					if(count > CHUNK_ADJOIN_MIN) {
						chunk_ref->update(right, left, back, front);
						chunk_ref->enable(true);
					} else {
						chunk_ref->enable(false);
					}
				}
			}

			for(std::vector<nomic::entity::plain *>::iterator iter = backdrop.begin(); iter != backdrop.end(); ++iter) {

				if(*iter) {
					delete *iter;
					*iter = nullptr;
				}
			}

			backdrop.clear();

			if(backdrop_texture) {
				delete backdrop_texture;
				backdrop_texture = nullptr;
			}

			if(backdrop_renderer) {
				delete backdrop_renderer;
				backdrop_renderer = nullptr;
			}

			if(message) {
				delete message;
				message = nullptr;
			}

			if(message_renderer) {
				delete message_renderer;
				message_renderer = nullptr;
			}

			if(diagnostic) {
				delete diagnostic;
				diagnostic = nullptr;
			}

			if(diagnostic_renderer) {
				delete diagnostic_renderer;
				diagnostic_renderer = nullptr;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::generate_spawn_location(void)
		{
			uint8_t type;
			glm::uvec3 block;
			glm::ivec2 chunk;
			glm::vec3 position;

			TRACE_ENTRY(LEVEL_VERBOSE);

#ifdef SPAWN_RANDOM
			chunk.x = (m_random_integer.next() % SPAWN_RADIUS);
			chunk.y = (m_random_integer.next() % SPAWN_RADIUS);
			block.x = (std::abs(m_random_integer.next()) % (CHUNK_WIDTH - 1));
			block.z = (std::abs(m_random_integer.next()) % (CHUNK_WIDTH - 1));
#else
			nomic::utility::position_as_block(m_camera->position(), chunk, block);
#endif // SPAWN_RANDOM

			block.y = (CHUNK_HEIGHT - PLAYER_HEIGHT - 1);
			for(; block.y > BLOCK_HEIGHT_BOUNDARY; --block.y) {

				type = m_manager_terrain.at(chunk)->block_type(block);
				if((type != BLOCK_AIR) && (type != BLOCK_CLOUD)) {
					break;
				}
			}

			block.y += (PLAYER_HEIGHT + 1);
			if(block.y < (BLOCK_HEIGHT_BOUNDARY + PLAYER_HEIGHT)) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_SPAWN);
			}

			position = nomic::utility::block_as_position(chunk, block);
			m_camera->rotation() = CAMERA_ROTATION_DEFAULT;
			m_camera->position() = position;
			m_spawn = m_camera->position();

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Spawn location set. Position={%f, %f, %f} (Chunk={%i, %i}, Block={%u, %u, %u})",
				position.x, position.y, position.z, chunk.x, chunk.y, block.x, block.y, block.z);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::initialize_entities(void)
		{
			uint32_t type;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Building background entity objects...");

			for(uint32_t iter = 0; iter <= ENTITY_OBJECT_BACKGROUND_MAX; ++iter) {

				switch(iter) {
					case ENTITY_OBJECT_BACKGROUND_SKYBOX:
						type = RENDERER_BACKGROUND_SKYBOX;
						break;
					case ENTITY_OBJECT_BACKGROUND_SUN:
						type = RENDERER_BACKGROUND_SUN;
						break;
					default:
						break;
				}

				m_entity_renderer_background.push_back(new nomic::core::renderer(type));
				if(!m_entity_renderer_background.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Renderer=%u", iter);
				}

				const renderer_config &config = ENTITY_RENDERER_BACKGROUND_CONFIGURATION.at(iter);

				nomic::core::renderer *rend = m_entity_renderer_background.at(iter);
				if(rend) {
					rend->set_shaders(std::get<RENDERER_SHADER_VERTEX>(config), std::get<RENDERER_SHADER_FRAGMENT>(config));
					rend->set_mode(std::get<RENDERER_MODE>(config));
					rend->set_blend(std::get<RENDERER_BLEND_ENABLED>(config), std::get<RENDERER_BLEND_SFACTOR>(config),
						std::get<RENDERER_BLEND_DFACTOR>(config));
					rend->set_cull(std::get<RENDERER_CULL_ENABLED>(config), std::get<RENDERER_CULL_MODE>(config));
					rend->set_depth(std::get<RENDERER_DEPTH_ENABLED>(config), std::get<RENDERER_DEPTH_MODE>(config));
				}

				switch(iter) {
					case ENTITY_OBJECT_BACKGROUND_SKYBOX:

						m_entity_object_background.push_back(new nomic::entity::skybox);
						if(!m_entity_object_background.at(iter)) {
							THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE,
								"Object=%u", iter);
						}

						((nomic::entity::skybox *) m_entity_object_background.back())->set(ENTITY_SKYBOX_FACE);
						break;
					case ENTITY_OBJECT_BACKGROUND_SUN:
						m_entity_object_background.push_back(new nomic::entity::sun);
						break;
					default:
						break;
				}

				if(!m_entity_object_background.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Object=%u", iter);
				}

				m_entity_object_background.at(iter)->enable(false);
				m_entity_object_background.at(iter)->show(false);
				m_entity_object_background.at(iter)->register_renderer(m_entity_renderer_background.at(iter)->type());
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Building foreground entity objects...");

			for(uint32_t iter = 0; iter <= ENTITY_OBJECT_FOREGROUND_MAX; ++iter) {

				switch(iter) {
					case ENTITY_OBJECT_FOREGROUND_PANEL:
						type = RENDERER_FOREGROUND_PANEL;
						break;
					case ENTITY_OBJECT_FOREGROUND_RETICLE:
						type = RENDERER_FOREGROUND_RETICLE;
						break;
					case ENTITY_OBJECT_FOREGROUND_SELECTOR:
						type = RENDERER_FOREGROUND_SELECTOR;
						break;
					default:
						break;
				}

				m_entity_renderer_foreground.push_back(new nomic::core::renderer(type));
				if(!m_entity_renderer_foreground.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Renderer=%u", iter);
				}

				const renderer_config &config = ENTITY_RENDERER_FOREGROUND_CONFIGURATION.at(iter);

				nomic::core::renderer *rend = m_entity_renderer_foreground.at(iter);
				if(rend) {
					rend->set_shaders(std::get<RENDERER_SHADER_VERTEX>(config), std::get<RENDERER_SHADER_FRAGMENT>(config));
					rend->set_mode(std::get<RENDERER_MODE>(config));
					rend->set_blend(std::get<RENDERER_BLEND_ENABLED>(config), std::get<RENDERER_BLEND_SFACTOR>(config),
						std::get<RENDERER_BLEND_DFACTOR>(config));
					rend->set_cull(std::get<RENDERER_CULL_ENABLED>(config), std::get<RENDERER_CULL_MODE>(config));
					rend->set_depth(std::get<RENDERER_DEPTH_ENABLED>(config), std::get<RENDERER_DEPTH_MODE>(config));
				}

				switch(iter) {
					case ENTITY_OBJECT_FOREGROUND_PANEL:
						m_entity_object_foreground.push_back(new nomic::entity::panel(FOREGROUND_PANEL_SELECTION));
						break;
					case ENTITY_OBJECT_FOREGROUND_RETICLE:
						m_entity_object_foreground.push_back(new nomic::entity::reticle);
						break;
					case ENTITY_OBJECT_FOREGROUND_SELECTOR:
						m_entity_object_foreground.push_back(new nomic::entity::selector);
						break;
					default:
						break;
				}

				if(!m_entity_object_foreground.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Object=%u", iter);
				}

				m_entity_object_foreground.at(iter)->enable(false);
				m_entity_object_foreground.at(iter)->show(false);
				m_entity_object_foreground.at(iter)->register_renderer(m_entity_renderer_foreground.at(iter)->type());
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Building debug objects...");

			for(uint32_t iter = 0; iter <= DEBUG_OBJECT_MAX; ++iter) {

				switch(iter) {
					case DEBUG_OBJECT_AXIS:
						type = RENDERER_DEBUG_AXIS;
						break;
					case DEBUG_OBJECT_BLOCK:
						type = RENDERER_DEBUG_BLOCK;
						break;
					case DEBUG_OBJECT_DIAGNOSTIC:
						type = RENDERER_DEBUG_DIAGNOSTIC;
						break;
					default:
						break;
				}

				m_debug_renderer.push_back(new nomic::core::renderer(type));
				if(!m_debug_renderer.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Renderer=%u", iter);
				}

				const renderer_config &config = DEBUG_RENDERER_CONFIGURATION.at(iter);

				nomic::core::renderer *rend = m_debug_renderer.at(iter);
				if(rend) {
					rend->set_shaders(std::get<RENDERER_SHADER_VERTEX>(config), std::get<RENDERER_SHADER_FRAGMENT>(config));
					rend->set_mode(std::get<RENDERER_MODE>(config));
					rend->set_blend(std::get<RENDERER_BLEND_ENABLED>(config), std::get<RENDERER_BLEND_SFACTOR>(config),
						std::get<RENDERER_BLEND_DFACTOR>(config));
					rend->set_cull(std::get<RENDERER_CULL_ENABLED>(config), std::get<RENDERER_CULL_MODE>(config));
					rend->set_depth(std::get<RENDERER_DEPTH_ENABLED>(config), std::get<RENDERER_DEPTH_MODE>(config));
				}

				switch(iter) {
					case DEBUG_OBJECT_AXIS:
						m_debug_object.push_back(new nomic::entity::axis);
						break;
					case DEBUG_OBJECT_BLOCK:
						m_debug_object.push_back(new nomic::entity::block(DEBUG_BLOCK_FACE, DEBUG_BLOCK_SCALE));
						break;
					case DEBUG_OBJECT_DIAGNOSTIC:
						m_debug_object.push_back(new nomic::entity::diagnostic);
						break;
					default:
						break;
				}

				if(!m_debug_object.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Object=%u", iter);
				}

				m_debug_object.at(iter)->enable(false);
				m_debug_object.at(iter)->show(false);
				m_debug_object.at(iter)->register_renderer(m_debug_renderer.at(iter)->type());
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Building chunk objects...");

			m_chunk_renderer = new nomic::core::renderer(RENDERER_CHUNK);
			if(!m_chunk_renderer) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE);
			}

			m_chunk_renderer->set_shaders(std::get<RENDERER_SHADER_VERTEX>(CHUNK_RENDERER_CONFIGURATION),
				std::get<RENDERER_SHADER_FRAGMENT>(CHUNK_RENDERER_CONFIGURATION));
			m_chunk_renderer->set_mode(std::get<RENDERER_MODE>(CHUNK_RENDERER_CONFIGURATION));
			m_chunk_renderer->set_blend(std::get<RENDERER_BLEND_ENABLED>(CHUNK_RENDERER_CONFIGURATION),
				std::get<RENDERER_BLEND_SFACTOR>(CHUNK_RENDERER_CONFIGURATION),
				std::get<RENDERER_BLEND_DFACTOR>(CHUNK_RENDERER_CONFIGURATION));
			m_chunk_renderer->set_cull(std::get<RENDERER_CULL_ENABLED>(CHUNK_RENDERER_CONFIGURATION),
				std::get<RENDERER_CULL_MODE>(CHUNK_RENDERER_CONFIGURATION));
			m_chunk_renderer->set_depth(std::get<RENDERER_DEPTH_ENABLED>(CHUNK_RENDERER_CONFIGURATION),
				std::get<RENDERER_DEPTH_MODE>(CHUNK_RENDERER_CONFIGURATION));

			generate_chunks_spawn();

			for(uint32_t iter = 0; iter <= ENTITY_OBJECT_BACKGROUND_MAX; ++iter) {
				m_entity_object_background.at(iter)->enable(true);
				m_entity_object_background.at(iter)->show(true);
			}

			for(uint32_t iter = 0; iter <= ENTITY_OBJECT_FOREGROUND_MAX; ++iter) {
				m_entity_object_foreground.at(iter)->enable(true);
				m_entity_object_foreground.at(iter)->show(true);
			}

			for(uint32_t iter = 0; iter <= DEBUG_OBJECT_MAX; ++iter) {
				m_debug_object.at(iter)->enable(m_debug);
				m_debug_object.at(iter)->show(m_debug);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::on_button(
			__in uint8_t button,
			__in uint8_t state,
			__in uint8_t clicks,
			__in int32_t x,
			__in int32_t y
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Button=%x, State=%x(%s), Clicks=%u, Position={%i, %i}", button, state,
				(state == SDL_PRESSED) ? "Press" : "Release", (uint16_t) clicks, x, y);

			switch(button) {
				case SDL_BUTTON_LEFT:

					if(block_selected() && (state == SDL_RELEASED)) {
						selected_block_add();
					}
					break;
				case SDL_BUTTON_RIGHT:

					if(block_selected() && (state == SDL_RELEASED)) {
						selected_block_remove();
					}
					break;
				default:
					break;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::on_key(
			__in uint16_t scancode,
			__in uint16_t modifier,
			__in uint8_t state
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Scancode=%x, Modifier=%x, State=%x(%s)", scancode, modifier, state,
				(state == SDL_PRESSED) ? "Press" : "Release");

			if(m_camera) {
				m_camera->key(scancode, modifier, state);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::on_initialize(void)
		{
			GLenum err;
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager initializing...");

			for(std::map<SDL_GLattr, GLint>::const_iterator iter = SDL_ATTRIBUTE.begin(); iter != SDL_ATTRIBUTE.end();
					++iter) {

				if(SDL_GL_SetAttribute(iter->first, iter->second)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
						"SDL_GL_SetAttribute(%x, %i) failed! Error=%s", iter->first, iter->second, SDL_GetError());
				}
			}

			m_manager_display.initialize();
			glewExperimental = GL_TRUE;

			err = glewInit();
			if(err != GLEW_OK) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"glewInit failed! Error=%x(%s)", err, GL_ERROR_AS_STRING(err));
			}

			GL_FLUSH_ERROR();

			if(SDL_GL_SetSwapInterval(DISPLAY_DEFAULT_VSYNC)) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"SDL_GL_SetSwapInterval(%x) failed! Error=%s", DISPLAY_DEFAULT_VSYNC, SDL_GetError());
			}

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Display vsync=%s", (DISPLAY_DEFAULT_VSYNC ? "Enabled" : "Disabled"));

			if(SDL_SetRelativeMouseMode(DISPLAY_MOUSE_RELATIVE)) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"SDL_SetRelativeMouseMode(%x) failed! Error=%s", DISPLAY_MOUSE_RELATIVE, SDL_GetError());
			}

			for(std::map<GLenum, std::map<GLenum, GLenum>>::const_iterator iter_attribute = GRAPHIC_ATTRIBUTE.begin();
					iter_attribute != GRAPHIC_ATTRIBUTE.end(); ++iter_attribute) {
				GL_CHECK(LEVEL_WARNING, glEnable, iter_attribute->first);

				for(std::map<GLenum, GLenum>::const_iterator iter_hint = iter_attribute->second.begin();
						iter_hint != iter_attribute->second.end(); ++iter_hint) {
					GL_CHECK(LEVEL_WARNING, glHint, iter_hint->first, iter_hint->second);
				}
			}

			m_manager_graphic.initialize();
			m_manager_render.initialize();
			m_manager_font.initialize();
			m_manager_entity.initialize();
			m_manager_terrain.initialize();

			m_atlas = new nomic::graphic::atlas;
			if(!m_atlas) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Failed to allocate atlas, Address=%p", m_atlas);
			}

			m_atlas->load(ATLAS_PATH_DEFAULT);

			m_camera = new nomic::entity::camera(m_manager_display.dimensions());
			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Failed to allocate camera, Address=%p", m_camera);
			}

			m_camera->enable(false);
			m_camera->show(false);
			m_clouds = false;
			m_manager_display.set_icon(DISPLAY_DEFAULT_ICON);
			m_random_float.setup(m_manager_terrain.generator().seed());
			m_random_integer.setup(m_manager_terrain.generator().seed());
			m_underwater = false;

			initialize_entities();
			generate_spawn_location();

			nomic::event::input::sync();
			nomic::core::thread::start(true);
			nomic::core::thread::notify();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager initialized");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_motion(
			__in uint32_t state,
			__in int32_t x,
			__in int32_t y,
			__in int32_t x_relative,
			__in int32_t y_relative
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "State=%x, Position={%i, %i}, Relative={%i, %i}", state, x, y,
				x_relative, y_relative);

			if(m_camera) {
				m_camera->motion(state, x, y, x_relative, y_relative);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::on_run(void)
		{
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			generate_chunks_runtime();

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager uninitializing...");

			nomic::core::thread::stop();

			uninitialize_entities();

			if(m_camera) {
				delete m_camera;
				m_camera = nullptr;
			}

			if(m_atlas) {
				delete m_atlas;
				m_atlas = nullptr;
			}

			m_manager_terrain.uninitialize();
			m_manager_entity.uninitialize();
			m_manager_font.uninitialize();
			m_manager_render.uninitialize();
			m_manager_graphic.uninitialize();
			m_manager_display.uninitialize();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager uninitialized");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::on_wheel(
			__in uint32_t direction,
			__in int32_t x,
			__in int32_t y
			)
		{
			bool right;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Direction=%x(%s), Position={%i, %i}", direction,
				(direction == SDL_MOUSEWHEEL_NORMAL) ? "Normal" : "Flipped", x, y);

			if(direction == SDL_MOUSEWHEEL_FLIPPED) {
				y *= -1;
			}

#ifndef CAMERA_FOV_CONFIGURABLE
			right = (y > 0);
			y = std::abs(y);

			for(uint32_t iter = 0; iter < y; ++iter) {

				if(right) {
					panel_move_left();
				} else {
					panel_move_right();
				}
			}
#else

			if(m_camera) {
				m_camera->wheel(direction, x, y);
			}
#endif // CAMERA_FOV_CONFIGURABLE

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::panel_move_left(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			((nomic::entity::panel *) m_entity_object_foreground.at(ENTITY_OBJECT_FOREGROUND_PANEL))->move_left();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::panel_move_right(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			((nomic::entity::panel *) m_entity_object_foreground.at(ENTITY_OBJECT_FOREGROUND_PANEL))->move_right();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::pause(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager pausing...");
			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager paused");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::render(
			__in_opt float delta
			)
		{
			glm::vec3 position = glm::vec3(0.f);
			nomic::entity::sun *sun_ref = nullptr;
			glm::vec4 color = glm::vec4(1.f), color_background = glm::vec4(1.f);

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f", delta);

			if(!m_atlas) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Atlas is not allocated, Address=%p", m_atlas);
			}

			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Camera is not allocated, Address=%p", m_camera);
			}

			sun_ref = (nomic::entity::sun *) m_entity_object_background.at(ENTITY_OBJECT_BACKGROUND_SUN);
			if(sun_ref) {
				color = sun_ref->color();
				color_background = sun_ref->color_background();
				position = sun_ref->position();
			}

			m_manager_display.clear();
			m_manager_render.render(m_camera->position(), m_camera->rotation(), m_camera->projection(), m_camera->view(),
				m_camera->dimensions(), *m_atlas, ((nomic::runtime *) m_runtime)->tick_cycle(), delta, color, color_background,
				position, m_clouds, m_underwater);
			m_manager_display.show();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		uint32_t 
		manager::seed(void)
		{
			uint32_t result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			result = m_manager_terrain.generator().seed();

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u(%x)", result, result);
			return result;
		}

		void 
		manager::selected_block(
			__inout glm::ivec2 &chunk,
			__inout glm::uvec3 &block,
			__inout uint8_t &face
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!m_block_selected) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_BLOCK_NOT_SELECTED);
			}

			block = m_block_selected_block;
			chunk = m_block_selected_chunk;
			face = m_block_selected_face;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Chunk={%i, %i}, Block={%u, %u, %u}, Face=%x(%s)", chunk.x, chunk.y,
				block.x, block.y, block.z, face, BLOCK_FACE_STRING(face));
		}

		bool 
		manager::selected_block_add(void)
		{
			bool result = false;
			uint8_t attribute = BLOCK_ATTRIBUTES_DEFAULT;

			TRACE_ENTRY(LEVEL_VERBOSE);

			if(m_block_selected) {
				bool place = false;
				glm::uvec3 block = m_block_selected_block;
				glm::ivec2 chunk = m_block_selected_chunk;

				switch(m_block_selected_face) {
					case BLOCK_FACE_RIGHT:

						place = (block.x < (CHUNK_WIDTH - 1));
						if(place) { // right
							attribute |= BLOCK_ATTRIBUTE_ROTATED_RIGHT;
							++block.x;
						} else { // right (boundary)

							place = m_manager_terrain.contains(glm::ivec2(chunk.x + 1, chunk.y));
							if(place) {
								attribute |= BLOCK_ATTRIBUTE_ROTATED_RIGHT;
								block.x = 0;
								++chunk.x;
							}
						}
						break;
					case BLOCK_FACE_LEFT:

						place = (block.x > 0);
						if(place) { // left
							attribute |= BLOCK_ATTRIBUTE_ROTATED_LEFT;
							--block.x;
						} else { // left (boundary)

							place = m_manager_terrain.contains(glm::ivec2(chunk.x - 1, chunk.y));
							if(place) {
								attribute |= BLOCK_ATTRIBUTE_ROTATED_LEFT;
								block.x = (CHUNK_WIDTH - 1);
								--chunk.x;
							}
						}
						break;
					case BLOCK_FACE_TOP:

						place = (block.y < (CHUNK_HEIGHT - 1));
						if(place) {
							++block.y;
						}
						break;
					case BLOCK_FACE_BOTTOM:

						place = (block.y > 0);
						if(place) {
							--block.y;
						}
						break;
					case BLOCK_FACE_BACK:

						place = (block.z < (CHUNK_WIDTH - 1));
						if(place) { // back
							attribute |= BLOCK_ATTRIBUTE_ROTATED_BACK;
							++block.z;
						} else { // back (boundary)

							place = m_manager_terrain.contains(glm::ivec2(chunk.x, chunk.y + 1));
							if(place) {
								attribute |= BLOCK_ATTRIBUTE_ROTATED_BACK;
								block.z = 0;
								++chunk.y;
							}
						}
						break;
					case BLOCK_FACE_FRONT:

						place = (block.z > 0);
						if(place) { // front
							attribute |= BLOCK_ATTRIBUTE_ROTATED_FRONT;
							--block.z;
						} else { // front (boundary)

							place = m_manager_terrain.contains(glm::ivec2(chunk.x, chunk.y - 1));
							if(place) {
								attribute |= BLOCK_ATTRIBUTE_ROTATED_FRONT;
								block.z = (CHUNK_WIDTH - 1);
								--chunk.y;
							}
						}
						break;
					default:
						break;
				}

				result = (place && m_camera);
				if(result) {

					for(uint32_t iter = 0; iter < CAMERA_HEIGHT_OFFSET; ++iter) {

						result = (block != (m_camera->block() - glm::uvec3(0, iter, 0)));
						if(!result) {
							break;
						}
					}

					if(result) {
						uint8_t type = ((nomic::entity::panel *) m_entity_object_foreground.at(
							ENTITY_OBJECT_FOREGROUND_PANEL))->selected();

						TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Adding block. Type=%x Chunk={%i, %i}, Block={%u, %u, %u}",
							type, chunk.x, chunk.y, block.x, block.y, block.z);

						m_manager_terrain.at(chunk)->set_block(block, type, attribute);
						m_block_selected = false;
						m_block_selected_face = BLOCK_FACE_UNDEFINED;
					}
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::selected_block_remove(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::entity::chunk *chunk = m_manager_terrain.at(m_block_selected_chunk);
			if(chunk && (chunk->block_attributes(m_block_selected_block) & BLOCK_ATTRIBUTE_BREAKABLE)) {
				glm::uvec3 position_above;
				uint8_t attributes = (BLOCK_ATTRIBUTE_STATIC & ~BLOCK_ATTRIBUTE_BREAKABLE), type = BLOCK_AIR;

				TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Removing block. Chunk={%i, %i}, Block={%u, %u, %u}",
					m_block_selected_chunk.x, m_block_selected_chunk.y, m_block_selected_block.x,
					m_block_selected_block.y, m_block_selected_block.z);

				if(((m_block_selected_block.y + 1) < CHUNK_HEIGHT)
						&& (chunk->block_type(m_block_selected_block + glm::uvec3(0, 1, 0)) == BLOCK_WATER)) { // top
					attributes |= BLOCK_ATTRIBUTE_HIDDEN;
					type = BLOCK_WATER;
				} else {

					if(((m_block_selected_block.x + 1) < CHUNK_WIDTH)
						&& (chunk->block_type(m_block_selected_block + glm::uvec3(1, 0, 0)) == BLOCK_WATER)) { // right
						type = BLOCK_WATER;
					} else if((m_block_selected_block.x == CHUNK_WIDTH)
							&& m_manager_terrain.contains(m_block_selected_chunk + glm::ivec2(1, 0))
							&& (m_manager_terrain.at(m_block_selected_chunk + glm::ivec2(1, 0))->block_type(
								glm::uvec3(0, m_block_selected_block.y, m_block_selected_block.z))
							== BLOCK_WATER)) { // right (boundary)
						type = BLOCK_WATER;
					} else if(m_block_selected_block.x
							&& (chunk->block_type(m_block_selected_block - glm::uvec3(1, 0, 0)) == BLOCK_WATER)) { // left
						type = BLOCK_WATER;
					} else if(!m_block_selected_block.x
							&& m_manager_terrain.contains(m_block_selected_chunk - glm::ivec2(1, 0))
							&& (m_manager_terrain.at(m_block_selected_chunk - glm::ivec2(1, 0))->block_type(
								glm::uvec3(CHUNK_WIDTH - 1, m_block_selected_block.y, m_block_selected_block.z))
							== BLOCK_WATER)) { // left (boundary)
						type = BLOCK_WATER;
					} else if(((m_block_selected_block.z + 1) < CHUNK_WIDTH)
							&& (chunk->block_type(m_block_selected_block + glm::uvec3(0, 0, 1)) == BLOCK_WATER)) { // back
						type = BLOCK_WATER;
					} else if((m_block_selected_block.z == CHUNK_WIDTH)
							&& m_manager_terrain.contains(m_block_selected_chunk + glm::ivec2(0, 1))
							&& (m_manager_terrain.at(m_block_selected_chunk + glm::ivec2(0, 1))->block_type(
								glm::uvec3(m_block_selected_block.x, m_block_selected_block.y, 0))
							== BLOCK_WATER)) { // back (boundary)
						type = BLOCK_WATER;
					} else if(m_block_selected_block.z
							&& (chunk->block_type(m_block_selected_block - glm::uvec3(0, 0, 1)) == BLOCK_WATER)) { // front
						type = BLOCK_WATER;
					} else if(!m_block_selected_block.z
							&& m_manager_terrain.contains(m_block_selected_chunk - glm::ivec2(0, 1))
							&& (m_manager_terrain.at(m_block_selected_chunk - glm::ivec2(0, 1))->block_type(
								glm::uvec3(m_block_selected_block.x, m_block_selected_block.y, CHUNK_WIDTH - 1))
							== BLOCK_WATER)) { // front (boundary)
						type = BLOCK_WATER;
					}

					if((type == BLOCK_WATER)
							&& ((m_block_selected_block.y + 1) < CHUNK_HEIGHT)
							&& (chunk->block_type(m_block_selected_block + glm::uvec3(0, 1, 0)) != BLOCK_AIR)) { // top
						attributes |= BLOCK_ATTRIBUTE_HIDDEN;
					}
				}

				position_above = glm::uvec3(m_block_selected_block.x, m_block_selected_block.y + 1, m_block_selected_block.z);
				if(determine_block_decoration(chunk->block_type(position_above))) { // remove decoration
					chunk->set_block(position_above, BLOCK_AIR, attributes);
				}

				chunk->set_block(m_block_selected_block, type, attributes);
				m_block_selected = false;
				m_block_selected_face = BLOCK_FACE_UNDEFINED;

				if((m_block_selected_block.x == (CHUNK_WIDTH - 1)) && m_manager_terrain.contains(glm::ivec2(
							m_block_selected_chunk.x + 1, m_block_selected_chunk.y))) { // right
					m_manager_terrain.at(glm::ivec2(m_block_selected_chunk.x + 1, m_block_selected_chunk.y))->update();
				} else if((m_block_selected_block.x == 0) && m_manager_terrain.contains(glm::ivec2(m_block_selected_chunk.x - 1,
							m_block_selected_chunk.y))) { // left
					m_manager_terrain.at(glm::ivec2(m_block_selected_chunk.x - 1, m_block_selected_chunk.y))->update();
				}

				if((m_block_selected_block.z == (CHUNK_WIDTH - 1)) && m_manager_terrain.contains(glm::ivec2(
							m_block_selected_chunk.x, m_block_selected_chunk.y + 1))) { // back
					m_manager_terrain.at(glm::ivec2(m_block_selected_chunk.x, m_block_selected_chunk.y + 1))->update();
				} else if((m_block_selected_block.z == 0) && m_manager_terrain.contains(glm::ivec2(m_block_selected_chunk.x,
							m_block_selected_chunk.y - 1))) { // front
					m_manager_terrain.at(glm::ivec2(m_block_selected_chunk.x, m_block_selected_chunk.y - 1))->update();
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_debug(
			__in bool debug
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Debug=%x", debug);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			m_debug = debug;
			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Debug mode %s", (m_debug ? "active" : "inactive"));

#ifdef RETICLE_DEBUG_CONTROL
			if(m_entity_object_foreground.size() >= ENTITY_OBJECT_FOREGROUND_RETICLE) {
				nomic::entity::reticle *reticle_ref
					= (nomic::entity::reticle *) m_entity_object_foreground.at(ENTITY_OBJECT_FOREGROUND_RETICLE);

				if(reticle_ref) {
					reticle_ref->enable(m_debug);
					reticle_ref->show(m_debug);
				}
			}
#endif // RETICLE_DEBUG_CONTROL

#ifdef SELECTOR_BOUNDS_DEBUG_CONTROL
			if(m_entity_object_foreground.size() >= ENTITY_OBJECT_FOREGROUND_SELECTOR) {
				nomic::entity::selector *selector_ref
					= (nomic::entity::selector *) m_entity_object_foreground.at(ENTITY_OBJECT_FOREGROUND_SELECTOR);

				if(selector_ref) {
					selector_ref->show_bounds(m_debug);
				}
			}
#endif // SELECTOR_BOUNDS_DEBUG_CONTROL

			for(std::vector<nomic::entity::object *>::iterator iter = m_debug_object.begin(); iter != m_debug_object.end(); ++iter) {

				if(*iter) {
					(*iter)->enable(m_debug);
					(*iter)->show(m_debug);
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_dimensions(
			__in const glm::uvec2 &dimensions
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimension={%u, %u}", dimensions.x, dimensions.y);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Camera is not allocated, Address=%p", m_camera);
			}

			m_manager_display.set_dimensions(dimensions);
			m_camera->set_dimensions(dimensions);
			m_manager_entity.set_view_dimensions(dimensions);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_fullscreen(
			__in bool fullscreen
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Fullscreen=%x", fullscreen);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			m_manager_display.set_fullscreen(fullscreen);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_icon(
			__in const std::string &path
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s", path.size(), STRING_CHECK(path));

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			m_manager_display.set_icon(path);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_runtime(
			__in void *runtime
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p", runtime);

			m_runtime = runtime;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_seed(
			__in uint32_t seed,
			__in_opt uint32_t octaves,
			__in_opt double amplitude,
			__in_opt uint32_t max
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Seed=%u(%x), Octaves=%u, Amplitude=%g, Max=%u", seed, octaves, amplitude, max);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			m_manager_terrain.generator().setup(seed, octaves, amplitude, max);
			m_random_float.setup(seed);
			m_random_integer.setup(seed);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_spawn(
			__in const glm::vec3 &position
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Position={%f, %f, %f}", position.x, position.y, position.z);

			m_spawn = position;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_title(
			__in const std::string &title
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Title[%u]=%s", title.size(), STRING_CHECK(title));

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			m_manager_display.set_title(title);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_vsync(
			__in bool vsync
			)
		{
			int flag;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Vsync=%x", vsync);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			flag = (vsync ? SDL_TRUE : SDL_FALSE);

			if(SDL_GL_SetSwapInterval(flag)) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"SDL_GL_SetSwapInterval(%x) failed! Error=%s", DISPLAY_DEFAULT_VSYNC, SDL_GetError());
			}

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Display vsync=%s", (vsync ? "Enabled" : "Disabled"));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		glm::vec3 
		manager::spawn(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result={%f, %f, %f}", m_spawn.x, m_spawn.y, m_spawn.z);
			return m_spawn;
		}

		nomic::terrain::manager &
		manager::terrain(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return m_manager_terrain;
		}

		std::string 
		manager::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_SESSION_MANAGER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << SINGLETON_CLASS(nomic::session::manager)::to_string(verbose)
					<< ", Mode=" << (m_debug ? "Non-debug" : "Debug") << ", " << (m_clouds ? "Clouds" : "Below-clouds")
					<< ", " << (m_underwater ? "Underwater" : "Above-water")
					<< ", Spawn={" << m_spawn.x << ", " << m_spawn.y << ", " << m_spawn.z << "}"
					<< ", " << (m_block_selected ? "Selected" : "Not Selected");

				if(m_block_selected) {
					result << "(Chunk={" << m_block_selected_chunk.x << ", " << m_block_selected_chunk.y << "}"
						<< ", Block={" << m_block_selected_block.x << ", " << m_block_selected_block.y
							<< ", " << m_block_selected_block.z << "}"
						<< ", Face=" << SCALAR_AS_HEX(uint8_t, m_block_selected_face)
							<< "(" << BLOCK_FACE_STRING(m_block_selected_face) << "))";
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		bool 
		manager::toggle_debug(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			set_debug(!m_debug);

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_debug);
			return m_debug;
		}

		void 
		manager::uninitialize_entities(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Destroying chunk objects...");

			if(m_chunk_renderer) {
				delete m_chunk_renderer;
				m_chunk_renderer = nullptr;
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Destroying debug objects...");

			for(uint32_t iter = 0; iter < m_debug_object.size(); ++iter) {

				if(m_debug_object.at(iter)) {
					delete m_debug_object.at(iter);
					m_debug_object.at(iter) = nullptr;
				}
			}

			for(uint32_t iter = 0; iter < m_debug_renderer.size(); ++iter) {

				if(m_debug_renderer.at(iter)) {
					delete m_debug_renderer.at(iter);
					m_debug_renderer.at(iter) = nullptr;
				}
			}

			m_debug_object.clear();
			m_debug_renderer.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Destroying foreground entity objects...");

			for(uint32_t iter = 0; iter < m_entity_object_foreground.size(); ++iter) {

				if(m_entity_object_foreground.at(iter)) {
					delete m_entity_object_foreground.at(iter);
					m_entity_object_foreground.at(iter) = nullptr;
				}
			}

			for(uint32_t iter = 0; iter < m_entity_renderer_foreground.size(); ++iter) {

				if(m_entity_renderer_foreground.at(iter)) {
					delete m_entity_renderer_foreground.at(iter);
					m_entity_renderer_foreground.at(iter) = nullptr;
				}
			}

			m_entity_object_foreground.clear();
			m_entity_renderer_foreground.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Destroying background entity objects...");

			for(uint32_t iter = 0; iter < m_entity_object_background.size(); ++iter) {

				if(m_entity_object_background.at(iter)) {
					delete m_entity_object_background.at(iter);
					m_entity_object_background.at(iter) = nullptr;
				}
			}

			for(uint32_t iter = 0; iter < m_entity_renderer_background.size(); ++iter) {

				if(m_entity_renderer_background.at(iter)) {
					delete m_entity_renderer_background.at(iter);
					m_entity_renderer_background.at(iter) = nullptr;
				}
			}

			m_entity_object_background.clear();
			m_entity_renderer_background.clear();

			m_block_selected = false;
			m_block_selected_block = glm::uvec3();
			m_block_selected_chunk = glm::ivec2();
			m_block_selected_face = BLOCK_FACE_UNDEFINED;
			m_spawn = glm::vec3();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::underwater(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_underwater);
			return m_underwater;
		}

		void 
		manager::unpause(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager unpausing...");
			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager unpaused");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::update(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::event::input::poll_input();
			m_camera->update();
			
			if(m_camera->chunk_changed()) {
				nomic::core::thread::notify();
			}

			update_clouds();
			update_underwater();
			update_selector();
			m_manager_entity.update(m_runtime, m_camera);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::update_clouds(void)
		{
			uint8_t type;
			glm::uvec3 block;
			glm::ivec2 chunk;

			TRACE_ENTRY(LEVEL_VERBOSE);

			block = m_camera->block();
			chunk = m_camera->chunk();

			type = m_manager_terrain.at(chunk)->block_type(block);
			if(!determine_block_decoration(type)) {
				m_clouds = ((block.y <= (CHUNK_HEIGHT - 1)) && (type == BLOCK_CLOUD));
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::update_selector(void)
		{
			uint8_t type;
			float distance = 0.f;
			glm::vec3 position, rotation;
			bool back = false, bottom = false, front = false, left = false, right = false, top = false;

			TRACE_ENTRY(LEVEL_VERBOSE);

			m_block_selected = false;
			position = m_camera->position();
			rotation = glm::normalize(m_camera->rotation()) * SELECTOR_DISTANCE_SCALE;

			for(; distance < SELECTOR_DISTANCE_MAX; ++distance) {
				position += rotation;
				nomic::utility::position_as_block(position, m_block_selected_chunk, m_block_selected_block);

				type = m_manager_terrain.at(m_block_selected_chunk)->block_type(m_block_selected_block);
				if(determine_block_selectable(type)) {
					int32_t x = m_block_selected_block.x, y = m_block_selected_block.y, z = m_block_selected_block.z;

					if((x + 1) < CHUNK_WIDTH) { // right

						type = m_manager_terrain.at(m_block_selected_chunk)->block_type(glm::uvec3(x + 1, y, z));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							right = true;
						}
					} else if(((x + 1) == CHUNK_WIDTH) && m_manager_terrain.contains(glm::ivec2(m_block_selected_chunk.x + 1,
							m_block_selected_chunk.y))) { // right (boundary)

						type = m_manager_terrain.at(glm::ivec2(
							m_block_selected_chunk.x + 1, m_block_selected_chunk.y))->block_type(glm::uvec3(0, y, z));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							right = true;
						}
					}

					if((x - 1) >= 0) { // left

						type = m_manager_terrain.at(m_block_selected_chunk)->block_type(glm::uvec3(x - 1, y, z));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							left = true;
						}
					} else if(((x - 1) < 0) && m_manager_terrain.contains(glm::ivec2(m_block_selected_chunk.x - 1,
							m_block_selected_chunk.y))) { // left (boundary)

						type = m_manager_terrain.at(glm::ivec2(
							m_block_selected_chunk.x - 1, m_block_selected_chunk.y))->block_type(glm::uvec3(
							CHUNK_WIDTH - 1, y, z));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							left = true;
						}
					}

					if((y - 1) > 0) { // bottom

						type = m_manager_terrain.at(m_block_selected_chunk)->block_type(glm::uvec3(x, y - 1, z));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							bottom = true;
						}
					}

					if((y + 1) < CHUNK_HEIGHT) { // top

						type = m_manager_terrain.at(m_block_selected_chunk)->block_type(glm::uvec3(x, y + 1, z));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							top = true;
						}
					}

					if((z + 1) < CHUNK_WIDTH) { // back

						type = m_manager_terrain.at(m_block_selected_chunk)->block_type(glm::uvec3(x, y, z + 1));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							back = true;
						}
					} else if(((z + 1) == CHUNK_WIDTH) && m_manager_terrain.contains(glm::ivec2(m_block_selected_chunk.x,
							m_block_selected_chunk.y + 1))) { // back (boundary)

						type = m_manager_terrain.at(glm::ivec2(
							m_block_selected_chunk.x, m_block_selected_chunk.y + 1))->block_type(glm::uvec3(x, y, 0));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							back = true;
						}
					}

					if((z - 1) >= 0) { // front

						type = m_manager_terrain.at(m_block_selected_chunk)->block_type(glm::uvec3(x, y, z - 1));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							front = true;
						}
					} else if(((z - 1) < 0) && m_manager_terrain.contains(glm::ivec2(m_block_selected_chunk.x,
							m_block_selected_chunk.y - 1))) { // front (boundary)

						type = m_manager_terrain.at(glm::ivec2(
							m_block_selected_chunk.x, m_block_selected_chunk.y - 1))->block_type(glm::uvec3(
							x, y, CHUNK_WIDTH - 1));
						if(!determine_block_selectable(type)) {
							m_block_selected = true;
							front = true;
						}
					}

					break;
				}
			}

			nomic::entity::selector *selector_ref
				= (nomic::entity::selector *) m_entity_object_foreground.at(ENTITY_OBJECT_FOREGROUND_SELECTOR);
			if(selector_ref) {

				if(m_block_selected) {
					uint8_t face = BLOCK_FACE_UNDEFINED;
					glm::vec3 position_block = nomic::utility::block_as_position(m_block_selected_chunk, m_block_selected_block),
						position_block_offset = position;

					selector_ref->position() = position_block;
					position_block_offset.x = glm::fract(position_block_offset.x);
					position_block_offset.y = glm::fract(position_block_offset.y);
					position_block_offset.z = glm::fract(position_block_offset.z);

					if(right && (position_block_offset.x >= (1.f - SELECTOR_DISTANCE_STEP))) {
						face = BLOCK_FACE_RIGHT;
					}

					if(left && (position_block_offset.x <= SELECTOR_DISTANCE_STEP)) {
						face = BLOCK_FACE_LEFT;
					}

					if(top && (position_block_offset.y >= (1.f - SELECTOR_DISTANCE_STEP))) {
						face = BLOCK_FACE_TOP;
					}

					if(bottom && (position_block_offset.y <= SELECTOR_DISTANCE_STEP)) {
						face = BLOCK_FACE_BOTTOM;
					}

					if(back && (position_block_offset.z >= (1.f - SELECTOR_DISTANCE_STEP))) {
						face = BLOCK_FACE_BACK;
					}

					if(front && (position_block_offset.z <= SELECTOR_DISTANCE_STEP)) {
						face = BLOCK_FACE_FRONT;
					}

					if(m_block_selected_face != face) {
						m_block_selected_face = face;
						selector_ref->set_face(m_block_selected_face);
					}
				}

				selector_ref->show(m_block_selected);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::update_underwater(void)
		{
			uint8_t type;
			glm::uvec3 block;
			glm::ivec2 chunk;

			TRACE_ENTRY(LEVEL_VERBOSE);

			block = m_camera->block();
			chunk = m_camera->chunk();

			type = m_manager_terrain.at(chunk)->block_type(block);
			if(!determine_block_decoration(type)) {
				m_underwater = ((block.y < (CHUNK_HEIGHT - 1)) && (type == BLOCK_WATER));
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
