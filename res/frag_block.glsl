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

in vec2 out_coordinate;
in vec3 out_vertex;

uniform vec3 position;
uniform bool underwater;

uniform sampler2D out_texture;

const vec4 FOG_COLOR = vec4(0.52f, 0.64f, 0.99f, 1.f);
const float FOG_FALLOFF = 0.003f;

vec4 
add_effect_air(
	in vec4 color,
	in float distance
	)
{
	float density = (1.f - exp(-distance * FOG_FALLOFF));

	return mix(color, FOG_COLOR, density);
}

const vec4 WATER_COLOR = vec4(0.02f, 0.26f, 0.45f, 1.f);
const float WATER_FALLOFF = 0.1f;

vec4 
add_effect_water(
	in vec4 color,
	in float distance
	)
{
	float density = (1.f - exp(-distance * WATER_FALLOFF));

	return mix(color, WATER_COLOR, density);
}

void
main(void)
{

	if(underwater) {
		gl_FragColor = add_effect_water(texture(out_texture, out_coordinate), distance(position, out_vertex));
	} else {
		gl_FragColor = add_effect_air(texture(out_texture, out_coordinate), distance(position, out_vertex));
	}
}
