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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"

size_t get_args_lg(char *const *args)
{
	if (NULL == args)
	{
		return 0;
	}

	size_t lg = 0;
	while (NULL != args[lg])
	{
		++lg;
	}
	return lg;
}

char *strcat2(char *dest, ...)
{
	va_list strings;
	char *string;
	size_t dest_length;
	size_t length;

	if (NULL == dest)
	{
		dest_length = 0;
	}
	else
	{
		dest_length = strlen(dest);
	}

	// Computes the length of the final string.
	length = dest_length;
	va_start(strings, dest);
	while (NULL != (string = va_arg(strings, char *)))
	{
		length += strlen(string);
	}
	va_end(strings);

	// Nothing to concatenate, we can stop here.
	if (dest_length == length)
	{
		return dest;
	}

	// Resizes the memory space pointed by dest to ensure it will fit.
	dest = (char *) realloc((void *) dest, length + 1);

	// The reallocation failed, so stops.
	if (NULL == dest)
	{
		return NULL;
	}

	// For an efficient copy.
	char *p = dest + dest_length; // <=> &dest[dest_length]

	// Copies each string.
	va_start(strings, dest);
	while (NULL != (string = va_arg(strings, char *)))
	{
		while ('\0' != (*p = *string))
		{
			++p;
			++string;
		}
	}
	va_end(strings);

	return dest;
}

