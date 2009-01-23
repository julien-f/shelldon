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

#include <error.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/**
 * Add a character to a string.
 *
 * @param character The character to add.
 * @param pstring A pointer to the string.
 * @param psize A pointer to size of the string in memory.
 * @param plength A pointer to the length of the string.
 **/
static void
add_character_on_string(char character, char **pstring, size_t *psize,
		size_t *plength)
{
	if (*plength >= *psize)
	{
		*psize += 25;
		*pstring = (char *) realloc((void *) *pstring, *psize);
		if (!*pstring)
		{
			error(EXIT_FAILURE, 0, "Error: realloc failed.\n");
		}
	}
	(*pstring)[*plength] = character;
	(*pstring)[++*plength] = '\0';
}

/**
 * Add a string to a vector.
 *
 * @param string The string to add.
 * @param string_lg The length of the string.
 * @param pvector A pointer to the vector.
 * @param psize A pointer to size of the vector in memory.
 * @param plength A pointer to the length of the vector.
 **/
static void
add_string_on_vector(const char *string, size_t string_lg,
		char ***pvector, size_t *psize, size_t *plength)
{
	if (*plength >= *psize)
	{
		*psize += 10;
		*pvector = (char **) realloc((void *) *pvector, sizeof(char *) * *psize);
		if (!*pvector)
		{
			error(EXIT_FAILURE, 0, "Error: realloc failed.\n");
		}
	}
	if (!((*pvector)[*plength] = strdup(string)))
	{
		error(EXIT_FAILURE, 0, "Error: strdup failed.\n");
	}
	(*pvector)[++*plength] = NULL;
}

/**
 * Parse a command line.
 *
 * @param cmd_line The command line to parse.
 * @return The command line parsed.
 **/
char **
parse_cmd_line(const char *cmd_line)
{
	size_t cmd_line_lg = strlen(cmd_line);

	char **result = NULL;
	size_t result_lg = 0;
	size_t result_size = 0;

	char *buffer = NULL;
	size_t buffer_lg = 0;
	size_t buffer_size = 0;

	char current_delim = ' ';
	bool escaped = false;

	for (register size_t i = 0; i < cmd_line_lg; ++i)
	{
		if (!escaped && cmd_line[i] == '\\')
		{
			escaped = true;
		}
		else if (!escaped && cmd_line[i] == ' ' && current_delim == ' ')
		{
			if (buffer_lg != 0)
			{
				add_string_on_vector(buffer, buffer_lg, &result,
						&result_size, &result_lg);
				buffer_lg = 0;
				buffer[0] = 0;
			}
		}
		else if (!escaped && ((cmd_line[i] == '\'' && current_delim != '"')
				|| (cmd_line[i] == '"' && current_delim != '\'')))
		{
			if (buffer_lg != 0)
			{
				add_string_on_vector(buffer, buffer_lg, &result,
						&result_size, &result_lg);
				buffer_lg = 0;
				buffer[0] = 0;
			}
			if (current_delim == ' ')
			{
				current_delim = cmd_line[i];
			}
			else
			{
				current_delim = ' ';
			}
		}
		else
		{
			if (escaped)
			{
				escaped = false;
			}
			add_character_on_string(cmd_line[i], &buffer, &buffer_size,
					&buffer_lg);
		}
	}
	if (buffer_lg != 0)
	{
		add_string_on_vector(buffer, buffer_lg, &result, &result_size,
				&result_lg);
	}
	free(buffer);

	return result;
}

/**
 * Free a parsed command line return by parse_cmd_line.
 *
 * @param parsed_cmd_line A pointer to the parsed command line to free.
 **/
void
free_parsed_cmd_line(char **parsed_cmd_line)
{
	if (parsed_cmd_line)
	{
		char **p = parsed_cmd_line;
		while (*p)
		{
			free(*(p++));
		}
		free(parsed_cmd_line);
	}
}

