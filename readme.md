Nomic
=====

A small graphics framework, written in C++

Table of Contents
=================

1. [Build](https://github.com/majestic53/nomic-alpha#build) -- How-to build the project
2. [Changelog](https://github.com/majestic53/nomic-alpha#changelog) -- Project changelist information
3. [License](https://github.com/majestic53/nomic-alpha#license) -- Project license information

Build
-----

__NOTE__: Tested with the Clang C++ compilers. Modification to the makefiles might be required to build with a different compiler.

To build the entire project, first install the required libraries:

* libSDL2
* libGLEW
* libFreeType

Next, run the following command from the projects root directory:

```
make release
```

Changelog
=========

Version 0.1.1732
----------------
*Updated: 8/12/2017*

* Added basic collision detection

*Updated: 8/10/2017*

* Added basic player physics

Version 0.1.1731
----------------
*Updated: 8/6/2017*

* Added shadowmap class (incomplete)

*Updated: 8/5/2017*

* Added basic lighting
* Added sun-rise/-set
* Code cleanup/bug fixes

*Updated: 8/1-3/2017*

* Added sun entity class <s>(incomplete)</s>

*Updated: 7/31/2017*

* Added textured blocks to block gui

*Changelog truncated (see file history for full log)*

License
=======

Copyright(C) 2017 David Jolly <majestic53@gmail.com>

Nomic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Nomic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
