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
#include <unistd.h>

#include "cmd.h"
#include "tools.h"
#include "version.h"

extern const cmd cmd_list[];
extern char **environ;
extern int putenv(char *string);;

int cmd_cd(char *const *args)
{
	if (1 != get_args_lg(args))
	{
		fprintf(stderr, "The command cd expects exactly one argument.\n");
		return -1;
	}
	return chdir(args[0]);
}

int cmd_exec(char *const *args)
{
	if (NULL == args || NULL == *args) // no args.
	{
		fprintf(stderr, "The command exec expects at least one argument.\n");
		return -1;
	}
	
	execvp(args[0], args);
	fprintf(stderr, "Error during exec.\n");
	return -1;
}

int cmd_exit(char *const *args)
{
	printf("Thx 4 usin' %s.\n", prog_name);
	exit(0);
}

int cmd_help(char *const *args)
{
	printf("Available commands:\n");

	const cmd *p = cmd_list;
	while (NULL != p->cmd)
	{
		printf("* %s\n  %s\n", p->cmd, p->help);
		++p;
	}
	return 0;
}

int cmd_setenv(char *const *args)
{
	if (NULL == args || NULL == *args) // No args, lists environment.
	{
		char **p = environ;
		while (NULL != *p)
		{
			printf("%s\n", *p);
			++p;
		}
	}
	else
	{
		char *const *p = args;
		while (NULL != *p)
		{
			// Checks the length of the string, null char included.
			size_t lg = 0;
			char *copy = *p;
			while ('\0' != copy[lg++]);

			// Copies the string *p into copy.
			copy = (char *) malloc(sizeof(char) * lg);
			lg = 0;
			while ('\0' != (copy[lg++] = (*p)[lg]));

			// Gives it to the environment (so do not free it after).
			putenv(copy);

			++p;
		}
	}

	return 0;
}

int cmd_version(char *const *args)
{
	print_version();
	return 0;
}

