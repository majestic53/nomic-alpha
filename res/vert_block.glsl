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

layout(location = 0) in vec2 in_coordinate;
layout(location = 1) in vec3 in_vertex;

uniform mat4 model;
uniform vec3 position;
uniform mat4 projection;
uniform bool underwater;
uniform mat4 view;

out vec2 out_coordinate;
out vec3 out_position;
out vec3 out_vertex;

void
main(void)
{
	out_coordinate = in_coordinate;
	out_position = position;
	out_vertex = in_vertex;
	gl_Position = (projection * view * model * vec4(in_vertex, 1.f));
}
