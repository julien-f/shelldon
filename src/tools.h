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
	char *cmd;
	int (*function)(char *const *);
	char *help;
} cmd;

/**
 * Counts the number of items in vector of char (char*) ending with a NULL
 * pointer.
 *
 * @param args The vector.
 * @return The number of items.
 **/
size_t get_args_lg(char *const *args);

/**
 * Concatenate strings.
 * All strings are concatenate into the first one.
 *
 * @param dest The destination string, can be NULL.
 * @param ... The strings to concatenate to dest, the last argument must be NULL.
 * @return NULL if the concatenation failed, else a pointer to the destination
 *         string (it might have changed).
 **/
char *strcat2(char *dest, ...);

/**
 * TODO: write help.
 **/
const char *get_home_dir();

/**
 * TODO: write help.
 **/
const char *get_user_name();

/**
 * TODO: write help.
 **/
const char *get_real_name();

#endif

