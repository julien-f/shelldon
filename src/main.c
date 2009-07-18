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

#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "array.h"
#include "cmd.h"
#include "object.h"
#include "shell.h"
#include "version.h"

#include "string.h"

int
main ()
{
	{
		// Prevents SIGINT & SIGTSTP from stopping the process.
		struct sigaction handler;
		handler.sa_handler = SIG_IGN;
		handler.sa_flags = 0;
		sigemptyset (&handler.sa_mask);
		sigaction (SIGINT, &handler, NULL);
		sigaction (SIGTSTP, &handler, NULL);
	}

	Shell *shell = shell_new (get_prog_name ());

	shell_add_command (shell, "cd", cmd_cd, "[DIR]",
		"Changes the current directory to DIR. If DIR is \"-\", tries to move to\n"
		"the previous one. Finally, if DIR is not specified, tries to move to\n"
		"your home directory.");
	shell_add_command (shell, "history", cmd_history, "-c",
		"Manages the history.");
	shell_add_command (shell, "exec", cmd_exec, "PATH",
		"Replaces the current shell with the program PATH.");
	shell_add_command (shell, "execbg", cmd_execbg, "PATH", NULL);
	shell_add_command (shell, "execfg", cmd_execfg, "PATH", NULL);
	shell_add_command (shell, "exit", cmd_exit, NULL, "Leaves the shell.");
	shell_add_command (shell, "help", cmd_help, "[COMMAND...]",
		"Lists the available commands or shows the help message of COMMAND.");
	shell_add_command (shell, "pwd", cmd_pwd, NULL,
		"Shows the current working directory.");
	shell_add_command (shell, "setenv", cmd_setenv, NULL,
		"Lists and sets environment variables.");
	shell_add_command (shell, "version", cmd_version, "[-n|-v]",
		"Shows the version of Shelldon.");
	shell_add_command (shell, "sdc", cmd_sdc, "COMMAND", NULL);

/*	print_version ();*/

	while (!shell_is_done (shell))
	{
		Array *cl;
		if ( (cl = shell_get_command_line (shell)) ) // The command line is not empty.
		{
			if (shell_execute_command_line (shell, cl, NULL) == -1)
			{
				fprintf (stderr, "Unable to execute your last command.\n");
			}
			object_unref (cl);
		}
	}

	printf ("Bye.\n");

	object_unref (shell);

	return EXIT_SUCCESS;
}

