#include "shell.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "array.h"
#include "assert.h"
#include "debug.h"
#include "object.h"
#include "string.h"
#include "tools.h"

static void
shell_free_command (void *command);

static void
shell_real_finalize (void *);

static void
shell_class_real_finalize (void *);

static ShellClass *klass = NULL;

ShellClass *
shell_class_allocate (size_t size, void *parent, char *name)
{
	assert (name);
	assert_cmpuint (size, >=, sizeof (ShellClass));

	ShellClass *klass = SHELL_CLASS (object_class_allocate (size, parent, name));
	if (!klass) // Allocation failed
	{
		return NULL;
	}

	OBJECT_CLASS (klass)->finalize = shell_real_finalize;

	return klass;
}

ShellClass *
shell_class_get ()
{
	if (!klass) // The Shell class is not yet initalized.
	{
		klass = shell_class_allocate (sizeof (ShellClass), object_class_get (), "Shell");
		OBJECT_CLASS (klass)->finalize_class = shell_class_real_finalize;
		return klass;
	}

	return object_class_ref (klass);
}

Shell *
shell_construct (size_t size, void *klass, const char *name, const char *prompt)
{
	assert_cmpuint (size, >=, sizeof (Shell));
	assert (klass);
	assert (name);

	Shell *self =  SHELL (object_construct (size, klass));

	self->name = strdup (name);
	self->prompt = (prompt && *prompt ? strdup (prompt) : NULL);
	self->default_command = strdup (DEFAULT_COMMAND);
	self->commands = array_new (shell_free_command);
	self->history_file = NULL;
	self->config_dir = NULL;
	self->done = true;

	shell_reset (self);

	return self;
}

void
shell_add_command (const void *self, const char *name, func_cmd_t function,
	const char *args_list, const char *help)
{
	assert (self);
	assert (name);
	assert (function);

	command_t *p = malloc (sizeof (command_t));
	assert (p);

	p->name = strdup (name);
	p->args_list = (args_list ? strdup (args_list) : NULL);
	p->help = (help ? strdup (help) : NULL);

	p->function = function;

	array_append (SHELL (self)->commands, p);
}

int
shell_execute_command_line (void *self, Array *command_line, int *status)
{
	assert (self);
	assert (!array_is_empty (command_line));

	const command_t *p = shell_get_command (self, array_get (command_line, 0));
	if (p)
	{
		array_remove_at (command_line, 0);
	}
	else if ( !(p = shell_get_default_command (self)) )
	{
		return -1;
	}
	if (status)
	{
		*status = p->function (self, command_line);
	}
	else
	{
		p->function (self, command_line);
	}

	return 0;
}

const command_t *
shell_get_command (const void *self, const char *name)
{
	assert (self);
	assert (name);


	const Array *commands = shell_get_commands (self);
	for (
		size_t i = 0, n = array_get_length (commands);
		i < n;
		++i
	)
	{
		const command_t *c = array_get (commands, i);
		if (!strcmp (c->name, name))
		{
			return c;
		}
	}

	return NULL;
}

Array *
shell_get_command_line (void *self)
{
	assert (self);

	if (shell_is_done (self))
	{
		shell_reset (self);
	}

	char *string = readline (shell_get_prompt (self));
	if (!string)
	{
		putchar ('\n');
		shell_stop (self);
		return NULL;
	}
	if ('\0' == *string || shell_is_done (self))
	{
		free (string);
		return NULL;
	}
	add_history (string);

	Array *a = shell_parse_command_line (string);
	free (string);

	return a;
}

const char *
shell_get_config_dir (void *self)
{
	assert (self);

	if ( !(SHELL (self)->config_dir) )
	{
		const char *config_dir = get_config_dir ();
		if (config_dir)
		{
			SHELL (self)->config_dir = string_concat (NULL, config_dir, "/",
				shell_get_name (self), NULL);
			if (-1 == mkdir (SHELL (self)->config_dir, 0777) && EEXIST != errno)
			{
				free (SHELL (self)->config_dir),
				SHELL (self)->config_dir = NULL;
			}
		}
	}

	return SHELL (self)->config_dir;
}

const char *
shell_get_history_file (void *self)
{
	assert (self);

	if ( !(SHELL (self)->history_file) )
	{
		const char *config_dir = shell_get_config_dir (self);
		if (config_dir)
		{
			SHELL (self)->history_file = string_concat (NULL, config_dir,
				"/history", NULL);
		}
	}

	return SHELL (self)->history_file;
}

Array *
shell_parse_command_line(const char *cmd_line)
{
	size_t cmd_line_lg = strlen (cmd_line);

	Array *result = array_new (free);

	String *buffer = string_new ();

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
				array_append (result, string_steal (buffer));
			}
		}
		else if (!escaped && ((cmd_line[i] == '\'' && current_delim != '"')
				|| (cmd_line[i] == '"' && current_delim != '\'')))
		{
			if (string_get_length (buffer) != 0)
			{
				array_append (result, string_steal (buffer));
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
		array_append (result, string_steal (buffer));
	}
	object_unref (buffer);

	return result;
}

void
shell_reset (void *self)
{
	assert (self);

	rl_readline_name = shell_get_name (self);

	using_history ();
	const char *history_file = shell_get_history_file (self);
	if (history_file)
	{
		read_history (history_file);
	}

	rl_initialize ();

	SHELL (self)->done = false;
}

static void
shell_free_command (void *p)
{
	assert (p);

	command_t *command = p;

	free (command->name);
	free (command->args_list);
	free (command->help);

	free (command);
}

static void
shell_real_finalize (void *self)
{
	assert (self);
	assert (klass);

	if (SHELL (self)->history_file)
	{
		write_history (SHELL (self)->history_file);
		free (SHELL (self)->history_file);
	}
	clear_history ();

	free (SHELL (self)->name);
	free (SHELL (self)->prompt);
	free (SHELL (self)->default_command);
	free (SHELL (self)->config_dir);
	object_unref (SHELL (self)->commands);

	object_class_get_parent (klass)->finalize (self);
}

static void
shell_class_real_finalize (void *_klass)
{
	assert (_klass == klass);
	klass = NULL;
}

