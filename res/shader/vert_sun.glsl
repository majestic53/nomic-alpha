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

#version 330 core

layout(location = 0) in vec4 in_color;
layout(location = 1) in vec2 in_coordinate;
layout(location = 2) in vec3 in_vertex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 out_color;
out vec2 out_coordinate;

void
main(void)
{
	mat4 in_view_model = (view * model);

	// billboard
	in_view_model[0][0] = 1;
	in_view_model[0][1] = 0;
	in_view_model[0][2] = 0;

	in_view_model[1][0] = 0;
	in_view_model[1][1] = 1;
	in_view_model[1][2] = 0;

	in_view_model[2][0] = 0;
	in_view_model[2][1] = 0;
	in_view_model[2][2] = 1;

	out_color = in_color;
	out_coordinate = in_coordinate;
	gl_Position = (projection * in_view_model * vec4(in_vertex, 1.f));
}
