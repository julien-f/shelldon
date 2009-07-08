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

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "shell.h"
#include "version.h"

#include "object.h"
#include "array.h"

int
main(int argc, char *const *argv)
{
	{
		void *o = object_new (NULL);
		void *a = array_new (NULL);
		void *o2 = object_new (NULL);
		void *a2 = array_new (NULL);

		printf ("%s\n", object_get_class_name (o));
		printf ("%s\n", object_get_class_name (a));

		object_unref (o);
		object_unref (a);
		object_unref (o2);
		object_unref (a2);
	}

/*	{*/
/*		// Prevents SIGINT & SIGTSTP from stopping the process.*/
/*		struct sigaction handler;*/
/*		handler.sa_handler = SIG_IGN;*/
/*		handler.sa_flags = 0;*/
/*//		handler.sa_mask = 0;*/
/*		sigaction(SIGINT, &handler, NULL);*/
/*		sigaction(SIGTSTP, &handler, NULL);*/
/*	}*/

/*	initialize_shell();*/

/*	print_version();*/

/*	char *string = NULL;*/
/*	char **parsed_cmd_line = NULL;*/
/*	while (!shell_done)*/
/*	{*/
/*		if ( (string = get_cmd_line()) ) // The string is not empty.*/
/*		{*/
/*			parsed_cmd_line = parse_cmd_line(string);*/
/*			if (parsed_cmd_line)*/
/*			{*/
/*				exec_cmd((const char* const*) parsed_cmd_line, NULL);*/
/*				free_parsed_cmd_line(parsed_cmd_line);*/
/*			}*/
/*			free(string);*/
/*		}*/
/*	}*/

/*	finalize_shell();*/

/*	printf("Bye.\n");*/

	return EXIT_SUCCESS;
}

