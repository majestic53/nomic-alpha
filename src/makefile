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
CC_FLAGS_EXT=
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./../include/
DIR_INC_CORE=./../include/core/
DIR_INC_UUID=./../include/uuid/
DIR_SRC=./
DIR_SRC_CORE=./core/
DIR_SRC_UUID=./uuid/
LIB=libnomic.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)base_exception.o $(DIR_BUILD)base_runtime.o $(DIR_BUILD)base_trace.o $(DIR_BUILD)base_utility.o \
		$(DIR_BUILD)core_object.o $(DIR_BUILD)core_signal.o $(DIR_BUILD)core_thread.o \
		$(DIR_BUILD)uuid_id.o $(DIR_BUILD)uuid_manager.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_base build_core build_uuid

### BASE ###

build_base: base_exception.o base_runtime.o base_trace.o base_utility.o

base_exception.o: $(DIR_SRC)exception.cpp $(DIR_INC)exception.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)exception.cpp -o $(DIR_BUILD)base_exception.o

base_runtime.o: $(DIR_SRC)runtime.cpp $(DIR_INC)runtime.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)runtime.cpp -o $(DIR_BUILD)base_runtime.o

base_trace.o: $(DIR_SRC)trace.cpp
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)trace.cpp -o $(DIR_BUILD)base_trace.o

base_utility.o: $(DIR_SRC)utility.cpp
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)utility.cpp -o $(DIR_BUILD)base_utility.o

### CORE ###

build_core: core_object.o core_signal.o core_thread.o

core_object.o: $(DIR_SRC_CORE)object.cpp $(DIR_INC_CORE)object.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)object.cpp -o $(DIR_BUILD)core_object.o

core_signal.o: $(DIR_SRC_CORE)signal.cpp $(DIR_INC_CORE)signal.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)signal.cpp -o $(DIR_BUILD)core_signal.o

core_thread.o: $(DIR_SRC_CORE)thread.cpp $(DIR_INC_CORE)thread.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)thread.cpp -o $(DIR_BUILD)core_thread.o

### UUID ###

build_uuid: uuid_id.o uuid_manager.o

uuid_id.o: $(DIR_SRC_UUID)id.cpp $(DIR_INC_UUID)id.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_UUID)id.cpp -o $(DIR_BUILD)uuid_id.o

uuid_manager.o: $(DIR_SRC_UUID)manager.cpp $(DIR_INC_UUID)manager.h
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_UUID)manager.cpp -o $(DIR_BUILD)uuid_manager.o
