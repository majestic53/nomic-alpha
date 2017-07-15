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

#ifndef NOMIC_CORE_ENTITY_H_
#define NOMIC_CORE_ENTITY_H_

#include <set>
#include "./id.h"
#include "./object.h"
#include "./renderer.h"
#include "./transform.h"

namespace nomic {

	namespace core {

		class entity :
				public nomic::core::id,
				public nomic::core::object,
				public nomic::core::transform {

			public:

				explicit entity(
					__in uint32_t type,
					__in uint32_t subtype = SUBTYPE_UNDEFINED,
					__in_opt const glm::vec3 &position = TRANSFORM_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT
					);

				entity(
					__in const entity &other
					);

				virtual ~entity(void);

				entity &operator=(
					__in const entity &other
					);

				void defer(
					__in bool state
					);

				bool deferred(void) const;

				void enable(
					__in bool state
					);

				bool enabled(void) const;

				virtual void on_render(
					__in nomic::core::renderer &renderer,
					__in void *textures,
					__in float delta
					);

				virtual void on_update(
					__in void *runtime,
					__in void *camera
					);

				virtual glm::mat4 &model(void) = 0;

				void register_renderer(
					__in GLuint id
					);

				bool registered(
					__in GLuint id
					) const;

				virtual void set_view_dimensions(
					__in const glm::vec2 &view_dimensions
					);

				void show(
					__in bool state
					);

				bool shown(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unregsiter_all_renderers(void);

				void unregister_renderer(
					__in GLuint id
					);

			protected:

				void add(void);

				void register_renderers(
					__in const std::set<GLuint> &renderer
					);

				void remove(void);

				bool m_deferred;

				bool m_enabled;

				std::set<GLuint> m_renderer;

				bool m_shown;
		};
	}
}

#endif // NOMIC_CORE_ENTITY_H_
