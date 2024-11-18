#include "shell.h"
#include <stdio.h>  
#include <stdlib.h>

int main(){
    printf("Welcome to COSMO !! type 'help' to see available commands.\n");

    while(1){
        char *line = read_line();
        char **args = parse_line(line);

        if (args[0] == NULL){
            free(line);
            free(args);
            continue;
        }

        if (!execute_command(args)){
            break; 
            // exiting the shell when the command returns false
        }

        free(line);
        free(args);
    }

    return 0;
}