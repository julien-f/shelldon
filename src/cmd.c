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

#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "array.h"
#include "cmd.h"
#include "shell.h"
#include "tools.h"
#include "version.h"

extern char **environ;

int
cmd_cd (Shell *shell, void *args)
{
	char *new_pwd;
	if (!array_is_empty (args))
	{
		if (0 == strcmp ("-", array_get (args, 0)))
		{
			if ( (new_pwd = getenv ("OLDPWD")) )
			{
				new_pwd = strdup (new_pwd);
			}
			else
			{
				fprintf (stderr, "Failed to get previous directory.\n");
				return -1;
			}
		}
		else
		{
			new_pwd = strdup (array_get (args, 0));
		}
	}
	else if (!get_home_dir ())
	{
		fprintf (stderr, "Failed to get home directory.\n");
		return -1;
	}
	else
	{
		new_pwd = strdup (get_home_dir ());
	}

	char *old_pwd = get_cwd ();
	if (0 != chdir (new_pwd))
	{
		fprintf (stderr, "Failed to change directory to \"%s\".\n", new_pwd);
		return -1;
	}
	// Because new_cwd does not contain an absolute path.
	free (new_pwd);

	if (old_pwd)
	{
		setenv ("OLDPWD", old_pwd, 1);
		free (old_pwd);
	}
	else
	{
		unsetenv ("OLDPWD");
	}

	if ( (new_pwd = get_cwd ()) )
	{
		setenv ("PWD", new_pwd, 1);
		free (new_pwd);
	}
	else
	{
		unsetenv ("PWD");
	}

	return 0;
}

int
cmd_history (Shell *shell, void *args)
{
	if (array_is_empty (args))
	{
		fprintf (stderr, "The command history expects at least one argument.\n");
		return -1;
	}
	const char *opt = array_get (args, 0);
	if (0 == strcmp ("-c", opt))
	{
		clear_history ();
		return 0;
	}
	return -1;
}

int
cmd_exec (Shell *shell, void *args)
{
	if (array_is_empty (args))
	{
		fprintf (stderr, "The command exec expects at least one argument.\n");
		return -1;
	}

	execute (array_get (args, 0), args, EXEC_REPLACE, NULL);
	error (0, errno, "Error");
	return -1;
}

int
cmd_execbg (Shell *shell, void *args)
{
	if (array_is_empty (args))
	{
		fprintf (stderr, "The command execbg expects at least one argument.\n");
		return -1;
	}

	return execute (array_get (args, 0), args, EXEC_BG, NULL);
}

int
cmd_execfg (Shell *shell, void *args)
{
	if (array_is_empty (args))
	{
		fprintf (stderr, "The command execfg expects at least one argument.\n");
		return -1;
	}

	int status;
	if (-1 == execute (array_get (args, 0), args, EXEC_FG, &status))
	{
		fprintf (stderr, "fork () failed.\n");
		return -1;
	}
	return status;
}

int
cmd_exit (Shell *shell, void *args)
{
	shell_stop (shell);
	return 0;
}

int
cmd_help (Shell *shell, void *args)
{
	if (array_is_empty (args))
	{
		printf ("Available commands:\n");

		const Array *a = shell_get_commands (shell);
		for (size_t i = 0, n = array_get_length (a); i < n; ++i)
		{
			const command_t *command = array_get (a, i);
			printf ("  %s\n", command->name);

		}
		return 0;
	}

	int return_value = 0;
	for (size_t i = 0, n = array_get_length (args); i < n; ++i)
	{
		const char *name = array_get (args, i);
		const command_t *p = shell_get_command (shell, name);
		if (!p) // Command not found.
		{
			fprintf (stderr, "No command \"%s\" found.\n", name);
			--return_value;
		}
		else
		{
			printf ("%s: %s", name, name);
			if (p->args_list)
			{
				printf (" %s", p->args_list);
			}
			printf ("\n  ");
			if (p->help)
			{
				printf ("%s", p->help);
			}
			else
			{
				printf ("%s", "No help available for this command.");
			}
			printf ("\n");
		}
	}
	return return_value;
}

int
cmd_pwd (Shell *shell, void *args)
{
	char *cwd = get_cwd ();
	if (!cwd)
	{
		fprintf (stderr, "Failed to get current working directory.\n");
		return -1;
	}
	printf ("%s\n", cwd);
	free (cwd);
	return 0;
}

int
cmd_setenv (Shell *shell, void *args)
{
	if (array_is_empty (args))
	{
		char **p = environ;
		while (*p)
		{
			printf ("%s\n", *p);
			++p;
		}
	}
	else
	{
		for (size_t i = 0, n = array_get_length (args); i < n; ++i)
		{
			putenv (strdup (array_get (args, i)));
		}
	}

	return 0;
}

int
cmd_version (Shell *shell, void *args)
{
	if (!array_is_empty (args))
	{
		const char *arg = array_get (args, 0);
		if (0 == strcmp ("-v", arg))
		{
			printf ("%s\n", get_prog_version ());
			return 0;
		}
		else if (0 == strcmp ("-n", arg))
		{
			printf ("%s\n", get_prog_version_name ());
			return 0;
		}
	}
	print_version ();
	return 0;
}

int
cmd_sdc (Shell *s, void *args)
{
	if (array_is_empty (args))
	{
		fprintf (stderr, "The command sdc expects at least one argument.\n");
		return -1;
	}

	shell_set_default_command (s, array_get (args, 0));
	return 0;
}

