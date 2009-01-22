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

/**
 * Parse a command line.
 *
 * @param cmd_line The command line to parse.
 * @return The command line parsed.
 **/
char **
parse_cmd_line(const char *cmd_line);

/**
 * Free a parsed command line return by parse_cmd_line.
 *
 * @param parsed_cmd_line The parsed command line to free.
 **/
void
free_parsed_cmd_line(char **parsed_cmd_line);

#endif

