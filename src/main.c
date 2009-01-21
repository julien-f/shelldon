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
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "cmd.h"
#include "parser.h"
#include "tools.h"
#include "version.h"

/**
 * Alias definition for infinite loop.
 **/
#define forever for (;;)

/**
 * This list contains all the internal commands.
 *
 * The functions are defined in "cmd.{h,c}.
 *
 * The last entry MUST BE "{NULL}".
 **/
const cmd cmd_list[] = {
	{"cd", cmd_cd, "cd PATH: Changes the current directory to PATH."},
	{"exec", cmd_exec, "exec PATH: Replaces the current shell with the program "
			"PATH."},
	{"exit", cmd_exit, "Leaves the shell."},
	{"help", cmd_help, "help [COMMAND]: Lists the available commands or shows "
			"the help message of COMMAND."},
	{"setenv", cmd_setenv, "Lists and sets environment variables."},
	{"version", cmd_version, "Shows the version of Shelldon."},
	{NULL}
};

/**
 * The prompt (pretty obvious isn't it?).
 **/
const char *prompt = "\33[31;1m>\33[0m ";

/**
 * Executes a built-in command.
 *
 * @param The command line parsed with parse_cmd_line(const char *str).
 * @return True if the command exists, else false.
 **/
bool exec_cmd(char *const *cl)
{
	if (cl == NULL || cl[0] == NULL)
	{
		return false;
	}
	const cmd *p = cmd_list;
	while (p->cmd != NULL)
	{
		if (strcmp(cl[0], p->cmd) == 0)
		{
			p->function(++cl);
			return true;
		}
		++p;
	}
	return false;
}

/**
 * Executes a command line parsed by the parse_cmd_line function.
 *
 * @param parsed_cmd_line The command line parsed.
 * @return The result returned by the command.
 **/
int exec_parsed_cmd_line(char *const *parsed_cmd_line)
{
	switch (fork())
	{
		case -1:
			printf("Error: fork failed.\n");
			exit(EXIT_FAILURE);
		case 0:
			execvp(parsed_cmd_line[0], parsed_cmd_line);
			error(EXIT_FAILURE, errno, "Error");
	}
	int result;
	if (waitpid(-1, &result, 0) == -1)
	{
		error(EXIT_FAILURE, errno, "Error");
	}
	return result;
	
}

int main(int argc, char *const *argv)
{
	{
		// Prevents SIGINT from stopping the process.
		struct sigaction handler;
		handler.sa_handler = SIG_IGN;
		handler.sa_flags = 0;
		sigaction(SIGINT, &handler, NULL);
	}

	print_version();

	char *home_dir = getenv("HOME");
	if (NULL == home_dir)
	{
		home_dir = "/tmp";
	}

	char *config_dir = getenv("XDG_CONFIG_HOME");
	if (NULL == config_dir || '\0' == *config_dir)
	{
		config_dir = strcat2(NULL, home_dir, "/.config", NULL);
	}
	if (-1 == mkdir(config_dir, 0777) && EEXIST != errno)
	{
		error(EXIT_FAILURE, errno, "Error");
	}

	char *prog_cfg_dir = strcat2(NULL, config_dir, "/" prog_name, NULL);
	if (-1 == mkdir(prog_cfg_dir, 0777) && EEXIST != errno)
	{
		error(EXIT_FAILURE, errno, "Error");
	}

	char *history_file = strcat2(NULL, prog_cfg_dir, "/history", NULL);

	char *string = NULL;
	char **parsed_cmd_line = NULL;
	using_history();
	read_history(history_file);
	forever
	{
		string = readline(prompt);
		if (string == NULL)
		{
			write_history(history_file);
			putchar('\n');
			cmd_exit(NULL);
		}
		if (strlen(string) != 0)
		{
			parsed_cmd_line = parse_cmd_line(string);
			if (parsed_cmd_line != NULL)
			{
				add_history(string);
				exec_cmd(parsed_cmd_line) ||
						exec_parsed_cmd_line(parsed_cmd_line);
				free_parsed_cmd_line(parsed_cmd_line);
			}
		}
		free(string);
	}

	return EXIT_SUCCESS;
}

