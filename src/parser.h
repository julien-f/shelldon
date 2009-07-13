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

#ifndef SHELLDON_PARSER_H
#define SHELLDON_PARSER_H

#include "array.h"

/**
 * Parses a command line and return a string containing Array which should be
 * deallocate when no longer needed with "object_unref (void *)".
 *
 * @param cmd_line The command line to parse.
 * @return The Array.
 **/
Array *
parse_cmd_line(const char *command_line);

#endif

