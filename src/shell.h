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

#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "assert.h"
#include "array.h"
#include "object.h"

#define DEFAULT_COMMAND "execfg"
#define DEFAULT_PROMPT "\001\033[31;1m\002>\001\033[0m\002 "

typedef struct Shell Shell;
typedef struct ShellClass ShellClass;

#define SHELL(pointer) ((Shell *) pointer)

#define SHELL_CLASS(pointer) ((ShellClass *) pointer)

/**
 * Represents the Shell class or a Shell-based class.
 */
struct ShellClass {
	ObjectClass parent;
};

/**
 * Allocates and initializes a new Shell-based class of size "size" with name
 * "name".
 *
 * This function is only useful to create a Shell-based class.
 *
 * @param size   The size of the structure of the class to allocate (must be
 *               greater or equal to "sizeof (ShellClass)".
 * @param parent An owned reference to the parent class.
 * @param name   The name of the class (must not be NULL).
 *
 * @return The new allocated memory with all fields filled.
 */
ShellClass *
shell_class_allocate (size_t size, void *parent, char *name);

/**
 * Returns an owned reference the Shell class.
 *
 * When no longer needed, the reference should be unreferenced by calling
 * "object_class_unref (void *)".
 *
 * This function is only useful to create a Shell-based class.
 *
 * @return The reference.
 */
ShellClass *
shell_class_get (void);

typedef int (*func_cmd_t) (Shell *s, void *args);

/**
 * This type is used to register a new internal command.
 **/
typedef struct
{
	/**
	 * The name of the command.
	 **/
	char *name;

	/**
	 * A pointer to the function to execute for this command.
	 **/
	func_cmd_t function;

	/**
	 * String containing a help message for this command, or NULL if none.
	 **/
	char *help;

	/**
	 * String containing the arguments list, or NULL if none.
	 **/
	char *args_list;
} command_t;

/**
 * Represents an instance of the Shell type.
 */
struct Shell {
	Object parent;

	/**
	 * The name of the shell (used for configuration).
	 */
	char *name;

	/**
	 * The prompt (may be NULL).
	 */
	char *prompt;

	/**
	 * The name of the default command.
	 *
	 * When the command entered by the user is not a valid command, the default
	 * command is runned with the command line as arguments.
	 */
	char *default_command;

	/**
	 * Array of command_t.
	 */
	Array *commands;

	/**
	 * The configuration directory of the shell (usually $HOME/.config/@name/).
	 */
	char *config_dir;

	/**
	 * The file which contains the history (@config_dir/history).
	 */
	char *history_file;

	/**
	 * True if the shell has been stop, else false.
	 */
	bool done;
};

/**
 * Allocates a memory space of size "size" and initializes the Shell object.
 *
 * @param size   The memory space to allocate (greater or equal to
 *               "sizeof (Shell)").
 * @param klass  An owned reference to the class of this object (must not be
 *               NULL).
 * @param name   The shell's name (must not be NULL).
 * @param prompt A prompt (may be NULL).
 *
 * @return An owned reference to the newly allocated Shell.
 */
Shell *
shell_construct (size_t size, void *klass, const char *name, const char *prompt);

/**
 * Allocates and initializes a new Shell object with a default prompt.
 *
 * @param name The shell's name (must not be NULL).
 *
 * @return An owned reference to the newly allocated Shell or NULL if there was
 *         an error.
 */
static inline Shell *
shell_new (const char *name);

/**
 * Allocates and initializes a new Shell object with a prompt.
 *
 * @param name   The shell's name (must not be NULL).
 * @param prompt A prompt (may be NULL).
 *
 * @return An owned reference to the newly allocated Shell or NULL if there was
 *         an error.
 */
static inline Shell *
shell_new_with_prompt (const char *name, const char *prompt);

void
shell_add_command (const void *self, const char *name, func_cmd_t function,
	const char *args_list, const char *help);

int
shell_execute_command_line (void *self, Array *command_line, int *status);

const char *
shell_get_config_dir (void *self);

const command_t *
shell_get_command (const void *self, const char *name);

Array *
shell_get_command_line (void *self);

static inline const Array *
shell_get_commands (const void *self);

static inline const command_t *
shell_get_default_command (const void *self);

const char *
shell_get_history_file (void *self);

static inline const char *
shell_get_name (const void *self);

static inline const char *
shell_get_prompt (const void *self);

static inline bool
shell_is_done (const void *self);

/**
 * Class method which parse a given command line.
 *
 * @param cmd_line The command line.
 *
 * @return The command line parsed.
 */
Array *
shell_parse_command_line(const char *cmd_line);

void
shell_reset (void *self);

static inline void
shell_set_default_command (void *self, const char *name);

static inline void
shell_stop (void *self);


// Inline functions:

static inline const Array *
shell_get_commands (const void *self)
{
	assert (self);

	return SHELL (self)->commands;
}

static inline const command_t *
shell_get_default_command (const void *self)
{
	assert (self);

	return shell_get_command (self, SHELL (self)->default_command);
}

static inline const char *
shell_get_name (const void *self)
{
	assert (self);

	return SHELL (self)->name;
}

static inline const char *
shell_get_prompt (const void *self)
{
	assert (self);

	return SHELL (self)->prompt;
}

static inline bool
shell_is_done (const void *self)
{
	assert (self);

	return SHELL (self)->done;
}

static inline Shell *
shell_new (const char *name)
{
	return shell_construct (sizeof (Shell), shell_class_get (), name, DEFAULT_PROMPT);
}

static inline Shell *
shell_new_with_prompt (const char *name, const char *prompt)
{
	return shell_construct (sizeof (Shell), shell_class_get (), name, prompt);
}

static inline void
shell_set_default_command (void *self, const char *name)
{
	assert (self);
	assert (name);

	SHELL (self)->default_command = strdup (name);
}

static inline void
shell_stop (void *self)
{
	assert (self);

	rl_done = 1;
	SHELL (self)->done = true;
}

#endif

