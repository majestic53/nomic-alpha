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

#ifndef NOMIC_ENTITY_STRING_H_
#define NOMIC_ENTITY_STRING_H_

#include "../font/manager.h"
#include "./object.h"

namespace nomic {

	namespace entity {

		class string :
				public nomic::entity::object {

			public:

				explicit string(
					__in_opt const std::string &path = STRING_FONT_DEFAULT,
					__in_opt uint32_t size = STRING_FONT_SIZE_DEFAULT,
					__in_opt const glm::uvec2 &position = STRING_POSITION_DEFAULT,
					__in_opt const glm::vec4 &color = STRING_COLOR_DEFAULT,
					__in_opt float scale = STRING_SCALE_DEFAULT
					);

				string(
					__in const string &other
					);

				virtual ~string(void);

				string &operator=(
					__in const string &other
					);

				void on_render(
					__in nomic::core::renderer &renderer,
					__in float delta
					);

				virtual void on_update(
					__in void *runtime,
					__in void *camera
					);

				void on_view_change(void);

				glm::uvec2 position(void);

				float scale(void);

				void set_color(
					__in const glm::vec4 &color
					);

				void set_font(
					__in const std::string &path,
					__in_opt uint32_t size = STRING_FONT_SIZE_DEFAULT
					);

				void set_position(
					__in const glm::uvec2 &position
					);

				void set_scale(
					__in float scale
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void setup(void);

				void teardown(void);

				glm::vec4 m_color;

				uint32_t m_font_id;

				nomic::font::manager &m_manager_font;

				std::string m_path;

				glm::uvec2 m_position;

				float m_scale;

				uint32_t m_size;

				std::string m_text;
		};
	}
}

#endif // NOMIC_ENTITY_STRING_H_
