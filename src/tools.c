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

#include <errno.h>
#include <error.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>

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

int
execute(const char *file, const char *const *args, exec_mode mode, int *status)
{
	pid_t pid;
	if (EXEC_REPLACE != mode)
	{
		pid = fork();
		if (-1 == pid) // The fork failed.
		{
			return -1;
		}
	}
	else
	{
		pid = 0;
	}
	if (!pid) // If we are in the child or the EXEC_REPLACE is active.
	{
		// We don't care if exec changes args because this program will be
		// destroyed.
		execvp(file, (char *const *) args);
		error(EXIT_FAILURE, errno, "Error");
	}
	if (EXEC_BG == mode) // The program is run in bakground.
	{
		return 0;
	}
	waitpid(pid, status, 0);
	return 0;
}

size_t
get_args_lg(const char *const *args)
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

	if (!dest)
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
	while ( (string = va_arg(strings, char *)) )
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
	if (!dest)
	{
		return NULL;
	}

	// For an efficient copy.
	char *p = dest + dest_length; // <=> &dest[dest_length]

	// Copies each string.
	va_start(strings, dest);
	while ( (string = va_arg(strings, char *)) )
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
			home_dir = strdup(passwd->pw_dir);
		}
		else if ( (home_dir = getenv("HOME")) )
		{
			home_dir = strdup(home_dir);
		}
		else
		{
			home_dir = strdup(get_tmp_dir());
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

		// Make a copy of passwd->pw_gecos until the first ",".
		char *p = strchr(passwd->pw_gecos, ',');
		if (p)
		{
			real_name = strndup(passwd->pw_gecos, (size_t) (p - passwd->pw_gecos));
		}
		else // No "," found, duplicates the whole string.
		{
			real_name = strdup(passwd->pw_gecos);
		}
	}
	return real_name;
}

const char *
get_tmp_dir()
{
	static char *tmp_dir = NULL;
	if (!tmp_dir)
	{
		if ( (tmp_dir = getenv("TMPDIR")) )
		{
			tmp_dir = strdup(tmp_dir);
		}
		else if ( (tmp_dir = getenv("TMP")) )
		{
			tmp_dir = strdup(tmp_dir);
		}
		else if ( (tmp_dir = getenv("TEMP")) )
		{
			tmp_dir = strdup(tmp_dir);
		}
		else
		{
			tmp_dir = strdup("/tmp");
		}
	}
	return tmp_dir;
}

#ifndef _GNU_SOURCE

char *
strndup(const char *s, size_t n)
{
	if (!s)
	{
		return NULL;
	}
	{
		size_t len = strlen(s);
		if (len < n)
		{
			n = len;
		}
	}
	char *r = (char *) malloc(n + 1);
	if (r)
	{
		r = strncpy(r, s, n);
		r[n] = '\0';
	}
	return r;
}

char *
get_cwd()
{
	char *cwd = NULL;
	char *buffer = NULL;
	for (size_t size = 128; (buffer = (char *) realloc((void *) buffer, size))
			&& !(cwd = getcwd(buffer, size)) && ERANGE == errno; size <<= 1);
	if (cwd)
	{
		return (char *) realloc((void *) cwd, strlen(cwd) + 1);
	}
	if (buffer)
	{
		error (0, 0, "Error");
		free(buffer);
	}
	return NULL;
} 

#endif

