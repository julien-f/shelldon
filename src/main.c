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
#include "shell.h"
#include "tools.h"
#include "version.h"

/**
 * Alias definition for infinite loop.
 **/
#define forever for (;;)

/**
 * The prompt (pretty obvious isn't it?).
 **/
const char *prompt = "\33[31;1m>\33[0m ";

int
main(int argc, char *const *argv)
{
	{
		// Prevents SIGINT from stopping the process.
		struct sigaction handler;
		handler.sa_handler = SIG_IGN;
		handler.sa_flags = 0;
//		handler.sa_mask = 0;
		sigaction(SIGINT, &handler, NULL);
	}

	print_version();

	char *config_dir = getenv("XDG_CONFIG_HOME");
	if (!config_dir || '\0' == *config_dir)
	{
		config_dir = strcat2(NULL, get_home_dir(), "/.config", NULL);
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
		char *p = strcat2(get_cwd(), prompt, NULL);
		string = readline(p);
		free(p);
		if (!string) // readline only received EOF.
		{
			write_history(history_file);
			putchar('\n');
			cmd_exit(NULL);
		}
		if ('\0' != *string) // The string is not empty.
		{
			parsed_cmd_line = parse_cmd_line(string);
			if (parsed_cmd_line)
			{
				add_history(string);
				exec_cmd((const char* const*) parsed_cmd_line);
				free_parsed_cmd_line(parsed_cmd_line);
			}
		}
		free(string);
	}

	return EXIT_SUCCESS;
}

