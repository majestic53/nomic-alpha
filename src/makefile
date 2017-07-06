# Nomic
# Copyright (C) 2017 David Jolly
#
# Nomic is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Nomic is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -std=gnu++11 -Wall -Werror
CC_FLAGS_EXT=`pkg-config --cflags freetype2`
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./../include/
DIR_INC_CORE=./../include/core/
DIR_INC_ENTITY=./../include/entity/
DIR_INC_EVENT=./../include/event/
DIR_INC_FONT=./../include/font/
DIR_INC_GRAPHIC=./../include/graphic/
DIR_INC_RENDER=./../include/render/
DIR_INC_SESSION=./../include/session/
DIR_INC_TERRAIN=./../include/terrain/
DIR_INC_UUID=./../include/uuid/
DIR_SRC=./
DIR_SRC_CORE=./core/
DIR_SRC_ENTITY=./entity/
DIR_SRC_EVENT=./event/
DIR_SRC_FONT=./font/
DIR_SRC_GRAPHIC=./graphic/
DIR_SRC_RENDER=./render/
DIR_SRC_SESSION=./session/
DIR_SRC_TERRAIN=./terrain/
DIR_SRC_UUID=./uuid/
LIB=libnomic.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)base_exception.o $(DIR_BUILD)base_runtime.o $(DIR_BUILD)base_trace.o $(DIR_BUILD)base_utility.o \
		$(DIR_BUILD)core_entity.o $(DIR_BUILD)core_event.o $(DIR_BUILD)core_font.o $(DIR_BUILD)core_id.o $(DIR_BUILD)core_noise.o \
			$(DIR_BUILD)core_object.o $(DIR_BUILD)core_primitive.o $(DIR_BUILD)core_renderer.o $(DIR_BUILD)core_signal.o \
			$(DIR_BUILD)core_thread.o $(DIR_BUILD)core_transform.o $(DIR_BUILD)core_uniform.o \
		$(DIR_BUILD)entity_axis.o $(DIR_BUILD)entity_block.o $(DIR_BUILD)entity_camera.o $(DIR_BUILD)entity_chunk.o \
			$(DIR_BUILD)entity_diagnostic.o $(DIR_BUILD)entity_manager.o $(DIR_BUILD)entity_message.o $(DIR_BUILD)entity_object.o \
			$(DIR_BUILD)entity_plain.o $(DIR_BUILD)entity_reticle.o $(DIR_BUILD)entity_skybox.o $(DIR_BUILD)entity_string.o \
		$(DIR_BUILD)event_input.o $(DIR_BUILD)event_manager.o $(DIR_BUILD)event_queue.o \
		$(DIR_BUILD)font_manager.o \
		$(DIR_BUILD)graphic_bitmap.o $(DIR_BUILD)graphic_character.o $(DIR_BUILD)graphic_cubemap.o $(DIR_BUILD)graphic_display.o \
			$(DIR_BUILD)graphic_manager.o $(DIR_BUILD)graphic_program.o $(DIR_BUILD)graphic_shader.o $(DIR_BUILD)graphic_texture.o \
			$(DIR_BUILD)graphic_vao.o $(DIR_BUILD)graphic_vbo.o \
		$(DIR_BUILD)render_manager.o \
		$(DIR_BUILD)session_manager.o \
		$(DIR_BUILD)terrain_chunk.o $(DIR_BUILD)terrain_generator.o $(DIR_BUILD)terrain_manager.o \
		$(DIR_BUILD)uuid_manager.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_base build_core build_entity build_event build_font build_graphic build_render build_session build_terrain build_uuid

### BASE ###

build_base: base_exception.o base_runtime.o base_trace.o base_utility.o

base_exception.o: $(DIR_SRC)exception.cpp $(DIR_INC)exception.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)exception.cpp -o $(DIR_BUILD)base_exception.o

base_runtime.o: $(DIR_SRC)runtime.cpp $(DIR_INC)runtime.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)runtime.cpp -o $(DIR_BUILD)base_runtime.o

base_trace.o: $(DIR_SRC)trace.cpp
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)trace.cpp -o $(DIR_BUILD)base_trace.o

base_utility.o: $(DIR_SRC)utility.cpp
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)utility.cpp -o $(DIR_BUILD)base_utility.o

### CORE ###

build_core: core_entity.o core_event.o core_font.o core_id.o core_noise.o core_object.o core_primitive.o core_renderer.o core_signal.o \
	core_thread.o core_transform.o core_uniform.o

core_entity.o: $(DIR_SRC_CORE)entity.cpp $(DIR_INC_CORE)entity.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)entity.cpp -o $(DIR_BUILD)core_entity.o

