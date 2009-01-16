#define _GNU_SOURCE
#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "parser.h"

/**
 * Alias definition for infinite loop.
 **/
#define forever for (;;)

typedef struct
{
	char *cmd;
	bool (*function)(char *const *);
	char *help;
} cmd;

/**
 * Counts the number of items in vector of char (char*) ending with a NULL
 * pointer.
 *
 * @param argv The vector.
 * @return The number of items.
 **/
unsigned int get_argv_lg(char *const *argv)
{
	unsigned int lg = 0;
	while (NULL != argv[lg++]);
	return lg - 1;
}

bool cmd_cd(char *const *argv);

bool cmd_exec(char *const *argv);

bool cmd_exit(char *const *argv);

bool cmd_help(char *const *argv);

static const cmd cmd_list[] = {
	{"cd", cmd_cd, "cd PATH: Changes the current directory."},
	{"exec", cmd_exec, "exec PATH: Replaces the current shell with a program."},
	{"exit", cmd_exit, "Leaves the shell."},
	{"help", cmd_help, "Shows this help message."},
	{NULL}
};

/**
 * Built-in command cd: change the curent directory.
 **/
bool cmd_cd(char *const *argv)
{
	if (1 != get_argv_lg(argv))
	{
		fprintf(stderr, "The command cd expects exactly one argument.\n");
		return false;
	}
	return chdir(argv[0]) == 0;
}

bool cmd_exec(char *const *argv)
{
	if (0 == get_argv_lg(argv))
	{
		fprintf(stderr, "The command exec expects at least one argument.\n");
		return false;
	}
	
	execvp(argv[0], argv);
	fprintf(stderr, "Error during exec.\n");
	return false;
}

bool cmd_exit(char *const *argv)
{
	printf("Bye.\n");
	exit(0);
}

bool cmd_help(char *const *argv)
{
	// print_version();

	printf("Available commands:\n");

	const cmd *p = cmd_list;
	while (NULL != p->cmd)
	{
		printf(" - %s\n\t%s\n", p->cmd, p->help);
		++p;
	}
	return true;
}

/**
 * Executes a built-in command.
 *
 * @param The command line parsed with parse_cmd_line(const char *str).
 * @return True if the command exists, else false.
 **/
bool exec_cmd(char *const *cl)
{
	if (cl == NULL || cl[0] == NULL)
	{
		return false;
	}
	const cmd *p = cmd_list;
	while (p->cmd != NULL)
	{
		if (strcmp(cl[0], p->cmd) == 0)
		{
			p->function(++cl);
			return true;
		}
		++p;
	}
	return false;
}

/**
 * Executes a command line parsed by the parse_cmd_line function.
 *
 * @param parsed_cmd_line The command line parsed.
 * @return The result returned by the command.
 **/
int exec_parsed_cmd_line(char *const *parsed_cmd_line)
{
	switch (fork())
	{
		case -1:
			printf("Error: fork failed.\n");
			exit(EXIT_FAILURE);
		case 0:
			execvp(parsed_cmd_line[0], parsed_cmd_line);
			error(EXIT_FAILURE, errno, "Error");
	}
	int result;
	if (waitpid(-1, &result, 0) == -1)
	{
		error(EXIT_FAILURE, errno, "Error");
	}
	return result;
	
}

int main(int argc, char **argv)
{
	char *string = NULL;
	char **parsed_cmd_line = NULL;
	using_history();
	forever
	{
		string = readline("$ ");
		if (string == NULL)
		{
			putchar('\n');
			cmd_exit(NULL);
		}
		if (strlen(string) != 0)
		{
			parsed_cmd_line = parse_cmd_line(string);
			if (parsed_cmd_line != NULL)
			{
				add_history(string);
				exec_cmd(parsed_cmd_line) ||
						exec_parsed_cmd_line(parsed_cmd_line);
				free_parsed_cmd_line(parsed_cmd_line);
			}
		}
		free(string);
	}

	return EXIT_SUCCESS;
}
