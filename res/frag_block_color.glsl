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

in vec4 out_color;
in vec2 out_coordinate;
in vec3 out_position;
in vec3 out_vertex;

const vec4 fog_color = vec4(0.52f, 0.64f, 0.99f, 1.f);
const float fog_falloff = 0.01f;
const float fog_scale = 0.8f;

vec4 
add_fog(
	in vec4 color,
	in float dist
	)
{
	float fog_density = fog_scale * (1.f - exp(-dist * fog_falloff));

	return mix(color, fog_color, fog_density);
}

void
main(void)
{
	gl_FragColor = add_fog(out_color, distance(out_vertex, out_position));
}
