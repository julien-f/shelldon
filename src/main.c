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
#include "expression.h"
#include "expression_list.h"

int
main(int argc, char *const *argv)
{
	{
		size_t n = 5;
		void *a = array_new (object_unref);

		for (size_t i = 0; i < n; ++i)
		{
			array_add (a, expression_new ());
		}
		for (size_t i = 0; i < n; ++i)
		{
			array_add (a, expression_list_new ());
		}

		for (size_t i = 0, n = array_get_length (a); i < n; ++i)
		{
			void *o = array_get (a, i);
			assert (o);
			printf ("%s.evaluate (): %d\n", object_get_class_name (o), expression_evaluate (o));
		}

		object_unref (a);
	}
	{
		size_t n = 5;
		void *a = array_new (object_unref);

		for (size_t i = 0; i < n; ++i)
		{
			array_add (a, expression_new ());
		}
		for (size_t i = 0; i < n; ++i)
		{
			array_add (a, expression_list_new ());
		}

		for (size_t i = 0, n = array_get_length (a); i < n; ++i)
		{
			void *o = array_get (a, i);
			assert (o);
			printf ("%s.evaluate (): %d\n", object_get_class_name (o), expression_evaluate (o));
		}

		object_unref (a);
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

