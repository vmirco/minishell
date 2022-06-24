#include "shell_lib.h"

int parser(char* COMMAND){
//GESTIONE PRELIMINARE COMANDO(rimozione newline, creazione array dinamico, conta argomenti e pipe)
	COMMAND = remove_newline(COMMAND);
	
	int command_no = 0;
	int pipe_no = 0;
	char** parsed_cmd = malloc(BUF_SIZE); //Buffer contenente ogni argomento del comando (pipe compresa)
	char* token;
	char* rest = COMMAND;

	while ((token = strtok_r(rest, " ", &rest))){ //Spazio separatore della strtok_r
		if(strcmp(token,"|") == 0) //Se il token è | vuol dire che c'è una pipe, quindi aumentò il suo counter
			pipe_no++;
		parsed_cmd[command_no] = token;//Inserisco il token in parsed_cmd
		command_no++; //Aumento il counter degli argomenti totali
		}
//A questo punto il comando è suddiviso in un array di command_no elementi
//Ogni cella dell'array contiene una stringa del comando dato in input

//VERIFICA SOMMARIA SINTASSI(posizione cd e redirezioni) E TRADUZIONE VARIABILI D'AMBIENTE
	if(syntax(parsed_cmd, command_no, pipe_no) == -1){
		free(parsed_cmd); //Se fallisce libero la memoria
		return 1; //Ritorno subito -1
		}

	int control = translate(parsed_cmd, command_no); //Traduco le variabili d'ambiente
	if(control == -1){ //Non esiste? esco
		printf("Error: No such environment variable\n");
		free(parsed_cmd);
		return 1;
		}
	if(control == 1 && command_no == 1){ //E' l'unico comando? Ne stampo il valore
		printf("%s\n", parsed_cmd[0]);
		free(parsed_cmd);
		return 1;
		}

//GESTIONE SEPARATA CASI: IF->COMANDO SINGOLO - ELSE->PRESENZA PIPE
	if(pipe_no == 0){ //COMANDO SINGOLO
		if(strcmp(parsed_cmd[0],"cd") == 0){ //cOMANDO CD
			if(command_no > 2){ //Errore se ho più di 2 argomenti
				printf("Error: Too many arguments following cd\n");
				free(parsed_cmd);
				return 1;
				}
			char* path = NULL;
			if(command_no == 2) //Due argomenti, vuol dire che ho anche un path
				path = parsed_cmd[1];
			int check = cd(path); //Eseguo cd passandogli path, che sarà null se ho inserito cd e basta
			free(parsed_cmd);
			if(check == -1){
				perror("chdir Error");
				return -1;
				}
			return 1;
			}
		parsed_cmd[command_no] = 0; //Inserisco 0 alla fine del array in modo da non creare problemi alla execvpe
		int check = my_fork(parsed_cmd, command_no); //Posso passare alla mia fork
		free(parsed_cmd);
		return check;
		}
	else{ //COMANDO CON PIPE
		int check = pipe_handler(parsed_cmd, command_no, pipe_no); //cLa gestione passa al pipe_handler
		return check;
		}
}

int syntax(char** COMMAND, int command_no, int pipe_no){
	for(int i = 0; i < command_no; i++){ //Controllo se c'è un comando cd nella pipe
		if(strcmp(COMMAND[i], "cd") == 0 && pipe_no != 0){
			printf("Error: Syntax error, can't use cd with pipe\n");
			return -1;
			}
		}
	for(int i = 0; i < command_no; i++) //Controllo se c'è un comando vuoto in mezzo alle pipe 
		if(strcmp(COMMAND[i],"|") == 0)
			if(strcmp(COMMAND[i],COMMAND[i+1]) == 0){
				printf("Error: Blank space in pipe\n");
				return -1;
				}

	int command = 1;
	for(int i = 0; i < command_no; i++){ //Controllo se le redirezioni sono nella posizione giusta in una pipe
		if(strcmp(COMMAND[i], "|") == 0)
			command++;

		char* arg = COMMAND[i];
		if(arg[0] == '<' && command != 1){
			printf("Syntax error: only the first command can redirect input\n");
			return -1;
			}
		if(arg[0] == '>' && command != pipe_no+1){
			printf("Syntax error: only the last command can redirect output\n");
			return -1;
			}
		}
	return 1;
}

int redirection(char* path, int in_out){ //Effettuo la redirezione
	if(in_out == 0){
		int fd = open(path, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR); //Per l'output, se non esiste il file lo creo, in lettura e scrittura
		if(dup2(fd,1) == -1){
			perror("dup2 Error");
			return -1;
			}
		close(fd);
		return 1;
		}
	if(in_out == 1){
		int fd = open(path, O_RDONLY);
		if(dup2(fd,STDIN_FILENO) == -1){ //Per l'input, se non esiste il file allora fallisce la dup2
			perror("dup2 Error");
			return -1;
			}
		close(fd);
		return 1;
		}

	return -1;
}

int translate(char** parsed_cmd, int command_no){ //Funzione che traduce la variabile d'ambiente e la sostituisce nel buffer
	for(int i = 0; i < command_no; i++){
		char* arg = parsed_cmd[i];
		if(arg[0] == '$'){
			parsed_cmd[i] = getenv(&arg[1]);
			if(parsed_cmd[i] == NULL)
				return -1;
			return 1;
			}
		}
	return 0;
}
