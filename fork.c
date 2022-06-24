#include "shell_lib.h"

int my_fork(char** parsed_cmd, int command_no){
   int status;
   pid_t pid;

   if ((pid = fork()) == -1){ //fork fallita
 		perror("fork Error"); 
 		return -1;
 		}
 	else if(pid == 0){ //Processo figlio
 			int check = my_exec(parsed_cmd, command_no);
 			exit(EXIT_SUCCESS);
 			return check;
 			}
 		else{ //Processo padre
 			if((pid = wait(&status)) == -1) //wait fallita
 				perror("wait Error");

			else{ //Controlli sul ritorno del figlio
 				if(WIFSIGNALED(status))
 					printf("Child process ended because of signal %d\n", WTERMSIG(status));
 				else if(!WIFEXITED(status))
 						printf("Child process did not end normally\n");
 				}
 			}

	return 1;
}
