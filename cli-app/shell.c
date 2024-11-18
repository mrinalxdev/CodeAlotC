#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define TOKEN_DELIMITERS " \t\r\n\a"


// Reads the input from the user and allocates it to a memory
// Allocates memory for the input string with fixed buffer size
// checks if memory allocation failed  then process to read input from the standard input into the allocated memory
// at last returns the allocated memory containing user input
char *read_line(){
    char *line = malloc(BUFFER_SIZE);
    if (!line){
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }

    if (fgets(line, BUFFER_SIZE, stdin) == NULL){
        free(line);
        return NULL;
    }
    return line;
}

// Parse input into tokens
char **parse_line(char *line){
    int buffer_size = 64, position = 0;
    char **tokens = malloc(buffer_size * sizeof(char *));
    char *token;

    if (!tokens) {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIMITERS);
    while (token != NULL){
        tokens[position++] = token;

        if (position >= buffer_size){
            buffer_size *= 2;
            tokens = realloc(tokens, buffer_size * sizeof(chat *));
            if (!tokens) {
                perror("Reallocation error");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

// Execute commands
int execute_command(char **args){
    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }

    if(strcmp(args[0], "cd") == 0){
        if (args[1] == NULL || chdir(args[1]) != 0){
            perror("cd");
        }
        return 1;
    }
    if (strcmp(args[0], "help") == 0){
        printf("CosmoShell : Supports basic commands !!");
        return 1;
    }

    pid_t pid = fork();
    if(pid == 0){
        // todo (after dinner) : logic for child process
        if(execvp(args[0], args) == -1) {
            perror("Command execution error");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0){
        perror("Fork error");
    } else {
        // todo (after dinner) : logic for parent process
        wait(NULL);
    }
    return 1;
}