core_event.o: $(DIR_SRC_CORE)event.cpp $(DIR_INC_CORE)event.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)event.cpp -o $(DIR_BUILD)core_event.o

core_font.o: $(DIR_SRC_CORE)font.cpp $(DIR_INC_CORE)font.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)font.cpp -o $(DIR_BUILD)core_font.o

core_id.o: $(DIR_SRC_CORE)id.cpp $(DIR_INC_CORE)id.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)id.cpp -o $(DIR_BUILD)core_id.o

core_noise.o: $(DIR_SRC_CORE)noise.cpp $(DIR_INC_CORE)noise.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)noise.cpp -o $(DIR_BUILD)core_noise.o

core_object.o: $(DIR_SRC_CORE)object.cpp $(DIR_INC_CORE)object.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)object.cpp -o $(DIR_BUILD)core_object.o

core_primitive.o: $(DIR_SRC_CORE)primitive.cpp $(DIR_INC_CORE)primitive.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)primitive.cpp -o $(DIR_BUILD)core_primitive.o

core_renderer.o: $(DIR_SRC_CORE)renderer.cpp $(DIR_INC_CORE)renderer.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)renderer.cpp -o $(DIR_BUILD)core_renderer.o

core_signal.o: $(DIR_SRC_CORE)signal.cpp $(DIR_INC_CORE)signal.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)signal.cpp -o $(DIR_BUILD)core_signal.o

core_thread.o: $(DIR_SRC_CORE)thread.cpp $(DIR_INC_CORE)thread.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)thread.cpp -o $(DIR_BUILD)core_thread.o

core_transform.o: $(DIR_SRC_CORE)transform.cpp $(DIR_INC_CORE)transform.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)transform.cpp -o $(DIR_BUILD)core_transform.o

core_uniform.o: $(DIR_SRC_CORE)uniform.cpp $(DIR_INC_CORE)uniform.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)uniform.cpp -o $(DIR_BUILD)core_uniform.o

### ENTITY ###

build_entity: entity_axis.o entity_block.o entity_camera.o entity_chunk.o entity_diagnostic.o entity_manager.o entity_message.o entity_object.o \
	entity_plain.o entity_reticle.o entity_skybox.o entity_string.o

entity_axis.o: $(DIR_SRC_ENTITY)axis.cpp $(DIR_INC_ENTITY)axis.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)axis.cpp -o $(DIR_BUILD)entity_axis.o

entity_block.o: $(DIR_SRC_ENTITY)block.cpp $(DIR_INC_ENTITY)block.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)block.cpp -o $(DIR_BUILD)entity_block.o

entity_camera.o: $(DIR_SRC_ENTITY)camera.cpp $(DIR_INC_ENTITY)camera.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)camera.cpp -o $(DIR_BUILD)entity_camera.o

entity_chunk.o: $(DIR_SRC_ENTITY)chunk.cpp $(DIR_INC_ENTITY)chunk.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)chunk.cpp -o $(DIR_BUILD)entity_chunk.o

entity_diagnostic.o: $(DIR_SRC_ENTITY)diagnostic.cpp $(DIR_INC_ENTITY)diagnostic.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)diagnostic.cpp -o $(DIR_BUILD)entity_diagnostic.o

entity_manager.o: $(DIR_SRC_ENTITY)manager.cpp $(DIR_INC_ENTITY)manager.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)manager.cpp -o $(DIR_BUILD)entity_manager.o

entity_message.o: $(DIR_SRC_ENTITY)message.cpp $(DIR_INC_ENTITY)message.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)message.cpp -o $(DIR_BUILD)entity_message.o

entity_object.o: $(DIR_SRC_ENTITY)object.cpp $(DIR_INC_ENTITY)object.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)object.cpp -o $(DIR_BUILD)entity_object.o

entity_plain.o: $(DIR_SRC_ENTITY)plain.cpp $(DIR_INC_ENTITY)plain.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)plain.cpp -o $(DIR_BUILD)entity_plain.o

entity_reticle.o: $(DIR_SRC_ENTITY)reticle.cpp $(DIR_INC_ENTITY)reticle.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)reticle.cpp -o $(DIR_BUILD)entity_reticle.o

entity_skybox.o: $(DIR_SRC_ENTITY)skybox.cpp $(DIR_INC_ENTITY)skybox.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)skybox.cpp -o $(DIR_BUILD)entity_skybox.o

entity_string.o: $(DIR_SRC_ENTITY)string.cpp $(DIR_INC_ENTITY)string.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ENTITY)string.cpp -o $(DIR_BUILD)entity_string.o

### EVENT ###

build_event: event_input.o event_manager.o event_queue.o

