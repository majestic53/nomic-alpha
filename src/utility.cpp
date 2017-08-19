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

#include <cstdarg>
#include <fstream>
#include "../include/define.h"
#include "./utility_type.h"

namespace nomic {

	#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
	#define TIMESTAMP_LENGTH 64

	glm::vec3 
	utility::block_as_position(
		__in const glm::ivec2 &chunk,
		__in const glm::uvec3 &block
		)
	{
		glm::vec3 result;

		result.x = (chunk.x * CHUNK_WIDTH);
		result.x += block.x;
		result.y = block.y;
		result.z = (chunk.y * CHUNK_WIDTH);
		result.z += block.z;

		return result;
	}

	bool 
	utility::block_decoration(
		__in uint8_t type
		)
	{
		bool result = false;

		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

		switch(type) {
			case BLOCK_CORAL_BLUE:
			case BLOCK_CORAL_BROWN:
			case BLOCK_CORAL_ORANGE:
			case BLOCK_CORAL_PINK:
			case BLOCK_CORAL_PURPLE:
			case BLOCK_FLOWER_RED:
			case BLOCK_FLOWER_YELLOW:
			case BLOCK_GRASS_SHORT:
			case BLOCK_GRASS_TALL:
			case BLOCK_SEAGRASS_GREEN:
			case BLOCK_SEAGRASS_BROWN:
			case BLOCK_SHRUB:
			case BLOCK_SUGAR_CANE:
				result = true;
				break;
			default:
				break;
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	uint8_t 
	utility::block_face_type(
		__in uint8_t type,
		__in uint32_t face,
		__inout uint8_t &attributes
		)
	{
		uint8_t result = BLOCK_AIR;

		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Face=%x, Attributes=%x", type, face, attributes);

		switch(type) {
			case BLOCK_AIR:
			case BLOCK_BOUNDARY:
			case BLOCK_BRICK:
			case BLOCK_CLAY:
			case BLOCK_CLOUD:
			case BLOCK_COBBLESTONE:
			case BLOCK_COBBLESTONE_MOSSY:
			case BLOCK_CORAL_BLUE:
			case BLOCK_CORAL_BROWN:
			case BLOCK_CORAL_ORANGE:
			case BLOCK_CORAL_PINK:
			case BLOCK_CORAL_PURPLE:
			case BLOCK_DIRT:
			case BLOCK_FLOWER_RED:
			case BLOCK_FLOWER_YELLOW:
			case BLOCK_GLASS:
			case BLOCK_GRASS_SHORT:
			case BLOCK_GRASS_TALL:
			case BLOCK_GRAVEL:
			case BLOCK_LEAVES_OAK:
			case BLOCK_LEAVES_SPRUCE:
			case BLOCK_OBSIDIAN:
			case BLOCK_ORE_GOLD:
			case BLOCK_ORE_IRON:
			case BLOCK_ORE_COAL:
			case BLOCK_ORE_DIAMOND:
			case BLOCK_PLANK_OAK:
			case BLOCK_PLANK_SPRUCE:
			case BLOCK_SAND:
			case BLOCK_SEAGRASS_GREEN:
			case BLOCK_SEAGRASS_BROWN:
			case BLOCK_SHRUB:
			case BLOCK_STONE:
			case BLOCK_SUGAR_CANE:
			case BLOCK_WATER:
				result = type;
				break;
			case BLOCK_CACTUS:

				switch(face) {
					case BLOCK_FACE_BACK:
					case BLOCK_FACE_FRONT:
					case BLOCK_FACE_LEFT:
					case BLOCK_FACE_RIGHT:
						result = BLOCK_CACTUS_SIDE;
						break;
					case BLOCK_FACE_BOTTOM:
						result = BLOCK_CACTUS_BOTTOM;
						break;
					case BLOCK_FACE_TOP:
						result = BLOCK_CACTUS;
						break;
					default:
						THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_FACE_INVALID,
							"Face=%x", face);
				}
				break;
			case BLOCK_GRASS:

				switch(face) {
					case BLOCK_FACE_BACK:
					case BLOCK_FACE_FRONT:
					case BLOCK_FACE_LEFT:
					case BLOCK_FACE_RIGHT:
						result = BLOCK_DIRT_GRASS_SIDE;
						break;
					case BLOCK_FACE_BOTTOM:
						result = BLOCK_DIRT;
						break;
					case BLOCK_FACE_TOP:
						result = BLOCK_GRASS;
						break;
					default:
						THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_FACE_INVALID,
							"Face=%x", face);
				}
				break;
			case BLOCK_SANDSTONE:

				switch(face) {
					case BLOCK_FACE_BACK:
					case BLOCK_FACE_FRONT:
					case BLOCK_FACE_LEFT:
					case BLOCK_FACE_RIGHT:
						result = BLOCK_SANDSTONE_SIDE;
						break;
					case BLOCK_FACE_BOTTOM:
					case BLOCK_FACE_TOP:
						result = BLOCK_SANDSTONE;
						break;
					default:
						THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_FACE_INVALID,
							"Face=%x", face);
				}
				break;
			case BLOCK_SNOW:

				switch(face) {
					case BLOCK_FACE_BACK:
					case BLOCK_FACE_FRONT:
					case BLOCK_FACE_LEFT:
					case BLOCK_FACE_RIGHT:
						result = BLOCK_STONE_SNOW_SIDE;
						break;
					case BLOCK_FACE_BOTTOM:
						result = BLOCK_STONE;
						break;
					case BLOCK_FACE_TOP:
						result = BLOCK_SNOW;
						break;
					default:
						THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_FACE_INVALID,
							"Face=%x", face);
				}
				break;
			case BLOCK_WOOD_OAK:
				attributes |= BLOCK_ATTRIBUTE_ROTATABLE;
				result = nomic::utility::block_face_type_rotated(face, attributes, BLOCK_WOOD_OAK, BLOCK_WOOD_OAK,
					BLOCK_WOOD_OAK_SIDE);
				break;
			case BLOCK_WOOD_SPRUCE:
				attributes |= BLOCK_ATTRIBUTE_ROTATABLE;
				result = nomic::utility::block_face_type_rotated(face, attributes, BLOCK_WOOD_SPRUCE, BLOCK_WOOD_SPRUCE,
					BLOCK_WOOD_SPRUCE_SIDE);
				break;
			default:
				THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_TYPE_INVALID,
					"Type=%x", type);
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	uint8_t 
	utility::block_face_type_rotated(
		__in uint32_t face,
		__in uint8_t attributes,
		__in uint8_t type_top,
		__in uint8_t type_bottom,
		__in uint8_t type_side
		)
	{
		uint8_t result = type_top;
		bool back, front, left, right;

		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type={%x, %x, %x}, Face=%x, Attributes=%x", type_bottom, type_top, type_side,
			face, attributes);

		back = (attributes & BLOCK_ATTRIBUTE_ROTATED_BACK);
		front = (attributes & BLOCK_ATTRIBUTE_ROTATED_FRONT);
		left = (attributes & BLOCK_ATTRIBUTE_ROTATED_LEFT);
		right = (attributes & BLOCK_ATTRIBUTE_ROTATED_RIGHT);

		switch(face) {
			case BLOCK_FACE_BACK:

				if(back) {
					result = type_top;
				} else if(front) {
					result = type_bottom;
				} else {
					result = type_side;
				}
				break;
			case BLOCK_FACE_FRONT:

				if(back) {
					result = type_bottom;
				} else if(front) {
					result = type_top;
				} else {
					result = type_side;
				}
				break;
			case BLOCK_FACE_LEFT:

				if(left) {
					result = type_top;
				} else if(right) {
					result = type_bottom;
				} else {


					result = type_side;
				}
				break;
			case BLOCK_FACE_RIGHT:

				if(left) {
					result = type_bottom;
				} else if(right) {
					result = type_top;
				} else {
					result = type_side;
				}
				break;
			case BLOCK_FACE_BOTTOM:

				if(back || front || left || right) {
					result = type_side;
				} else {
					result = type_bottom;
				}
				break;
			case BLOCK_FACE_TOP:

				if(back || front || left || right) {
					result = type_side;
				} else {
					result = type_top;
				}
				break;
			default:
				THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_FACE_INVALID,
					"Face=%x", face);
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	bool 
	utility::block_passable(
		__in uint8_t type
		)
	{
		bool result = false;

		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

		switch(type) {
			case BLOCK_AIR:
			case BLOCK_CLOUD:
			case BLOCK_CORAL_BLUE:
			case BLOCK_CORAL_BROWN:
			case BLOCK_CORAL_ORANGE:
			case BLOCK_CORAL_PINK:
			case BLOCK_CORAL_PURPLE:
			case BLOCK_FLOWER_RED:
			case BLOCK_FLOWER_YELLOW:
			case BLOCK_GRASS_SHORT:
			case BLOCK_GRASS_TALL:
			case BLOCK_SEAGRASS_GREEN:
			case BLOCK_SEAGRASS_BROWN:
			case BLOCK_SHRUB:
			case BLOCK_SUGAR_CANE:
			case BLOCK_WATER:
				result = true;
				break;
			default:
				break;
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	bool 
	utility::block_selectable(
		__in uint8_t type
		)
	{
		bool result = true;

		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

		switch(type) {
			case BLOCK_AIR:
			case BLOCK_CLOUD:
			case BLOCK_WATER:
				result = false;
				break;
			default:
				break;
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	bool 
	utility::block_small(
		__in uint8_t type
		)
	{
		bool result = false;

		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

		switch(type) {
			case BLOCK_CACTUS:
				result = true;
				break;
			default:
				break;
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	bool 
	utility::block_transparent(
		__in uint8_t type
		)
	{
		bool result = false;

		TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x", type);

		switch(type) {
			case BLOCK_AIR:
			case BLOCK_CACTUS:
			case BLOCK_CLOUD:
			case BLOCK_CORAL_BLUE:
			case BLOCK_CORAL_BROWN:
			case BLOCK_CORAL_ORANGE:
			case BLOCK_CORAL_PINK:
			case BLOCK_CORAL_PURPLE:
			case BLOCK_FLOWER_RED:
			case BLOCK_FLOWER_YELLOW:
			case BLOCK_GLASS:
			case BLOCK_GRASS_SHORT:
			case BLOCK_GRASS_TALL:
			case BLOCK_LEAVES_OAK:
			case BLOCK_LEAVES_SPRUCE:
			case BLOCK_SEAGRASS_GREEN:
			case BLOCK_SEAGRASS_BROWN:
			case BLOCK_SHRUB:
			case BLOCK_SUGAR_CANE:
			case BLOCK_WATER:
				result = true;
				break;
			default:
				break;
		}

		TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
		return result;
	}

	std::string 
	utility::format_as_string(
		__in const char *format,
		...
		)
	{
		std::string result;

		if(format) {
			va_list arguments;

			va_start(arguments, format);
			int length = std::vsnprintf(nullptr, 0, format, arguments);
			va_end(arguments);

			if(length > 0) {
				result.resize(++length);
				va_start(arguments, format);
				length = std::vsnprintf(&result[0], result.size(), format, arguments);
				va_end(arguments);
			}

			if(length < 0) {
				result = NOMIC_UTILITY_EXCEPTION_STRING(NOMIC_UTILITY_EXCEPTION_FORMAT_MALFORMED);
			}
		}

		return result;
	}

	void 
	utility::position_as_block(
		__in const glm::vec3 &position,
		__inout glm::ivec2 &chunk,
		__inout glm::uvec3 &block
		)
	{
		block.x = std::floor(position.x);
		block.x %= CHUNK_WIDTH;
		block.y = std::floor(position.y);
		block.z = std::floor(position.z);
		block.z %= CHUNK_WIDTH;
		chunk.x = std::floor(std::fabs(position.x));
		chunk.x /= CHUNK_WIDTH;
		chunk.y = std::floor(std::fabs(position.z));
		chunk.y /= CHUNK_WIDTH;

		if(position.x < 0) {
			chunk.x *= -1;
			--chunk.x;
		}

		if(position.z < 0) {
			chunk.y *= -1;
			--chunk.y;
		}
	}

	std::string 
	utility::read_file(
		__in const std::string &path
		)
	{
		size_t length;
		std::string result;
		std::ifstream file;

		if(path.empty()) {
			THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_PATH_MALFORMED, "Path[%u]=%p", path.size(), &path[0]);
		}

		file = std::ifstream(path.c_str(), std::ios::in);
		if(!file) {
			THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_PATH_NOT_FOUND, "Path[%u]=%s", path.size(), STRING_CHECK(path));
		}

		file.seekg(0, std::ios::end);
		length = file.tellg();
		file.seekg(0, std::ios::beg);

		if(length) {
			result.resize(length, 0);
			file.read(&result[0], result.size());
		}

		file.close();

		return result;
	}

	time_t 
	utility::timestamp(void)
	{
		return std::time(nullptr);
	}

	std::string 
	utility::timestamp_as_string(
		__in const time_t &time
		)
	{
		size_t length;
		std::string result;

		result.resize(TIMESTAMP_LENGTH);

		length = std::strftime((char *) &result[0], TIMESTAMP_LENGTH, TIMESTAMP_FORMAT, std::localtime(&time));
		if(length) {
			result.resize(length);
		}

		return result;
	}
}
