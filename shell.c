#include "shell_lib.h"

int main(){
	for(;;){ //Ciclo infinito
		char* COMMAND = malloc(BUF_SIZE); //Buffer input

		prompt(); //Prompt bash

		if(fgets(COMMAND, BUF_SIZE, stdin) == NULL){ //Se premo CTRL+D
			printf("\nClosing Minishell :)\n");
			free(COMMAND); //Libero la memoria allocata
			exit(EXIT_SUCCESS); //Esco dal programma
			}

		if(COMMAND[0] == '\n'){ //Se il comando corrisponde ad un ENTER (newline)
			printf("Error: No command inserted!\n"); 
			free(COMMAND); //Libero la memoria allocata
			continue; //Rifaccio inserire l'input
			}

		if(parser(COMMAND) == -1){ //Se il parser, o una funzione da lui chiamata, ha fallito
			free(COMMAND); //Libero la memoria allocata
			exit(EXIT_FAILURE); //Se fallisce esco
		}

		free(COMMAND); //Libero la memoria alla fine del ciclo
	}
}
