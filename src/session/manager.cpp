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
			m_manager_display(nomic::graphic::display::acquire()),
			m_manager_graphic(nomic::graphic::manager::acquire())
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		manager::~manager(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_manager_display.release();
			m_manager_graphic.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::on_initialize(void)
		{
			GLenum err;
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

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

			// TODO: initialize gl managers

			m_manager_graphic.initialize();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager initialized");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			// TODO: uninitialize gl managers

			m_manager_graphic.uninitialize();
			m_manager_display.uninitialize();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager uninitialized");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::pause(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

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

			// TODO: handle render event

			m_manager_display.render();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::set_dimensions(
			__in const uint32_t width,
			__in const uint32_t height
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimension={%u, %u}", width, height);

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			m_manager_display.set_dimensions(width, height);

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

			// TODO: handle unpause event

			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager unpaused");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::update(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			// TODO: handle update event

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
