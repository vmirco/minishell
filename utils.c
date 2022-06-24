#include "shell_lib.h"

void prompt(){
	char cwd[PATH_MAX]; //Conterrà il PATH della cartella in cui mi trovo

	if(getcwd(cwd, sizeof(cwd)) != NULL){ //Stampo ubash (in verde), cwd (in blu) seguito infine da $
		printf("\033[1;32muBash\033[0m:");
		printf("\033[1;34m");
		printf("~%s\033[0m$ ", cwd);
		}
	else{
   	perror("getcwd() error: ");
      exit(EXIT_FAILURE); //Chiudo con errore
   	}
}

char* remove_newline(char* str){ //Rimuovo il carattere newline
	char* trim = strrchr(str, '\n');
   if(trim != NULL){
   	*trim = 0;
		}

	return str;
}

char** check_redir(char** parsed_cmd, int command_no){ //Controllo il comando per vedere se c'è una o più redirezioni
	char** final_cmd = malloc(BUF_SIZE); //Conterrà il comando senza eventuali redirezioni
	int check;
	int counter = 0;
	int in_out = 0;
	for(int i = 0; i < command_no; i++){
		char* arg = parsed_cmd[i];
		if(arg[0] == '<' || arg[0] == '>'){
			if(strlen(arg) == 1){ //Se dopo il carattere di redirezione non c'è nulla allora errore 
				printf("Error: Unspecified path for redir\n");
				free(final_cmd);
				exit(EXIT_FAILURE);
				}
			char* path = &arg[1];
			if(arg[0] == '<')
				in_out = 1;
			if(arg[0] == '>')
				in_out = 0;
			check = redirection(path, in_out);
			if(check == -1){
				free(final_cmd);
				return NULL;
				}
			continue;
			}
		final_cmd[counter] = parsed_cmd[i];
		counter++;
		}
	final_cmd[counter] = 0;
	return final_cmd;
}
