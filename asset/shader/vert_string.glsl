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

layout(location = 0) in vec4 in_vertex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 out_coordinate;

void
main(void)
{
	out_coordinate = in_vertex.zw;
	gl_Position = (projection * vec4(in_vertex.xy, 0.f, 1.f));
}
