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

#ifndef SHELLDON_SHELL_H
#define SHELLDON_SHELL_H

/**
 * This type is used to register a new internal command.
 **/
typedef struct
{
	/**
	 * String corresponding to the command (i.e. its name).
	 **/
	char *cmd;

	/**
	 * A pointer to the function to execute for this command.
	 **/
	int (*function) (void *);

	/**
	 * String containing a help message for this command, or NULL if none.
	 **/
	char *help;

	/**
	 * String containing the arguments list, or NULL if none.
	 **/
	char *args_list;
} cmd;

/**
 * TODO: write help.
 **/
extern char shell_done;

/**
 * TODO: write help.
 **/
const cmd *
get_cmd (const char *);

/**
 * TODO: write help.
 **/
const cmd *
get_cmd_list ();

/**
 * TODO: write help.
 *
 * @return
 **/
const cmd *
get_default_cmd ();

/**
 * TODO: write help.
 **/
int
exec_cmd (void **cl, int *status);

/**
 * Initializes the shell.
 **/
void
initialize_shell ();

/**
 * Finalizes the shell.
 **/
void
finalize_shell ();

/**
 * TODO: write help.
 *
 * @return
 **/
char *
get_cmd_line ();

/**
 * TODO: write help.
 **/
void
stop_shell ();

#endif

