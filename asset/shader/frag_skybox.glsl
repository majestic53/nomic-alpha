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

in vec3 out_vertex;

uniform vec4 ambient_background;
uniform bool clouds;
uniform float cycle;
uniform bool underwater;

uniform samplerCube out_cube;

const vec4 AMBIENT_DARK = vec4(0.f, 0.f, 0.f, 1.f);
const float AMBIENT_DARK_END = 0.05f;
const float AMBIENT_DARK_MIN = 0.1f;
const float AMBIENT_DARK_MAX = 0.9f;
const float AMBIENT_DARK_START = 0.95f;

const vec4 CLOUD_COLOR = vec4(0.8f, 0.83f, 0.84f, 1.f);
const float CLOUD_FALLOFF = 0.08f;

const vec4 FOG_COLOR_DEFAULT = vec4(0.34f, 0.71f, 0.88f, 1.f);
const vec4 FOG_COLOR_MC = vec4(0.54f, 0.70f, 1.f, 1.f);
const float FOG_FALLOFF = 0.004f;

const float SKYBOX_DISTANCE = 100.f;

const vec4 WATER_COLOR = vec4(0.25f, 0.56f, 0.86f, 1.f);
const float WATER_FALLOFF = 0.08f;

vec4 
add_light_ambient(
	in float cycle,
	in vec4 ambient
	)
{
	float scale;

	if(cycle <= AMBIENT_DARK_END) {
		scale = (1.f - (cycle / AMBIENT_DARK_END));
	} else if((cycle > AMBIENT_DARK_END) && (cycle <= AMBIENT_DARK_START)) {
		scale = 0.f;
	} else {
		scale = ((cycle - AMBIENT_DARK_START) / (1.f - AMBIENT_DARK_START));
	}

	return mix(ambient, AMBIENT_DARK, clamp(scale, AMBIENT_DARK_MIN, AMBIENT_DARK_MAX));
}

vec4 
add_fog_constant(
	in vec4 color,
	in vec4 color_fog,
	in float falloff,
	in float distance
	)
{
	float density = (1.f - exp(-distance * falloff));

	return mix(color, color_fog, density);
}

void
main(void)
{
	vec4 color = texture(out_cube, out_vertex);

	color *= add_light_ambient(cycle, ambient_background);

	if(clouds) { // clouds
		color = add_fog_constant(color, add_light_ambient(cycle, CLOUD_COLOR), CLOUD_FALLOFF, SKYBOX_DISTANCE);
	} else if(underwater) { // underwater
		color = add_fog_constant(color, add_light_ambient(cycle, WATER_COLOR), WATER_FALLOFF, SKYBOX_DISTANCE);
	}

	gl_FragColor = color;
}
