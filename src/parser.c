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
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "array.h"
#include "string.h"

void *
parse_cmd_line(const char *cmd_line)
{
	size_t cmd_line_lg = strlen (cmd_line);

	void *result = array_new (free);

	void *buffer = string_new ();

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
			if (string_get_length (buffer) != 0)
			{
				array_append (result, strdup (string_get_str (buffer)));
				string_clear (buffer);
			}
		}
		else if (!escaped && ((cmd_line[i] == '\'' && current_delim != '"')
				|| (cmd_line[i] == '"' && current_delim != '\'')))
		{
			if (string_get_length (buffer) != 0)
			{
				array_append (result, strdup (string_get_str (buffer)));
				string_clear (buffer);
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
			string_append_char (buffer, cmd_line[i]);
		}
	}
	if (string_get_length (buffer) != 0)
	{
		array_append (result, strdup (string_get_str (buffer)));
	}
	object_unref (buffer);

	return result;
}

