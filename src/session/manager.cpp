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
#include "../../include/trace.h"
#include "./manager_type.h"

// TODO: DEBUG
#include "../../include/core/renderer.h"
#include "../../include/graphic/vao.h"

#define CURSOR_RATIO (DISPLAY_DEFAULT_HEIGHT / (float) DISPLAY_DEFAULT_WIDTH)
#define CURSOR_WIDTH 0.04f

const float CURSOR_COLOR[] = {
	0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, // grey
	0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, // grey
	};

const float CURSOR_VERTEX[] = {
	-CURSOR_WIDTH * CURSOR_RATIO, 0.f, 0.f, CURSOR_WIDTH * CURSOR_RATIO, 0.f, 0.f, // horizontal
	0.0f, -CURSOR_WIDTH, 0.f, 0.f, CURSOR_WIDTH, 0.f,  // vertical
	};

const float AXIS_COLOR[] = {
	1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, // red x
	0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, // green y
	0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, // blue z
	};

const float AXIS_VERTEX[] = {
	0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.75f, 0.25f, 0.f, 1.f, 0.f, 0.f, 0.75f, -0.25f, 0.f, // x
	0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.25f, 0.75f, 0.f, 0.f, 1.f, 0.f, -0.25f, 0.75f, 0.f, // y
	0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.25f, 0.75f, 0.f, 0.f, 1.f, 0.f, -0.25f, 0.75f, // z
	};

GLint model_id = 0, projection_id = 0, view_id = 0;
nomic::graphic::vao *vao_axis = nullptr, *vao_cursor = nullptr;
nomic::core::renderer *prog_axis = nullptr, *prog_cursor = nullptr;
// ---

namespace nomic {

	namespace session {

		static const std::map<SDL_GLattr, GLint> DISPLAY_ATTRIBUTE = {
			std::pair<SDL_GLattr, GLint>(SDL_GL_ACCELERATED_VISUAL, GL_ATTRIBUTE_ACCELERATE_VISUAL),
			std::pair<SDL_GLattr, GLint>(SDL_GL_BLUE_SIZE, GL_ATTRIBUTE_COLOR_SIZE),
			std::pair<SDL_GLattr, GLint>(SDL_GL_CONTEXT_MAJOR_VERSION, GL_ATTRIBUTE_MAJOR_VERSION),
			std::pair<SDL_GLattr, GLint>(SDL_GL_CONTEXT_MINOR_VERSION, GL_ATTRIBUTE_MINOR_VERSION),
			std::pair<SDL_GLattr, GLint>(SDL_GL_DEPTH_SIZE, GL_ATTRIBUTE_DEPTH_SIZE),
			std::pair<SDL_GLattr, GLint>(SDL_GL_DOUBLEBUFFER, GL_ATTRIBUTE_DOUBLEBUFFER),
			std::pair<SDL_GLattr, GLint>(SDL_GL_GREEN_SIZE, GL_ATTRIBUTE_COLOR_SIZE),
			std::pair<SDL_GLattr, GLint>(SDL_GL_RED_SIZE, GL_ATTRIBUTE_COLOR_SIZE),
			};

		manager::manager(void) :
			m_camera(nullptr),
			m_manager_display(nomic::graphic::display::acquire()),
			m_manager_entity(nomic::entity::manager::acquire()),
			m_manager_graphic(nomic::graphic::manager::acquire())
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		manager::~manager(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_manager_display.release();
			m_manager_entity.release();
			m_manager_graphic.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::on_initialize(void)
		{
			GLenum err;
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager initializing...");

			for(std::map<SDL_GLattr, GLint>::const_iterator iter = DISPLAY_ATTRIBUTE.begin(); iter != DISPLAY_ATTRIBUTE.end();
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

			m_manager_graphic.initialize();
			m_manager_entity.initialize();

			// TODO: initialize gl managers

			m_camera = new nomic::graphic::camera(glm::uvec2(DISPLAY_DEFAULT_WIDTH, DISPLAY_DEFAULT_HEIGHT));
			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Failed to allocate camera, Address=%p", m_camera);
			}

// TODO: DEBUG

			vao_axis = new nomic::graphic::vao;
			if(!vao_axis) {
				THROW_EXCEPTION("Allocation failed!");
			}

			vao_axis->add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &AXIS_COLOR[0],
				((uint8_t *) &AXIS_COLOR[0]) + (54 * sizeof(GLfloat))), GL_STATIC_DRAW), 0, 3, GL_FLOAT);
			vao_axis->enable(0);

			vao_axis->add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &AXIS_VERTEX[0],
				((uint8_t *) &AXIS_VERTEX[0]) + (54 * sizeof(GLfloat))), GL_STATIC_DRAW), 1, 3, GL_FLOAT);
			vao_axis->enable(1);

			prog_axis = new nomic::core::renderer;
			if(!prog_axis) {
				THROW_EXCEPTION("Allocation failed!");
			}

			prog_axis->set_shaders("./res/vert_axis.glsl", "./res/frag_axis.glsl");

			vao_cursor = new nomic::graphic::vao;
			if(!vao_cursor) {
				THROW_EXCEPTION("Allocation failed!");
			}

			vao_cursor->add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &CURSOR_COLOR[0],
				((uint8_t *) &CURSOR_COLOR[0]) + (12 * sizeof(GLfloat))), GL_STATIC_DRAW), 0, 3, GL_FLOAT);
			vao_cursor->enable(0);

			vao_cursor->add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &CURSOR_VERTEX[0],
				((uint8_t *) &CURSOR_VERTEX[0]) + (12 * sizeof(GLfloat))), GL_STATIC_DRAW), 1, 3, GL_FLOAT);
			vao_cursor->enable(1);

			prog_cursor = new nomic::core::renderer;
			if(!prog_cursor) {
				THROW_EXCEPTION("Allocation failed!");
			}

			prog_cursor->set_shaders("./res/vert_cursor.glsl", "./res/frag_cursor.glsl");
// ---

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

			// TODO: uninitialize gl managers

// TODO: DEBUG

			if(prog_axis) {
				delete prog_axis;
				prog_axis = nullptr;
			}

			if(prog_cursor) {
				delete prog_cursor;
				prog_cursor = nullptr;
			}

			if(vao_axis) {
				vao_axis->disable_all();
				delete vao_axis;
				vao_axis = nullptr;
			}

			if(vao_cursor) {
				vao_cursor->disable_all();
				delete vao_cursor;
				vao_cursor = nullptr;
			}

			model_id = 0;
			projection_id = 0;
			view_id = 0;
// ---

			m_manager_entity.uninitialize();
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

			// TODO: handle render event

// TODO: DEBUG
			prog_cursor->use();
			vao_cursor->bind();
			GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_LINES, 0, 4);

			prog_axis->use(m_camera->projection(), m_camera->view());
			vao_axis->bind();
			GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_LINES, 0, 18);
// ---

			m_manager_display.show();

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
