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

#include <string.h>

#include "cmd.h"
#include "shell.h"

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

	{"test", cmd_test, NULL},
	{NULL}
};

static char *default_cmd = "execfg";

const cmd *
get_cmd(const char *name)
{
	const cmd *p = cmd_list;
	while (p->cmd && 0 != strcmp(p->cmd, name))
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
get_cmd_list()
{
	return cmd_list;
}

const cmd *
get_default_cmd()
{
	return get_cmd(default_cmd);
}

