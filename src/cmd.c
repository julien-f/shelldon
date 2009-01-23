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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmd.h"
#include "tools.h"
#include "version.h"

// Defined in "main.c"
extern const cmd cmd_list[];

extern char **environ;
extern int putenv(char *string);

int
cmd_cd(char *const *args)
{
	char *new_pwd;
	if (args && *args)
	{
		if (0 == strcmp("-", *args))
		{
			if ( (new_pwd = getenv("OLDPWD")) )
			{
				new_pwd = strdup(new_pwd);
			}
			else
			{
				fprintf(stderr, "Failed to get previous directory.\n");
				return -1;
			}
		}
		else
		{
			new_pwd = strdup(*args);
		}
	}
	else if (!get_home_dir())
	{
		fprintf(stderr, "Failed to get home directory.\n");
		return -1;
	}
	else
	{
		new_pwd = strdup(get_home_dir());
	}

	char *old_pwd = get_cwd();
	if (0 != chdir(new_pwd))
	{
		fprintf(stderr, "Failed to change directory to \"%s\".\n", new_pwd);
		return -1;
	}
	// Because new_cwd does not contain an absolute path.
	free(new_pwd);

	if (old_pwd)
	{
		setenv("OLDPWD", old_pwd, 1);
		free(old_pwd);
	}
	else
	{
		unsetenv("OLDPWD");
	}

	if ( (new_pwd = get_cwd()) )
	{
		setenv("PWD", new_pwd, 1);
		free(new_pwd);
	}
	else
	{
		unsetenv("PWD");
	}

	return 0;
}

int
cmd_exec(char *const *args)
{
	if (!args || !*args) // No args.
	{
		fprintf(stderr, "The command exec expects at least one argument.\n");
		return -1;
	}
	
	execvp(args[0], args);
	fprintf(stderr, "Error during exec.\n");
	return -1;
}

int
cmd_exit(char *const *args)
{
	exit(EXIT_SUCCESS);
}

int
cmd_help(char *const *args)
{
	if (!args || !*args)
	{
		printf("Available commands:\n");

		const cmd *p = cmd_list;
		while (p->cmd)
		{
			printf(" %s\n", p->cmd);
			++p;
		}
	}
	else
	{
		const cmd *p = cmd_list;
		while (p->cmd && 0 != strcmp(args[0], p->cmd))
		{
			++p;
		}
		if (!p->cmd) // Command not found.
		{
			return -1;
		}
		printf("%s\n", p->help);
	}
	return 0;
}

int
cmd_pwd(char *const *args)
{
	char *cwd = get_cwd();
	if (!cwd)
	{
		fprintf(stderr, "Failed to get current working directory.\n");
		return -1;
	}
	printf("%s\n", cwd);
	free(cwd);
	return 0;
}

int
cmd_setenv(char *const *args)
{
	if (!args || !*args) // No args, lists environment.
	{
		char **p = environ;
		while (*p)
		{
			printf("%s\n", *p);
			++p;
		}
	}
	else
	{
		char *const *p = args;
		while (*p)
		{
			putenv(strdup(*p));
			++p;
		}
	}

	return 0;
}

int
cmd_version(char *const *args)
{
	print_version();
	return 0;
}