event_input.o: $(DIR_SRC_EVENT)input.cpp $(DIR_INC_EVENT)input.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EVENT)input.cpp -o $(DIR_BUILD)event_input.o

event_manager.o: $(DIR_SRC_EVENT)manager.cpp $(DIR_INC_EVENT)manager.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EVENT)manager.cpp -o $(DIR_BUILD)event_manager.o

event_queue.o: $(DIR_SRC_EVENT)queue.cpp $(DIR_INC_EVENT)queue.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EVENT)queue.cpp -o $(DIR_BUILD)event_queue.o

### FONT ###

build_font: font_manager.o

font_manager.o: $(DIR_SRC_FONT)manager.cpp $(DIR_INC_FONT)manager.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_FONT)manager.cpp -o $(DIR_BUILD)font_manager.o

### GRAPHIC ###

build_graphic: graphic_bitmap.o graphic_character.o graphic_cubemap.o graphic_display.o graphic_manager.o graphic_program.o graphic_shader.o \
	graphic_texture.o graphic_vao.o graphic_vbo.o

graphic_bitmap.o: $(DIR_SRC_GRAPHIC)bitmap.cpp $(DIR_INC_GRAPHIC)bitmap.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)bitmap.cpp -o $(DIR_BUILD)graphic_bitmap.o

graphic_character.o: $(DIR_SRC_GRAPHIC)character.cpp $(DIR_INC_GRAPHIC)character.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)character.cpp -o $(DIR_BUILD)graphic_character.o

graphic_cubemap.o: $(DIR_SRC_GRAPHIC)cubemap.cpp $(DIR_INC_GRAPHIC)cubemap.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)cubemap.cpp -o $(DIR_BUILD)graphic_cubemap.o

graphic_display.o: $(DIR_SRC_GRAPHIC)display.cpp $(DIR_INC_GRAPHIC)display.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)display.cpp -o $(DIR_BUILD)graphic_display.o

graphic_manager.o: $(DIR_SRC_GRAPHIC)manager.cpp $(DIR_INC_GRAPHIC)manager.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)manager.cpp -o $(DIR_BUILD)graphic_manager.o

graphic_program.o: $(DIR_SRC_GRAPHIC)program.cpp $(DIR_INC_GRAPHIC)program.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)program.cpp -o $(DIR_BUILD)graphic_program.o

graphic_shader.o: $(DIR_SRC_GRAPHIC)shader.cpp $(DIR_INC_GRAPHIC)shader.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)shader.cpp -o $(DIR_BUILD)graphic_shader.o

graphic_texture.o: $(DIR_SRC_GRAPHIC)texture.cpp $(DIR_INC_GRAPHIC)texture.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)texture.cpp -o $(DIR_BUILD)graphic_texture.o

graphic_vao.o: $(DIR_SRC_GRAPHIC)vao.cpp $(DIR_INC_GRAPHIC)vao.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)vao.cpp -o $(DIR_BUILD)graphic_vao.o

graphic_vbo.o: $(DIR_SRC_GRAPHIC)vbo.cpp $(DIR_INC_GRAPHIC)vbo.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_GRAPHIC)vbo.cpp -o $(DIR_BUILD)graphic_vbo.o

### RENDER ###

build_render: render_manager.o

render_manager.o: $(DIR_SRC_RENDER)manager.cpp $(DIR_INC_RENDER)manager.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_RENDER)manager.cpp -o $(DIR_BUILD)render_manager.o

### SESSION ###

build_session: session_manager.o

session_manager.o: $(DIR_SRC_SESSION)manager.cpp $(DIR_INC_SESSION)manager.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_SESSION)manager.cpp -o $(DIR_BUILD)session_manager.o

### TERRAIN ###

build_terrain: terrain_chunk.o terrain_generator.o terrain_manager.o

terrain_chunk.o: $(DIR_SRC_TERRAIN)chunk.cpp $(DIR_INC_TERRAIN)chunk.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_TERRAIN)chunk.cpp -o $(DIR_BUILD)terrain_chunk.o

terrain_generator.o: $(DIR_SRC_TERRAIN)generator.cpp $(DIR_INC_TERRAIN)generator.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_TERRAIN)generator.cpp -o $(DIR_BUILD)terrain_generator.o

terrain_manager.o: $(DIR_SRC_TERRAIN)manager.cpp $(DIR_INC_TERRAIN)manager.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_TERRAIN)manager.cpp -o $(DIR_BUILD)terrain_manager.o

### UUID ###

build_uuid: uuid_manager.o

uuid_manager.o: $(DIR_SRC_UUID)manager.cpp $(DIR_INC_UUID)manager.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_UUID)manager.cpp -o $(DIR_BUILD)uuid_manager.o
