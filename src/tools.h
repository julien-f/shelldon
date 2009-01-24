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

#ifndef SHELLDON_TOOLS_H
#define SHELLDON_TOOLS_H

#include <stdarg.h>
#include <stdlib.h>

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
	int (*function)(const char *const *);

	/**
	 * String containing a help message for this command, or NULL if none. 
	 **/
	char *help;
} cmd;

/**
 * Counts the number of items in a NULL-terminated vector of strings
 * (i.e. char**).
 *
 * @param args The vector.
 * @return The number of items.
 **/
size_t
get_args_lg(const char *const *args);

/**
 * Concatenates a variable number of strings into @dest. @dest is automatically
 * resized to fit. If @dest is NULL, a new string will be allocated via malloc.
 *
 * @param dest The destination string or NULL.
 * @param ... The strings to concatenate to @dest, the last argument must be
 *            NULL.
 * @return NULL if the concatenation failed, else a pointer to the destination
 *         string (it might have changed).
 **/
char *
strcat2(char *dest, ...);

/**
 * Returns the current user's home directory.
 * The directory is search in the password database and in the HOME environment
 * variable in that order. If none found, the value returned is get_tmp_dir().
 *
 * @return The home directory.
 **/
const char *
get_home_dir();

/**
 * Returns the current user's name or NULL if not found.
 *
 * @param The user name.
 **/
const char 
*get_user_name();

/**
 * Returns the current user's real name or NULL if not found.
 *
 * @param The real name.
 **/
const char *
get_real_name();

/**
 * Returns the directory to use for temporary file.
 * The directory is search in the TMPDIR, TMP and TEMP environment variables in
 * that order. If none found, the value returned is "/tmp".
 *
 * @param The real name.
 **/
const char *
get_tmp_dir();

// get_current_dir_name() and strndup() are GNU extensions, so we have to define
// them ourselves if they are not already defined.
#ifndef _GNU_SOURCE

/**
 * TODO: write help.
 **/
char *
strndup(const char *s, size_t n);

/**
 * TODO: write help.
 **/
char *
get_cwd();

#else

/**
 * TODO: write help.
 **/
#define get_cwd() get_current_dir_name()

#endif


#endif

