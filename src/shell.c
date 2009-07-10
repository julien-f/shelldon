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
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "assert.h"
#include "array.h"
#include "cmd.h"
#include "shell.h"
#include "tools.h"
#include "version.h"

#define DEFAULT_PROMPT "\33[31;1m>\33[0m "

char shell_done = 0;

/**
 * This list contains all the internal commands.
 *
 * The functions are defined in "cmd.{h,c}.
 *
 * The last entry MUST BE "{NULL}".
 **/
static const cmd cmd_list[] = {
	{"cd", cmd_cd, "cd [DIR]: Changes the current directory to DIR if specified, "
			"else to your home directory."},
	{"history", cmd_history, "Manages the history."},
	{"exec", cmd_exec, "exec PATH: Replaces the current shell with the program "
			"PATH."},
	{"execbg", cmd_execbg, NULL},
	{"execfg", cmd_execfg, NULL},
	{"exit", cmd_exit, "Leaves the shell."},
	{"help", cmd_help, "help [COMMAND]: Lists the available commands or shows "
			"the help message of COMMAND."},
	{"pwd", cmd_pwd, "Shows the current working directory."},
	{"setenv", cmd_setenv, "Lists and sets environment variables."},
	{"version", cmd_version, "Shows the version of Shelldon."},
	{NULL}
};

static char *default_cmd = "execfg";

static char *history_file = NULL;

/**
 * The prompt (pretty obvious isn't it?).
 **/
static char *prompt = NULL;

static char *shell_config_dir = NULL;

static const char *
get_shell_config_dir ()
{
	if (!shell_config_dir)
	{
		const char *config_dir = get_config_dir ();
		if (config_dir)
		{
			shell_config_dir = strcat2 (NULL, config_dir, "/", get_prog_name (), NULL);
			if (-1 == mkdir (shell_config_dir, 0777) && EEXIST != errno)
			{
				free (shell_config_dir),
				shell_config_dir = NULL;
			}
		}
	}
	return shell_config_dir;
}

static void
initialize_readline ()
{
	rl_readline_name = get_prog_name ();
	using_history ();
	const char *shell_config_dir = get_shell_config_dir ();
	if (shell_config_dir)
	{
		history_file = strcat2 (NULL, shell_config_dir, "/history", NULL);
		read_history (history_file);
	}
}

static void
finalize_readline ()
{
	if (history_file)
	{
		write_history (history_file);
	}
}

const cmd *
get_cmd (const char *name)
{
	const cmd *p = cmd_list;
	while (p->cmd && 0 != strcmp (p->cmd, name))
	{
		++p;
	}
	if (!p->cmd)
	{
		return NULL;
	}
	return p;
}

const cmd *
get_cmd_list ()
{
	return cmd_list;
}

const cmd *
get_default_cmd ()
{
	return get_cmd (default_cmd);
}


int
exec_cmd (void **cl, int *status)
{
	assert (!array_is_empty (cl));

	const cmd *p = get_cmd (array_get (cl, 0));
	if (p)
	{
		array_remove_at (cl, 0);
	}
	else if ( ! (p = get_default_cmd ()) )
	{
		return -1;
	}
	if (status)
	{
		*status = p->function (cl);
	}
	else
	{
		p->function (cl);
	}

	return 0;
}

void
initialize_shell ()
{
	if (!prompt)
	{
		prompt = strdup (DEFAULT_PROMPT);
	}

	initialize_readline ();
	shell_done = 0;
}

void
finalize_shell ()
{
	finalize_readline ();
	if (history_file)
	{
		free (history_file);
		history_file = NULL;
	}
	if (prompt)
	{
		free (prompt);
		prompt = NULL;
	}
	if (shell_config_dir)
	{
		free (shell_config_dir);
		shell_config_dir = NULL;
	}
	shell_done = 1;
}

char *
get_cmd_line ()
{
	char *string = readline (prompt);
	if (!string)
	{
		putchar ('\n');
		stop_shell ();
		return NULL;
	}
	if ('\0' == *string || shell_done)
	{
		free (string);
		return NULL;
	}
	add_history (string);
	return string;
}

void
stop_shell ()
{
	shell_done = 1;
}

