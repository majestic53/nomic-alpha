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
in float out_distance;
in vec3 out_normal;
in vec3 out_normal_position;
in vec3 out_vertex;

uniform vec4 ambient;
uniform vec3 ambient_position;
uniform float cycle;
uniform vec3 position;
uniform vec3 rotation;
uniform bool underwater;

uniform sampler2D out_texture;

const float ALPHA_LIMIT = 0.2f;

const vec4 AMBIENT_DARK = vec4(0.f, 0.f, 0.f, 1.f);
const float AMBIENT_DARK_END = 0.1f;
const float AMBIENT_DARK_MIN = 0.0f;
const float AMBIENT_DARK_MAX = 1.f;
const float AMBIENT_DARK_START = 0.9f;
const float AMBIENT_FOG_BLEND = 0.6f;

const float DIFFUSE_SCALE = 0.2f;

const vec4 FOG_COLOR = vec4(0.54f, 0.70f, 1.f, 1.f);
const float FOG_FALLOFF = 0.004f;

const vec4 WATER_COLOR = vec4(0.06f, 0.25f, 1.f, 1.f);
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
add_light_diffuse(
	in float cycle,
	in vec3 normal,
	in vec3 normal_position,
	in vec4 ambient,
	in vec3 ambient_position
	)
{
	float scale;
	vec4 diffuse;

	if(cycle <= AMBIENT_DARK_END) {
		scale = (1.f - (cycle / AMBIENT_DARK_END));
	} else if((cycle > AMBIENT_DARK_END) && (cycle <= AMBIENT_DARK_START)) {
		scale = 0.f;
	} else {
		scale = ((cycle - AMBIENT_DARK_START) / (1.f - AMBIENT_DARK_START));
	}

	diffuse = (max(dot(normalize(normal), normalize(ambient_position - normal_position)), 0.f) * ambient);

	return (mix(diffuse, AMBIENT_DARK, clamp(scale, AMBIENT_DARK_MIN, AMBIENT_DARK_MAX)) * DIFFUSE_SCALE);
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

vec4 
add_fog_non_constant(
	in vec4 color,
	in vec4 color_fog,
	in float falloff,
	in float distance,
	in vec3 position,
	in vec3 rotation
	)
{
	float density = (exp(-position.y * falloff) * ((1.f - exp(-distance * falloff * rotation.y)) / rotation.y));

	return mix(color, color_fog, clamp(density, 0.f, 1.f));
}

void
main(void)
{
	vec4 color = texture(out_texture, out_coordinate);

	if(color.a < ALPHA_LIMIT) {
		discard;
	}

	color *= (add_light_ambient(cycle, ambient) + add_light_diffuse(cycle, out_normal, out_normal_position, ambient, ambient_position));

	if(underwater) { // underwater
		color = add_fog_constant(color, add_light_ambient(cycle, WATER_COLOR), WATER_FALLOFF, out_distance);
	} else { // above water
		color = add_fog_non_constant(color, add_light_ambient(cycle, FOG_COLOR), FOG_FALLOFF, out_distance,
				position, rotation);
	}

	gl_FragColor = color;
}
