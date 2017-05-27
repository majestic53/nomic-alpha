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

#include "../include/runtime.h"

int 
main(
	__in int argc,
	__in const char *argv[]
	)
{
	int result = EXIT_SUCCESS;

	try {
		nomic::runtime &instance = nomic::runtime::acquire();
		TRACE_MESSAGE_FORMAT(nomic::LEVEL_INFORMATION, NOMIC " %s", STRING_CHECK(nomic::runtime::version(true)));
		TRACE_MESSAGE(nomic::LEVEL_INFORMATION, NOMIC_COPYRIGHT);
		instance.initialize();
		instance.run(argc, argv);
		instance.wait();
		instance.terminate();
		instance.uninitialize();
		instance.release();
	} catch(nomic::exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = EXIT_FAILURE;
	} catch(std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		result = EXIT_FAILURE;
	}

	return result;
}
