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

#ifndef NOMIC_GRAPHIC_SHADER_H_
#define NOMIC_GRAPHIC_SHADER_H_

#include "../core/primitive.h"

namespace nomic {

	namespace graphic {

		class shader :
				public nomic::core::primitive {

			public:

				explicit shader(
					__in GLenum type,
					__in_opt const std::string &input = std::string(),
					__in_opt bool is_file = true
					);

				shader(
					__in const shader &other
					);

				virtual ~shader(void);

				shader &operator=(
					__in const shader &other
					);

				void set(
					__in const std::string &input,
					__in_opt bool is_file = true
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;
		};
	}
}

#endif // NOMIC_GRAPHIC_SHADER_H_
