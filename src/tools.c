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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>

#include "tools.h"

#include "array.h"
#include "string.h"

static void cleaner (int i, void *ptr)
{
	free (ptr);
}

static const struct passwd *
get_passwd_info (void)
{
	static struct passwd *passwd = NULL;
	if (!passwd)
	{
		const char *logname = getenv ("LOGNAME");
		if (logname)
		{
			passwd = getpwnam (logname);
		}
		else
		{
			passwd = getpwuid (getuid ());
		}
	}
	return passwd;
}

pid_t
execute (const char *file, void **args, exec_mode mode, int *status)
{
	pid_t pid;
	if (EXEC_REPLACE != mode)
	{
		pid = fork ();
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
		void *arr = array_get_array (args, true);
		execvp (file, arr);
		free (arr); // If there was an error, we must free arr.
		if (EXEC_REPLACE == mode)
		{
			return -1;
		}
		error (EXIT_FAILURE, errno, "Error");
	}
	if (EXEC_BG == mode) // The program is run in bakground.
	{
		return pid;
	}
	waitpid (pid, status, 0);
	return pid;
}

size_t
get_args_lg (const char *const *args)
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

const char *
get_config_dir (void)
{
	static char *config_dir = NULL;
	if (!config_dir)
	{
		config_dir = getenv ("XDG_CONFIG_HOME");
		if (config_dir && '\0' != *config_dir)
		{
			config_dir = strdup (config_dir);
		}
		else
		{
			config_dir = string_concat (NULL, get_home_dir (), "/.config", NULL);
		}
		if (-1 == mkdir (config_dir, 0777) && EEXIST != errno)
		{
			free (config_dir);
			config_dir = NULL;
		}
		on_exit (cleaner, config_dir);
	}
	return config_dir;
}

const char *
get_home_dir (void)
{
	static char *home_dir = NULL;
	if (!home_dir)
	{
		const struct passwd *passwd = get_passwd_info ();
		if (passwd && passwd->pw_dir)
		{
			home_dir = strdup (passwd->pw_dir);
		}
		else if ( (home_dir = getenv ("HOME")) )
		{
			home_dir = strdup (home_dir);
		}
		else
		{
			home_dir = strdup (get_tmp_dir ());
		}
		on_exit (cleaner, home_dir);
	}
	return home_dir;
}

const char *
get_user_name (void)
{
	const struct passwd *passwd = get_passwd_info ();
	if (!passwd)
	{
		return NULL;
	}
	return passwd->pw_name;
}

const char *
get_real_name (void)
{
	char *real_name = NULL;
	if (!real_name)
	{
		const struct passwd *passwd = get_passwd_info ();
		if (!passwd)
		{
			return NULL;
		}

		// Make a copy of passwd->pw_gecos until the first ",".
		char *p = strchr (passwd->pw_gecos, ',');
		if (p)
		{
			real_name = strndup (passwd->pw_gecos, (size_t) (p - passwd->pw_gecos));
		}
		else // No "," found, duplicates the whole string.
		{
			real_name = strdup (passwd->pw_gecos);
		}
		on_exit (cleaner, real_name);
	}
	return real_name;
}

const char *
get_tmp_dir (void)
{
	static char *tmp_dir = NULL;
	if (!tmp_dir)
	{
		if ( (tmp_dir = getenv ("TMPDIR")) )
		{
			tmp_dir = strdup (tmp_dir);
		}
		else if ( (tmp_dir = getenv ("TMP")) )
		{
			tmp_dir = strdup (tmp_dir);
		}
		else if ( (tmp_dir = getenv ("TEMP")) )
		{
			tmp_dir = strdup (tmp_dir);
		}
		else
		{
			tmp_dir = strdup ("/tmp");
		}
		on_exit (cleaner, tmp_dir);
	}
	return tmp_dir;
}

#if !(_GNU_SOURCE || _POSIX_C_SOURCE >= 200809L)

char *
strndup (const char *s, size_t n)
{
	if (!s)
	{
		return NULL;
	}
	{
		size_t len = strlen (s);
		if (len < n)
		{
			n = len;
		}
	}
	char *r = (char *) malloc (n + 1);
	if (r)
	{
		r = strncpy (r, s, n);
		r[n] = '\0';
	}
	return r;
}

#endif

#ifndef _GNU_SOURCE

char *
get_cwd (void)
{
	char *cwd = NULL;
	char *buffer = NULL;
	for (size_t size = 128; (buffer = (char *) realloc ( (void *) buffer, size))
			&& ! (cwd = getcwd (buffer, size)) && ERANGE == errno; size <<= 1);
	if (cwd)
	{
		return (char *) realloc ( (void *) cwd, strlen (cwd) + 1);
	}
	if (buffer)
	{
		error (0, 0, "Error");
		free (buffer);
	}
	return NULL;
}

#endif

