#include "shell_lib.h"

int pipe_handler(char** parsed_cmd, int command_no, int pipe_no){
	int offset = 0;
	int counter = 0;
	for(int i = 0; i < command_no; i++){ //Conto quanti comandi ci sono prima della prima pipe
		if(strcmp(parsed_cmd[i],"|") == 0){
			offset = i;
			break;
			}
		counter++;
		}

	char** first_cmd = malloc(BUF_SIZE); //Nuovo buffer che conterrà il primo comando
	for(int j = 0; j < counter; j++)
		first_cmd[j] = parsed_cmd[j];
	pipe_no--; //Una pipe di meno
	first_cmd[counter] = 0; //Sempre 0 alla fine per l'execvpe

//A questo punto first_cmd contiene il primo comando della pipe

	int dim = command_no - counter-1;
	char** new_cmd = malloc(BUF_SIZE); //Alto buffer per il resto dell'input
	for(int n = 0; n < dim; n++){
		new_cmd[n] = parsed_cmd[offset+1];
		offset++;
		}

//A questo punto new_cmd contiene il comando di input meno il primo

	int check = my_pipe(first_cmd, new_cmd, counter, dim, pipe_no); //Passo alla mia pipe
	//LIBERO I BUFFER UTILIZZATI
	free(new_cmd);
	free(first_cmd);
	free(parsed_cmd);
	return check;
}

int my_pipe(char** first_cmd, char** new_cmd, int counter, int dim, int pipe_no){
	pid_t pid1, pid2;
   int fd[2]; //fd[0]READ - fd[1]WRITE
   if (pipe(fd) == -1){ //pipe error
   	perror("pipe Error");
   	return -1;
   	}

   pid1 = fork();
   if(pid1 < 0){ //fork error
   	perror("fork Error");
   	return -1;
   	}

   if(pid1 == 0){ //Processo figlio
		if(dup2(fd[1],1) == -1){ //dup2 error
			perror("dup2 Error");
   		return -1;
   		}
      close(fd[0]);
      close(fd[1]);
      if(my_exec(first_cmd, counter) == -1) return -1;
    	exit(EXIT_SUCCESS);
    	}
	else{ //Processo Padre
		pid2 = fork();
      if(pid2 < 0){ //fork error
   		perror("fork Error");
   		return -1;
   		}

      if(pid2 == 0){ //Processo Figlio 2
			if(dup2(fd[0],0) == -1){ //dup2 error
				perror("dup2 Error");
   			return -1;
   			}	
    		close(fd[0]);
      	close(fd[1]);
			if(pipe_no == 0){
         	if(my_exec(new_cmd, dim) == -1) return -1;
				}
			else {
				int pipe_c = pipe_handler(new_cmd, dim, pipe_no);
				if(pipe_c == -1)
					perror("pipe handler Error");
				}
			exit(EXIT_SUCCESS);
			return -1;
			}
		else{ //Processo Padre
				close(fd[0]);
				close(fd[1]);
        wait(NULL);
        wait(NULL);
      }
		}
	return 1;
}
