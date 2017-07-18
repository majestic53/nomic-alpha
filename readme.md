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

To build the entire project, run the following command from the projects root directory:

```
make release
```

Changelog
=========

Version 0.1.1729
----------------

*Updated: 7/17-18/2017*

* Added underwater detection to shader
* Fixed coordinate system

Version 0.1.1728
----------------
*Updated: 7/15/2017*

* Integrated textured into renderer

*Updated: 7/12-13/2017*

* Added texture atlas class <s>(incomplete)</s>

*Updated: 7/11/2017*

* Added fog to chunk shader
* Fixed chunk generation threading issue

Version 0.1.1727
----------------
*Updated: 7/9/2017*

* Added infinite terrain generation

*Updated: 7/6/2017*

* Added chunk manager class
* Tied camera to update tick

*Updated: 7/5/2017*

* Implemented chunk joining

*Updated: 7/3/2017*

* Added loading screen

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
