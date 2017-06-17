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
#include "../../include/session/manager.h"
#include "../../include/core/renderer.h"
#include "../../include/entity/axis.h"
#include "../../include/entity/reticle.h"
#include "../../include/graphic/vao.h"
#include "../../include/trace.h"
#include "./manager_type.h"

// TODO: DEBUG
uint32_t font_id = 0;
std::string string_test = "<DEBUG>";

#define STRING_SEGMENT_COUNT 6
#define STRING_SEGMENT_WIDTH 4

enum {
	STRING_INDEX_VERTEX = 0,
};

GLuint string_vbo_id;
nomic::graphic::vao *string_vao = nullptr;
nomic::core::renderer *string_renderer = nullptr;
// ---

namespace nomic {

	namespace session {

		enum {
			DEBUG_OBJECT_AXIS = 0,
			DEBUG_OBJECT_RETICLE,
		};

		#define DEBUG_OBJECT_MAX DEBUG_OBJECT_RETICLE

		static const std::vector<std::pair<std::string, std::string>> DEBUG_RENDERER_SHADER = {
			{ "./res/vert_axis.glsl", "./res/frag_axis.glsl", },
			{ "./res/vert_reticle.glsl", "./res/frag_reticle.glsl", },
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
			m_camera(nullptr),
			m_debug(SESSION_DEBUG_DEFAULT),
			m_manager_display(nomic::graphic::display::acquire()),
			m_manager_entity(nomic::entity::manager::acquire()),
			m_manager_font(nomic::font::manager::acquire()),
			m_manager_graphic(nomic::graphic::manager::acquire()),
			m_manager_render(nomic::render::manager::acquire())
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

			// TODO: configure display
			m_manager_display.set_icon(DISPLAY_DEFAULT_ICON);
			// ---

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

			// TODO: initialize gl managers

// TODO
			font_id = m_manager_font.load("./res/FreeSans.ttf", 16);

			string_renderer = new nomic::core::renderer;
			string_renderer->set_shaders("./res/vert_string_static.glsl", "./res/frag_string_static.glsl");

			string_vao = new nomic::graphic::vao;
			string_vbo_id = string_vao->add(nomic::graphic::vbo(GL_ARRAY_BUFFER,
				STRING_SEGMENT_COUNT * STRING_SEGMENT_WIDTH * sizeof(GLfloat), GL_DYNAMIC_DRAW), STRING_INDEX_VERTEX,
				STRING_SEGMENT_WIDTH, GL_FLOAT);
			string_vao->enable(STRING_INDEX_VERTEX);
// ---

			m_camera = new nomic::graphic::camera(glm::uvec2(DISPLAY_DEFAULT_WIDTH, DISPLAY_DEFAULT_HEIGHT));
			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Failed to allocate camera, Address=%p", m_camera);
			}

			m_camera->enable(false);
			m_camera->show(false);

			for(size_t iter = 0; iter <= DEBUG_OBJECT_MAX; ++iter) {

				m_debug_renderer.push_back(new nomic::core::renderer);
				if(!m_debug_renderer.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Renderer=%u", iter);
				}

				m_debug_renderer.at(iter)->set_shaders(DEBUG_RENDERER_SHADER.at(iter).first, DEBUG_RENDERER_SHADER.at(iter).second);

				switch(iter) {
					case DEBUG_OBJECT_AXIS:
						m_debug_object.push_back(new nomic::entity::axis);
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

				m_debug_object.at(iter)->enable(m_debug);
				m_debug_object.at(iter)->show(m_debug);
				m_debug_object.at(iter)->register_renderer(m_debug_renderer.at(iter)->get_id());
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager initialized");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager uninitializing...");

			if(m_camera) {
				delete m_camera;
				m_camera = nullptr;
			}

			for(size_t iter = 0; iter <= DEBUG_OBJECT_MAX; ++iter) {

				if(m_debug_object.at(iter)) {
					delete m_debug_object.at(iter);
					m_debug_object.at(iter) = nullptr;
				}

				if(m_debug_renderer.at(iter)) {
					delete m_debug_renderer.at(iter);
					m_debug_renderer.at(iter) = nullptr;
				}
			}

			m_debug_object.clear();
			m_debug_renderer.clear();

// TODO: DEBUG
			if(string_vao) {
				delete string_vao;
				string_vao = nullptr;
			}

			if(string_renderer) {
				delete string_renderer;
				string_renderer = nullptr;
			}
// ---

			// TODO: uninitialize gl managers

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

			// TODO: handle pause event

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager paused");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::render(
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Delta=%f", delta);

			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Camera is not allocated, Address=%p", m_camera);
			}

			m_camera->render(delta);
			m_manager_display.clear();
			m_manager_render.render(m_camera->projection(), m_camera->view(), delta);

// TODO: DEBUG
			string_renderer->use(glm::ortho(0.f, (float) DISPLAY_DEFAULT_WIDTH, 0.f, (float) DISPLAY_DEFAULT_HEIGHT, -1.f, 1.f));
			string_renderer->set_uniform(string_renderer->uniform_location("out_color"), glm::vec3(1.f, 1.f, 1.f));
			GL_CHECK(LEVEL_WARNING, glActiveTexture, GL_TEXTURE0);
			string_vao->bind();

			GLfloat x = 25.f, y = (DISPLAY_DEFAULT_HEIGHT - 25.f);

			for(std::string::iterator iter = string_test.begin(); iter != string_test.end(); ++iter) {
				nomic::graphic::character &ch = m_manager_font.character(font_id, *iter);

				GLfloat xpos = x + (ch.bearing().x * 1.f),
					ypos = y - ((ch.dimension().y - ch.bearing().y) * 1.f),
					w = (ch.dimension().x * 1.f),
					h = (ch.dimension().y * 1.f),
					vertices[STRING_SEGMENT_COUNT][STRING_SEGMENT_WIDTH] = {
						{ xpos, ypos + h, 0.f, 0.f },
						{ xpos, ypos, 0.f, 1.f },
						{ xpos + w, ypos, 1.f, 1.f },
						{ xpos, ypos + h, 0.f, 0.f },
						{ xpos + w, ypos, 1.f, 1.f },
						{ xpos + w, ypos + h, 1.f, 0.f },
					};

				ch.bind();
				string_vao->set_subdata(string_vbo_id, 0, sizeof(vertices), vertices);
				GL_CHECK(LEVEL_WARNING, glBindBuffer, GL_ARRAY_BUFFER, 0);
				GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, STRING_SEGMENT_COUNT);

				x += ((ch.advance() >> 6) * 1.f);
			}

			GL_CHECK(LEVEL_WARNING, glBindVertexArray, 0);
			GL_CHECK(LEVEL_WARNING, glBindTexture, GL_TEXTURE_2D, 0);
// ---

			m_manager_display.show();

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
			__in const glm::uvec2 &dimension
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimension={%u, %u}", dimension.x, dimension.y);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Camera is not allocated, Address=%p", m_camera);
			}

			m_manager_display.set_dimensions(dimension);
			m_camera->set_dimensions(dimension);

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
		manager::unpause(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager unpausing...");

			// TODO: handle unpause event

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager unpaused");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::update(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			// TODO: handle update event

			m_manager_entity.update();

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
