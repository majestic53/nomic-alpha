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

#include "../../include/graphic/display.h"
#include "../../include/trace.h"
#include "./display_type.h"

namespace nomic {

	namespace graphic {

		display::display(void) :
			m_window(nullptr),
			m_window_context(nullptr)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		display::~display(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		display::clear(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			GL_CHECK(LEVEL_WARNING, glClearColor, DISPLAY_DEFAULT_RED, DISPLAY_DEFAULT_GREEN, DISPLAY_DEFAULT_BLUE,
				DISPLAY_DEFAULT_ALPHA);
			GL_CHECK(LEVEL_WARNING, glClear, DISPLAY_FLAGS_CLEAR);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		display::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Display manager initializing...");

			m_window = SDL_CreateWindow(DISPLAY_DEFAULT_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_DEFAULT_WIDTH,
				DISPLAY_DEFAULT_HEIGHT, DISPLAY_FLAGS_DEFAULT);

			if(!m_window) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION_FORMAT(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateWindow(%s, {%u, %u}, %i) failed! Error=%s", DISPLAY_DEFAULT_TITLE, DISPLAY_DEFAULT_WIDTH,
					DISPLAY_DEFAULT_HEIGHT, SDL_GetError());
			}

			m_window_context = SDL_GL_CreateContext(m_window);
			if(!m_window_context) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION_FORMAT(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_GL_CreateContext(%p) failed! Error=%s", m_window, SDL_GetError());
			}

			if(SDL_GL_MakeCurrent(m_window, m_window_context)) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION_FORMAT(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_GL_MakeCurrent(%p, %p) failed! Error=%s", m_window, m_window_context, SDL_GetError());
			}

			if(DISPLAY_DEFAULT_FULLSCREEN && SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN)) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION_FORMAT(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_SetWindowFullscreen(%p, %x) failed! Error=%s", m_window, SDL_WINDOW_FULLSCREEN, SDL_GetError());
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Display manager initialized");
			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Display title[%u]=%s", std::string(DISPLAY_DEFAULT_TITLE).size(),
				STRING_CHECK(std::string(DISPLAY_DEFAULT_TITLE)));
			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Display dimesions={%u, %u}", DISPLAY_DEFAULT_WIDTH, DISPLAY_DEFAULT_HEIGHT);
			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Display state=%s", (DISPLAY_DEFAULT_FULLSCREEN ? "Fullscreen" : "Windowed"));

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		display::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Display manager uninitializing...");

			if(m_window_context) {
				SDL_GL_DeleteContext(m_window_context);
				m_window_context = nullptr;
			}

			if(m_window) {
				SDL_DestroyWindow(m_window);
				m_window = nullptr;
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Display manager uninitialized");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		display::set_dimensions(
			__in const glm::uvec2 &dimension
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Dimension={%u, %u}", dimension.x, dimension.y);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			SDL_SetWindowSize(m_window, dimension.x, dimension.y);
			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Display dimesions={%u, %u}", dimension.x, dimension.y);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		display::set_fullscreen(
			__in bool fullscreen
			)
		{
			uint32_t flags;
	
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Fullscreen=%x", fullscreen);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			flags = (fullscreen ? SDL_WINDOW_FULLSCREEN : 0);

			if(SDL_SetWindowFullscreen(m_window, flags)) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION_FORMAT(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_SetWindowFullscreen(%x) failed! Error=%s", flags, SDL_GetError());
			}

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Display state=%s", (fullscreen ? "Fullscreen" : "Windowed"));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		display::set_icon(
			__in const std::string &path
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Path[%u]=%s", path.size(), STRING_CHECK(path));

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			m_window_icon.set(path);
			SDL_SetWindowIcon(m_window, m_window_icon.surface());
			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Display icon[%u]=%s", path.size(), STRING_CHECK(path));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		display::set_title(
			__in const std::string &title
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Title[%u]=%s", title.size(), STRING_CHECK(title));

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			SDL_SetWindowTitle(m_window, title.c_str());
			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Display title[%u]=%s", title.size(), STRING_CHECK(title));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		display::show(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_initialized) {
				THROW_NOMIC_GRAPHIC_DISPLAY_EXCEPTION(NOMIC_GRAPHIC_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			SDL_GL_SwapWindow(m_window);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		display::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_GRAPHIC_DISPLAY_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << SINGLETON_CLASS(nomic::graphic::display)::to_string(true);

				if(m_initialized) {
					glm::ivec2 dimension;
					bool fullscreen = ((SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) ? true : false);
					std::string title = SDL_GetWindowTitle(m_window);

					SDL_GetWindowSize(m_window, &dimension.x, &dimension.y);
					result << ", Title[" << title.size() << "]=" << title
						<< ", Dimension={" << dimension.x << ", " << dimension.y << "}"
						<< ", State=" << (fullscreen ? "Fullscreen" : "Windowed");
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
