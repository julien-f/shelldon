#include "parser.h"

#include <error.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * Add a character to a string.
 *
 * @param character The character to add.
 * @param pstring A pointer to the string.
 * @param psize A pointer to size of the string in memory.
 * @param plength A pointer to the length of the string.
 **/
static void add_character_on_string(char character, char **pstring,
		size_t *psize, size_t *plength)
{
	if (++*plength > *psize)
	{
		*psize += 25;
		*pstring = realloc(*pstring, sizeof(char) * *psize);
		if (*pstring == NULL)
		{
			error(EXIT_FAILURE, 0, "Error: realloc failed.\n");
		}
	}
	(*pstring)[*plength - 1] = character;
	(*pstring)[*plength] = 0;
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
static void add_string_on_vector(const char *string, size_t string_lg,
		char ***pvector, size_t *psize, size_t *plength)
{
	if (++*plength > *psize)
	{
		*psize += 10;
		*pvector = realloc(*pvector, sizeof(char *) * *psize);
		if (*pvector == NULL)
		{
			error(EXIT_FAILURE, 0, "Error: realloc failed.\n");
		}
	}
	(*pvector)[*plength - 1] = malloc(sizeof(char) * (string_lg + 1));
	strcpy((*pvector)[*plength - 1], string);
	(*pvector)[*plength] = NULL;
}

/**
 * Parse a command line.
 *
 * @param cmd_line The command line to parse.
 * @return The command line parsed.
 **/
char **parse_cmd_line(const char *cmd_line)
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
void free_parsed_cmd_line(char **parsed_cmd_line)
{
	for (register size_t i = 0; parsed_cmd_line[i] != NULL; ++i)
	{
		free(parsed_cmd_line[i]);
	}
	free(parsed_cmd_line);
}
