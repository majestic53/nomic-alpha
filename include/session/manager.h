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

#ifndef NOMIC_SESSION_MANAGER_H_
#define NOMIC_SESSION_MANAGER_H_

#include <vector>
#include "../core/thread.h"
#include "../entity/camera.h"
#include "../entity/chunk.h"
#include "../entity/manager.h"
#include "../font/manager.h"
#include "../graphic/atlas.h"
#include "../graphic/display.h"
#include "../graphic/manager.h"
#include "../render/manager.h"
#include "../terrain/manager.h"

namespace nomic {

	namespace session {

		class manager :
				public SINGLETON_CLASS(nomic::session::manager),
				protected nomic::core::thread {

			public:

				~manager(void);

				bool block_selected(void);

				nomic::entity::camera *camera(void);

				void pause(void);

				void render(
					__in_opt float delta = DELTA_FULL
					);

				uint32_t seed(void);

				void selected_block(
					__inout glm::ivec2 &chunk,
					__inout glm::uvec3 &block
					);

				void set_debug(
					__in bool debug
					);

				void set_dimensions(
					__in const glm::uvec2 &dimensions
					);

				void set_fullscreen(
					__in bool fullscreen
					);

				void set_icon(
					__in const std::string &path
					);

				void set_runtime(
					__in void *runtime
					);

				void set_seed(
					__in uint32_t seed,
					__in_opt uint32_t octaves = NOISE_OCTAVES_DEFAULT,
					__in_opt double amplitude = NOISE_AMPLITUDE_DEFAULT,
					__in_opt uint32_t max = CHUNK_MAX_DEFAULT
					);

				void set_spawn(
					__in const glm::vec3 &position
					);

				void set_title(
					__in const std::string &title
					);

				void set_vsync(
					__in bool vsync
					);

				glm::vec3 spawn(void);

				nomic::terrain::manager &terrain(void);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void toggle_debug(void);

				bool underwater(void);

				void unpause(void);

				void update(void);

			protected:

				SINGLETON_CLASS_BASE(nomic::session::manager);

				manager(void);

				manager(
					__in const manager &other
					) = delete;

				manager &operator=(
					__in const manager &other
					) = delete;

				void generate_chunks_runtime(void);

				void generate_chunks_spawn(void);

				void generate_spawn_location(void);

				void initialize_entities(void);

				bool on_initialize(void);

				bool on_run(void);

				void on_uninitialize(void);

				void uninitialize_entities(void);

				void update_position(void);

				void update_selector(void);

				void update_underwater(void);

				nomic::graphic::atlas *m_atlas;

				bool m_block_selected;

				glm::uvec3 m_block_selected_block;

				glm::ivec2 m_block_selected_chunk;

				nomic::entity::camera *m_camera;

				nomic::core::renderer *m_chunk_renderer;

				bool m_debug;

				std::vector<nomic::entity::object *> m_debug_object;

				std::vector<nomic::core::renderer *> m_debug_renderer;

				std::vector<nomic::entity::object *> m_entity_object_background;

				std::vector<nomic::entity::object *> m_entity_object_foreground;

				std::vector<nomic::core::renderer *> m_entity_renderer_background;

				std::vector<nomic::core::renderer *> m_entity_renderer_foreground;

				nomic::graphic::display &m_manager_display;

				nomic::entity::manager &m_manager_entity;

				nomic::font::manager &m_manager_font;

				nomic::graphic::manager &m_manager_graphic;

				nomic::render::manager &m_manager_render;

				nomic::terrain::manager &m_manager_terrain;

				void *m_runtime;

				glm::vec3 m_spawn;

				bool m_underwater;
		};
	}
}

#endif // NOMIC_SESSION_MANAGER_H_
