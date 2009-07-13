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

#ifndef SHELLDON_CMD_H
#define SHELLDON_CMD_H

#include "shell.h"

/**
 * Changes current directory.
 *
 * @param args An Array which contains the arguments.
 * @return 0 if success, else -1.
 **/
int
cmd_cd (Shell *shell, void *args);

/**
 * TODO: write help.
 **/
int
cmd_history (Shell *shell, void *args);

/**
 * Replaces the shell by the program args[0].
 *
 * @param args An Array which contains the arguments.
 * @return 0 if success, else -1.
 **/
int
cmd_exec (Shell *shell, void *args);

/**
 * TODO: write help.
 **/
int
cmd_execbg (Shell *shell, void *args);

/**
 * TODO: write help.
 **/
int
cmd_execfg (Shell *shell, void *args);

/**
 * Ends the shell.
 *
 * @param args An Array which contains the arguments (not used).
 * @return Nothing, because the process is destroyed.
 **/
int
cmd_exit (Shell *shell, void *args);

/**
 * Lists available commands and their description.
 *
 * @param args An Array which contains the arguments.
 * @return 0 if success, else minus the number of errors.
 **/
int
cmd_help (Shell *shell, void *args);

/**
 * Shows the current working directory.
 *
 * @param args An Array which contains the arguments (not used).
 * @return 0 if success, else -1.
 **/
int
cmd_pwd (Shell *shell, void *args);

/**
 * If there are no arguments, shows the list of environment vars, else, defines,
 * redefines or undefines environment variables.
 *
 * @param args An Array which contains the arguments.
 * @return 0 if success, else -1.
 **/
int
cmd_setenv (Shell *shell, void *args);

/**
 * Shows the version of Shelldon.
 *
 * @param args An Array which contains the arguments (not used).
 * @return 0 if success, else -1.
 **/
int
cmd_version (Shell *shell, void *args);

int
cmd_sdc (Shell *s, void *args);

#endif

