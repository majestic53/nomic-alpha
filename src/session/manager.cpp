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
#include "../../include/entity/reticle.h"
#include "../../include/graphic/vao.h"
#include "../../include/trace.h"
#include "./manager_type.h"

// TODO: DEBUGGING
#include "../../include/graphic/cubemap.h"

#define BLOCK_SEGMENT_COUNT 36
#define BLOCK_SEGMENT_WIDTH 3

enum {
	BLOCK_INDEX_VERTEX = 0,
};

static const float BLOCK_VERTEX[] = {
	-1.f, 1.f, -1.f, -1.f, -1.f, -1.f, 1.f, -1.f, -1.f, //right
	1.f, -1.f, -1.f, 1.f, 1.f, -1.f, -1.f, 1.f, -1.f,
	-1.f, -1.f, 1.f, -1.f, -1.f, -1.f, -1.f, 1.f, -1.f, // left
	-1.f, 1.f, -1.f, -1.f, 1.f, 1.f, -1.f, -1.f, 1.f,
	1.f, -1.f, -1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, // top
	1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, -1.f, -1.f,
	-1.f, -1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, 1.f, // bottom
	1.f, 1.f, 1.f, 1.f, -1.f, 1.f, -1.f, -1.f, 1.f,
	-1.f, 1.f, -1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, // back
	1.f, 1.f, 1.f, -1.f, 1.f, 1.f, -1.f, 1.f, -1.f,
	-1.f, -1.f, -1.f, -1.f, -1.f, 1.f, 1.f, -1.f, -1.f, // front
	1.f, -1.f, -1.f, -1.f, -1.f, 1.f, 1.f, -1.f, 1.f,
	};

static const std::map<uint32_t, std::string> BLOCK_PATH = {
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, "./res/block_right.bmp" }, // right
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "./res/block_left.bmp" }, // left
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "./res/block_top.bmp" }, // top
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "./res/block_bottom.bmp" }, // bottom
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "./res/block_back.bmp" }, // back
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "./res/block_forward.bmp" }, // forward
	};

nomic::core::renderer *block_renderer = nullptr;
nomic::graphic::vao *block_vao = nullptr;
nomic::graphic::cubemap *block_cubemap = nullptr;
// ---

namespace nomic {

	namespace session {

		enum {
			DEBUG_OBJECT_AXIS = 0,
			DEBUG_OBJECT_DIAGNOSTIC,
			DEBUG_OBJECT_RETICLE,
		};

		#define DEBUG_OBJECT_MAX DEBUG_OBJECT_RETICLE

		enum {
			DEBUG_TUPLE_SHADER_VERTEX = 0,
			DEBUG_TUPLE_SHADER_FRAGMENT,
			DEBUG_TUPLE_MODE,
		};

		static const std::vector<std::tuple<std::string, std::string, bool>> DEBUG_RENDERER_COFIGURATION = {
			{ "./res/vert_axis.glsl", "./res/frag_axis.glsl", RENDER_PERSPECTIVE },
			{ "./res/vert_string_static.glsl", "./res/frag_string_static.glsl", RENDER_ORTHOGONAL },
			{ "./res/vert_reticle.glsl", "./res/frag_reticle.glsl", RENDER_PERSPECTIVE },
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
			m_manager_render(nomic::render::manager::acquire()),
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

			m_camera = new nomic::entity::camera(m_manager_display.dimensions());
			if(!m_camera) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_EXTERNAL,
					"Failed to allocate camera, Address=%p", m_camera);
			}

			m_camera->enable(false);
			m_camera->show(false);

			// TODO: configure display
			m_manager_display.set_icon(DISPLAY_DEFAULT_ICON);
			// ---

			for(size_t iter = 0; iter <= DEBUG_OBJECT_MAX; ++iter) {

				m_debug_renderer.push_back(new nomic::core::renderer);
				if(!m_debug_renderer.at(iter)) {
					THROW_NOMIC_SESSION_MANAGER_EXCEPTION_FORMAT(NOMIC_SESSION_MANAGER_EXCEPTION_ALLOCATE, "Renderer=%u", iter);
				}

				m_debug_renderer.at(iter)->set_shaders(std::get<DEBUG_TUPLE_SHADER_VERTEX>(DEBUG_RENDERER_COFIGURATION.at(iter)),
					std::get<DEBUG_TUPLE_SHADER_FRAGMENT>(DEBUG_RENDERER_COFIGURATION.at(iter)));
				m_debug_renderer.at(iter)->set_mode(std::get<DEBUG_TUPLE_MODE>(DEBUG_RENDERER_COFIGURATION.at(iter)));

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

				m_debug_object.at(iter)->enable(m_debug);
				m_debug_object.at(iter)->show(m_debug);
				m_debug_object.at(iter)->register_renderer(m_debug_renderer.at(iter)->get_id());
			}

// TODO: DEBUGGING

			block_renderer = new nomic::core::renderer;
			if(!block_renderer) {
				THROW_EXCEPTION("Failed to allocate renderer!");
			}

			block_renderer->set_shaders("./res/vert_block.glsl", "./res/frag_block.glsl");
			block_renderer->set_mode(RENDER_PERSPECTIVE);

			block_vao = new nomic::graphic::vao;
			if(!block_vao) {
				THROW_EXCEPTION("Failed to allocate vao!");
			}

			block_vao->add(nomic::graphic::vbo(GL_ARRAY_BUFFER, std::vector<uint8_t>((uint8_t *) &BLOCK_VERTEX[0],
				((uint8_t *) &BLOCK_VERTEX[0]) + ((BLOCK_SEGMENT_COUNT * BLOCK_SEGMENT_WIDTH) * sizeof(GLfloat))), GL_STATIC_DRAW),
				BLOCK_INDEX_VERTEX, BLOCK_SEGMENT_WIDTH, GL_FLOAT);
			block_vao->enable(BLOCK_INDEX_VERTEX);

			block_cubemap = new nomic::graphic::cubemap;
			if(!block_cubemap) {
				THROW_EXCEPTION("Failed to allocate cubemap!");
			}

			block_cubemap->set(BLOCK_PATH);
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

// TODO: DEBUGGING

			if(block_cubemap) {
				delete block_cubemap;
				block_cubemap = nullptr;
			}

			if(block_vao) {
				delete block_vao;
				block_vao = nullptr;
			}

			if(block_renderer) {
				delete block_renderer;
				block_renderer = nullptr;
			}
// ---

			m_debug_object.clear();
			m_debug_renderer.clear();
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

// TODO: DEBUGGING
			block_renderer->use(m_camera->projection(), m_camera->view());
			block_cubemap->bind();
			block_vao->bind();
			GL_CHECK(LEVEL_WARNING, glDrawArrays, GL_TRIANGLES, 0, BLOCK_SEGMENT_COUNT);
// ---

			m_manager_render.render(m_camera->projection(), m_camera->view(), m_camera->dimensions(), delta);
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

			if(!m_initialized) {
				THROW_NOMIC_SESSION_MANAGER_EXCEPTION(NOMIC_SESSION_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			m_runtime = runtime;

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
			TRACE_MESSAGE(LEVEL_INFORMATION, "Session manager unpaused");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::update(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_manager_entity.update(m_runtime, m_camera);

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
