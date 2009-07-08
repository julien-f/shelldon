/**
 * This file is a part of Shelldon.
 *
 * Shelldon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Shelldon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Shelldon.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef DEBUG_H
#define DEBUG_H

#ifdef DISABLE_DEBUG

#	define debug(...)

#else

#	include <stdio.h>

#	define debug(...) \
	{\
		printf ("** %s:%d: ", __FILE__, __LINE__);\
		printf (__VA_ARGS__);\
		printf ("\n");\
	}

#endif

#endif

