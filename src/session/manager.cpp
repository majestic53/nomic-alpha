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
#include "../../include/entity/diagnostic.h"
#include "../../include/entity/message.h"
#include "../../include/entity/reticle.h"
#include "../../include/entity/skybox.h"
#include "../../include/graphic/vao.h"
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

		static const renderer_config CHUNK_RENDERER_CONFIGURATION = {
			"./res/vert_block_color.glsl", "./res/frag_block_color.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
			RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_MODE_DEFAULT, GL_FRONT,
			RENDERER_DEPTH_DEFAULT, RENDERER_DEPTH_MODE_DEFAULT
			};

		enum {
			DEBUG_OBJECT_AXIS = 0,
			DEBUG_OBJECT_DIAGNOSTIC,
			DEBUG_OBJECT_RETICLE,
		};

		#define DEBUG_OBJECT_MAX DEBUG_OBJECT_RETICLE

		static const std::vector<renderer_config> DEBUG_RENDERER_CONFIGURATION = {
			{ "./res/vert_axis.glsl", "./res/frag_axis.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT, RENDERER_BLEND_DFACTOR_DEFAULT,
				RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, RENDERER_CULL_MODE_DEFAULT, RENDERER_DEPTH_DEFAULT,
				RENDERER_DEPTH_MODE_DEFAULT }, // axis
			{ "./res/vert_string_static.glsl", "./res/frag_string_static.glsl", RENDER_ORTHOGONAL, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_MODE_DEFAULT, RENDERER_CULL_MODE_DEFAULT,
				RENDERER_DEPTH_DEFAULT, RENDERER_DEPTH_MODE_DEFAULT }, // diagnostic
			{ "./res/vert_reticle.glsl", "./res/frag_reticle.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT, RENDERER_CULL_MODE_DEFAULT,
				RENDERER_DEPTH_DEFAULT, RENDERER_DEPTH_MODE_DEFAULT }, // reticle
			};

		enum {
			ENTITY_OBJECT_SKYBOX = 0,
		};

		#define ENTITY_OBJECT_MAX ENTITY_OBJECT_SKYBOX

		static const std::vector<renderer_config> ENTITY_RENDERER_CONFIGURATION = {
			{ "./res/vert_skybox.glsl", "./res/frag_skybox.glsl", RENDER_PERSPECTIVE, RENDERER_BLEND_DEFAULT,
				RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT, RENDERER_CULL_DEFAULT,
				RENDERER_CULL_MODE_DEFAULT, false, RENDERER_DEPTH_MODE_DEFAULT }, // skybox
			};

		static const std::map<uint32_t, std::string> ENTITY_SKYBOX_FACE = {
			{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, "./res/skybox_side.bmp" },
			{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "./res/skybox_side.bmp" },
			{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "./res/skybox_top.bmp" },
			{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "./res/skybox_bottom.bmp" },
			{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "./res/skybox_side.bmp" },
			{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "./res/skybox_side.bmp" },
			};

		static const renderer_config MESSAGE_RENDERER_CONFIGURATION = { "./res/vert_string_static.glsl", "./res/frag_string_static.glsl",
				RENDER_ORTHOGONAL, RENDERER_BLEND_DEFAULT, RENDERER_BLEND_DFACTOR_DEFAULT, RENDERER_BLEND_SFACTOR_DEFAULT,
				RENDERER_CULL_MODE_DEFAULT, RENDERER_CULL_MODE_DEFAULT, RENDERER_DEPTH_DEFAULT, RENDERER_DEPTH_MODE_DEFAULT };

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
			m_camera(nullptr),
			m_chunk_renderer(nullptr),
			m_debug(SESSION_DEBUG_DEFAULT),
			m_manager_display(nomic::graphic::display::acquire()),
			m_manager_entity(nomic::entity::manager::acquire()),
			m_manager_font(nomic::font::manager::acquire()),
			m_manager_graphic(nomic::graphic::manager::acquire()),
			m_manager_render(nomic::render::manager::acquire()),
			m_manager_terrain(nomic::terrain::manager::acquire()),
			m_runtime(nullptr)
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

		nomic::entity::camera *
		manager::camera(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", m_camera);
			return m_camera;
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
						entry->register_renderer(m_chunk_renderer->get_id());
					}
				}
			}

			for(int32_t z = -VIEW_RADIUS_RUNTIME; z < VIEW_RADIUS_RUNTIME; ++z) {

				for(int32_t x = -VIEW_RADIUS_RUNTIME; x < VIEW_RADIUS_RUNTIME; ++x) {
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
						m_manager_terrain.at(glm::ivec2(x, z), false)->update(right, left, back, front);
					}
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::generate_chunks_spawn(void)
		{
			nomic::core::renderer diagnostic_renderer, message_renderer;
			uint32_t completed = 0, current, previous = 0, total = (VIEW_WIDTH * VIEW_WIDTH);

			TRACE_ENTRY(LEVEL_VERBOSE);

			const renderer_config &config = DEBUG_RENDERER_CONFIGURATION.at(DEBUG_OBJECT_DIAGNOSTIC);
			diagnostic_renderer.set_shaders(std::get<RENDERER_SHADER_VERTEX>(config), std::get<RENDERER_SHADER_FRAGMENT>(config));
			diagnostic_renderer.set_mode(std::get<RENDERER_MODE>(config));
			diagnostic_renderer.set_blend(std::get<RENDERER_BLEND_ENABLED>(config), std::get<RENDERER_BLEND_SFACTOR>(config),
				std::get<RENDERER_BLEND_DFACTOR>(config));
			diagnostic_renderer.set_cull(std::get<RENDERER_CULL_ENABLED>(config), std::get<RENDERER_CULL_MODE>(config));
			diagnostic_renderer.set_depth(std::get<RENDERER_DEPTH_ENABLED>(config), std::get<RENDERER_DEPTH_MODE>(config));

			nomic::entity::diagnostic diagnostic(false);
			diagnostic.enable(true);
			diagnostic.show(true);
			diagnostic.register_renderer(diagnostic_renderer.get_id());

			nomic::entity::message message;
			message_renderer.set_shaders(std::get<RENDERER_SHADER_VERTEX>(MESSAGE_RENDERER_CONFIGURATION),
				std::get<RENDERER_SHADER_FRAGMENT>(MESSAGE_RENDERER_CONFIGURATION));
			message_renderer.set_mode(std::get<RENDERER_MODE>(MESSAGE_RENDERER_CONFIGURATION));
			message_renderer.set_blend(std::get<RENDERER_BLEND_ENABLED>(MESSAGE_RENDERER_CONFIGURATION),
				std::get<RENDERER_BLEND_SFACTOR>(MESSAGE_RENDERER_CONFIGURATION),
				std::get<RENDERER_BLEND_DFACTOR>(MESSAGE_RENDERER_CONFIGURATION));
			message_renderer.set_cull(std::get<RENDERER_CULL_ENABLED>(MESSAGE_RENDERER_CONFIGURATION),
				std::get<RENDERER_CULL_MODE>(MESSAGE_RENDERER_CONFIGURATION));
			message_renderer.set_depth(std::get<RENDERER_DEPTH_ENABLED>(MESSAGE_RENDERER_CONFIGURATION),
				std::get<RENDERER_DEPTH_MODE>(MESSAGE_RENDERER_CONFIGURATION));

			message.enable(true);
			message.show(true);
			message.register_renderer(message_renderer.get_id());
			message.text() = "Generating chunk objects...";
			update();
			render();

			for(int32_t z = -VIEW_RADIUS_SPAWN; z < VIEW_RADIUS_SPAWN; ++z) {

				for(int32_t x = -VIEW_RADIUS_SPAWN; x < VIEW_RADIUS_SPAWN; ++x, ++completed) {

					current = (100 * (completed / (float) total));
					if(current && (current != previous)) {
						std::stringstream stream;
						stream << "Generating chunk objects... " << current << "%";
						message.text() = stream.str();
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
					entry->register_renderer(m_chunk_renderer->get_id());
				}
			}

			completed = 0;

			for(int32_t z = -VIEW_RADIUS_SPAWN; z < VIEW_RADIUS_SPAWN; ++z) {

				for(int32_t x = -VIEW_RADIUS_SPAWN; x < VIEW_RADIUS_SPAWN; ++x, ++completed) {
					uint32_t count = 0;
					nomic::entity::chunk *back = nullptr, *front = nullptr, *left = nullptr, *right = nullptr;

					current = (100 * (completed / (float) total));
					if(current && (current != previous)) {
						std::stringstream stream;
						stream << "Joining chunk objects... " << current << "%";
						message.text() = stream.str();
						update();
						render();
						previous = current;
					}

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
						m_manager_terrain.at(glm::ivec2(x, z), false)->update(right, left, back, front);
					}
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::initialize_entities(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Building entity objects...");

			for(uint32_t iter = 0; iter <= ENTITY_OBJECT_MAX; ++iter) {

				m_entity_renderer.push_back(new nomic::core::renderer);
				if(!m_entity_renderer.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Renderer=%u", iter);
				}

				const renderer_config &config = ENTITY_RENDERER_CONFIGURATION.at(iter);

				nomic::core::renderer *rend = m_entity_renderer.at(iter);
				if(rend) {
					rend->set_shaders(std::get<RENDERER_SHADER_VERTEX>(config), std::get<RENDERER_SHADER_FRAGMENT>(config));
					rend->set_mode(std::get<RENDERER_MODE>(config));
					rend->set_blend(std::get<RENDERER_BLEND_ENABLED>(config), std::get<RENDERER_BLEND_SFACTOR>(config),
						std::get<RENDERER_BLEND_DFACTOR>(config));
					rend->set_cull(std::get<RENDERER_CULL_ENABLED>(config), std::get<RENDERER_CULL_MODE>(config));
					rend->set_depth(std::get<RENDERER_DEPTH_ENABLED>(config), std::get<RENDERER_DEPTH_MODE>(config));
				}

				switch(iter) {
					case ENTITY_OBJECT_SKYBOX:

						m_entity_object.push_back(new nomic::entity::skybox);
						if(!m_entity_object.at(iter)) {
							THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE,
								"Object=%u", iter);
						}

						((nomic::entity::skybox *) m_entity_object.back())->set(ENTITY_SKYBOX_FACE);
						break;
					default:
						break;
				}

				if(!m_entity_object.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Object=%u", iter);
				}

				m_entity_object.at(iter)->enable(true);
				m_entity_object.at(iter)->show(true);
				m_entity_object.at(iter)->register_renderer(m_entity_renderer.at(iter)->get_id());
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Building debug objects...");

			for(uint32_t iter = 0; iter <= DEBUG_OBJECT_MAX; ++iter) {

				m_debug_renderer.push_back(new nomic::core::renderer);
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
					case DEBUG_OBJECT_DIAGNOSTIC:
						m_debug_object.push_back(new nomic::entity::diagnostic);
						break;
					case DEBUG_OBJECT_RETICLE:
						m_debug_object.push_back(new nomic::entity::reticle);
						break;
					default:
						break;
				}

				if(!m_debug_object.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Object=%u", iter);
				}

				m_debug_object.at(iter)->defer(true);
				m_debug_object.at(iter)->enable(false);
				m_debug_object.at(iter)->show(false);
				m_debug_object.at(iter)->register_renderer(m_debug_renderer.at(iter)->get_id());
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Building chunk objects...");

			m_chunk_renderer = new nomic::core::renderer;
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

			for(uint32_t iter = 0; iter <= DEBUG_OBJECT_MAX; ++iter) {
				m_debug_object.at(iter)->enable(m_debug);
				m_debug_object.at(iter)->show(m_debug);
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

			m_camera = new nomic::entity::camera(m_manager_display.dimensions());
			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Failed to allocate camera, Address=%p", m_camera);
			}

			m_camera->enable(false);
			m_camera->show(false);
			m_manager_display.set_icon(DISPLAY_DEFAULT_ICON);
			initialize_entities();
			m_camera->position().y = BLOCK_HEIGHT_WATER;
			nomic::core::thread::start(true);
			nomic::core::thread::notify();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager initialized");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		manager::on_run(void)
		{
			bool result = true;
			glm::vec3 position;
			glm::ivec2 position_chunk;

			TRACE_ENTRY(LEVEL_VERBOSE);

			//generate_chunks_runtime();

			position = m_camera->position();
			position_chunk = glm::ivec2(position.x / CHUNK_WIDTH, position.z / CHUNK_WIDTH);

			std::map<std::pair<int32_t, int32_t>, nomic::entity::chunk *> &chunk_ref = m_manager_terrain.chunks();
			for(std::map<std::pair<int32_t, int32_t>, nomic::entity::chunk *>::iterator iter = chunk_ref.begin();
					iter != chunk_ref.end(); ++iter) {
				int32_t dx = ((iter->first.first > position_chunk.x) ? (iter->first.first - position_chunk.x) :
					(position_chunk.x - iter->first.first));
				int32_t dz = ((iter->first.second > position_chunk.y) ? (iter->first.second - position_chunk.y) :
					(position_chunk.y - iter->first.second));

				if((std::abs(dx * dx) + std::abs(dz * dz)) <= (VIEW_RADIUS_RUNTIME * VIEW_RADIUS_RUNTIME)) {

					if(!iter->second->enabled()) {
						iter->second->enable(true);
					}

					if(!iter->second->shown()) {
						iter->second->show(true);
					}
				} else {

					if(iter->second->enabled()) {
						iter->second->enable(false);
					}

					if(iter->second->shown()) {
						iter->second->show(false);
					}
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager uninitializing...");

			nomic::core::thread::stop();

			if(m_camera) {
				delete m_camera;
				m_camera = nullptr;
			}

			uninitialize_entities();
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
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f", delta);

			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Camera is not allocated, Address=%p", m_camera);
			}

			m_manager_display.clear();
			m_manager_render.render(m_camera->projection(), m_camera->view(), m_camera->dimensions(), delta);
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
				result << " Base=" << SINGLETON_CLASS(nomic::session::manager)::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		manager::toggle_debug(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			set_debug(!m_debug);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::uninitialize_entities(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

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

			TRACE_MESSAGE(LEVEL_INFORMATION, "Destroying chunk objects...");

			if(m_chunk_renderer) {
				delete m_chunk_renderer;
				m_chunk_renderer = nullptr;
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Destroying entity objects...");

			for(uint32_t iter = 0; iter < m_entity_object.size(); ++iter) {

				if(m_entity_object.at(iter)) {
					delete m_entity_object.at(iter);
					m_entity_object.at(iter) = nullptr;
				}
			}

			for(uint32_t iter = 0; iter < m_entity_renderer.size(); ++iter) {

				if(m_entity_renderer.at(iter)) {
					delete m_entity_renderer.at(iter);
					m_entity_renderer.at(iter) = nullptr;
				}
			}

			m_entity_object.clear();
			m_entity_renderer.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
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

			m_camera->update();			
			if(m_camera->moved()) {
				nomic::core::thread::notify();
			}

			m_manager_entity.update(m_runtime, m_camera);

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
