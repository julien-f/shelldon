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

#include "object.h"
#include "parser.h"
#include "shell.h"
#include "version.h"

int
main (int argc, char *const *argv)
{
	{
		// Prevents SIGINT & SIGTSTP from stopping the process.
		struct sigaction handler;
		handler.sa_handler = SIG_IGN;
		handler.sa_flags = 0;
//		handler.sa_mask = 0;
		sigaction (SIGINT, &handler, NULL);
		sigaction (SIGTSTP, &handler, NULL);
	}

	initialize_shell ();

	print_version ();

	char *string = NULL;
	void *parsed_cmd_line = NULL;
	while (!shell_done)
	{
		if ( (string = get_cmd_line ()) ) // The string is not empty.
		{
			parsed_cmd_line = parse_cmd_line (string);
			if (parsed_cmd_line)
			{
				exec_cmd (parsed_cmd_line, NULL);
				object_unref (parsed_cmd_line);
			}
			free (string);
		}
	}

	finalize_shell ();

	printf ("Bye.\n");

	return EXIT_SUCCESS;
}

