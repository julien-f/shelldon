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

#ifndef ASSERT_H
#define ASSERT_H

#ifdef DISABLE_ASSERT

#	define assert(expr)

#else

#	include <stdio.h>

	/* From glib */
	/* Provide a string identifying the current function, non-concatenatable */
#	if defined (__GNUC__)
# 	 define G_STRFUNC     ((const char*) (__PRETTY_FUNCTION__))
#	elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 19901L
# 	 define G_STRFUNC     ((const char*) (__func__))
#	else
# 	 define G_STRFUNC     ((const char*) ("???"))
#	endif

#	define assert(expr) \
	{\
		if (!(expr)) \
		{\
			fprintf (stderr, "**\nAssertion failed (%s:%d:%s) %s\n", __FILE__, __LINE__, G_STRFUNC, #expr);\
			abort ();\
		}\
	}

#endif

#endif

