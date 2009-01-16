#ifndef PARSER_H
#define PARSER_H

/**
 * Parse a command line.
 *
 * @param cmd_line The command line to parse.
 * @return The command line parsed.
 **/
char **parse_cmd_line(const char *cmd_line);

/**
 * Free a parsed command line return by parse_cmd_line.
 *
 * @param parsed_cmd_line The parsed command line to free.
 **/
void free_parsed_cmd_line(char **parsed_cmd_line);

#endif
