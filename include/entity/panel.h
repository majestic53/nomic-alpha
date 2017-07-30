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

#ifndef NOMIC_ENTITY_PANEL_H_
#define NOMIC_ENTITY_PANEL_H_

#include <tuple>
#include "./object.h"
#include "../event/input.h"
#include "../graphic/atlas.h"

namespace nomic {

	namespace entity {

		typedef std::tuple<uint8_t, uint32_t, uint32_t, uint32_t, uint32_t> panel_data;

		class panel :
				public nomic::entity::object {

			public:

				panel(
					__in const std::vector<uint8_t> &selection
					);

				panel(
					__in const panel &other
					);

				virtual ~panel(void);

				panel &operator=(
					__in const panel &other
					);

				uint8_t index(void);

				uint8_t move_left(void);

				uint8_t move_right(void);

				virtual void on_render(
					__in nomic::core::renderer &renderer,
					__in void *textures,
					__in float delta
					);

				uint8_t selected(void);

				void set_selected(
					__in uint8_t index
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void setup(
					__in const std::vector<uint8_t> &selection
					);

				void setup(
					__in const std::vector<panel_data> &selection
					);

				void setup_vertex_array(void);

				nomic::graphic::atlas m_atlas;

				uint8_t m_index;

				std::vector<panel_data> m_selection;
		};
	}
}

#endif // NOMIC_ENTITY_PANEL_H_
