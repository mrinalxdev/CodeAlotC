#ifndef SHELL_H
#define SHELL_H

char *read_line();
char **parse_line(char *line);
int execute_command(char **args);

#endif