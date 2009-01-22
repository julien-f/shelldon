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

#include <pwd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "tools.h"

static const struct passwd *
get_passwd_info()
{
	static struct passwd *passwd = NULL;
	if (!passwd)
	{
		const char *logname = getenv("LOGNAME");
		if (logname)
		{
			passwd = getpwnam(logname);
		}
		else
		{
			passwd = getpwuid(getuid());
		}
	}
	return passwd;
}

size_t
get_args_lg(char *const *args)
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

char *
strcat2(char *dest, ...)
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

const char *
get_home_dir()
{
	static char *home_dir = NULL;
	if (!home_dir)
	{
		const struct passwd *passwd = get_passwd_info();
		if (passwd && passwd->pw_dir)
		{
			home_dir = passwd->pw_dir;
		}
		else if ( (home_dir = getenv("HOME")) )
		{
			home_dir = strdup(home_dir);
		}
		else
		{
			home_dir = "/tmp";
		}
	}
	return home_dir;
}

const char *
get_user_name()
{
	const struct passwd *passwd = get_passwd_info();
	if (!passwd)
	{
		return NULL;
	}
	return passwd->pw_name;
}

const char *
get_real_name()
{
	char *real_name = NULL;
	if (!real_name)
	{
		const struct passwd *passwd = get_passwd_info();
		if (!passwd)
		{
			return NULL;
		}
	/*		char *p = index(passwd->pw_gecos, ',');*/
	/*		size_t length = (size_t) (p - passwd_gecos);*/
	/*		real_name = (char *) malloc((size_t) (p - passwd->pw_gecos));*/
	/*		strcncpy*/
		char *copy = strdup(passwd->pw_gecos);
		real_name = strdup(strtok(copy, ","));
		free(copy);
	}
	return real_name;
}

