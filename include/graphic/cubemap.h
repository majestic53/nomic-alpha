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

#ifndef NOMIC_GRAPHIC_CUBEMAP_H_
#define NOMIC_GRAPHIC_CUBEMAP_H_

#include <map>
#include "../core/primitive.h"

namespace nomic {

	namespace graphic {

		class cubemap :
				public nomic::core::primitive {

			public:

				explicit cubemap(
					__in_opt const std::map<uint32_t, std::string> &path = std::map<uint32_t, std::string>(),
					__in_opt float scale = CUBEMAP_SCALE_DEFAULT,
					__in_opt GLenum wrap_s = CUBEMAP_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = CUBEMAP_WRAP_T_DEFAULT,
					__in_opt GLenum wrap_r = CUBEMAP_WRAP_R_DEFAULT,
					__in_opt GLenum filter_min = CUBEMAP_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = CUBEMAP_FILTER_MAG_DEFAULT
					);

				cubemap(
					__in const cubemap &other
					);

				virtual ~cubemap(void);

				cubemap &operator=(
					__in const cubemap &other
					);

				void bind(void);

				void set(
					__in const std::map<uint32_t, std::string> &path,
					__in_opt float scale = CUBEMAP_SCALE_DEFAULT,
					__in_opt GLenum wrap_s = CUBEMAP_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = CUBEMAP_WRAP_T_DEFAULT,
					__in_opt GLenum wrap_r = CUBEMAP_WRAP_R_DEFAULT,
					__in_opt GLenum filter_min = CUBEMAP_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = CUBEMAP_FILTER_MAG_DEFAULT
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;
		};
	}
}

#endif // NOMIC_GRAPHIC_CUBEMAP_H_
