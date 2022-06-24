#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <fcntl.h>

#define PATH_MAX 100 //Valore massimo del PATH di cwd
#define BUF_SIZE 100 //Valore massimo del buffer per il comando in input

//FUNZIONI UTILI----------------------------------------------------------------------------------------------------------------------
void prompt(); //Funzione che stampa il prompt

char* remove_newline(char* str);  //Funzione che rimuove il newline alla fine del Input

char** check_redir(char** parsed_cmd, int command_no); //Controllo il comando per vedere se ci sono redirezioni

//FUNZIONI PARSING------------------------------------------------------------------------------------------------------------------------
int parser(char* COMMAND); //Funzione parser generale

int syntax(char** COMMAND, int command_no, int pipe_command); //Funzione controllo sintassi

int redirection(char* path, int in_out); //Funzione che redirige Input o Output

int translate(char** parsed_cmd, int command_no); //Funzione che traduce le variabili d'ambiente

//COMANDO BUILTIN-------------------------------------------------------------------------------------------------------------------------
int cd(char *path); //Funzione per il cambio di directory(built-in)

//FUNZIONI PER L'ESECUZIONE DI COMANDI GENERICI-------------------------------------------------------------------------------------------
int my_fork(char** parsed_cmd, int command_no); //Funzione che esegue la fork

int my_exec(char** parsed_cmd, int command_no); //Funzione che esegue il singolo comando

int pipe_handler(char** parsed_cmd, int command_no, int pipe_no); //Funzione che gestisce e prepara la pipe

int my_pipe(char** first_cmd,char** new_cmd,int counter,int dim, int pipe_no); //Funzione che esegue la pipe

