#include "shell_lib.h"

int my_exec(char** parsed_cmd, int command_no){
	if(parsed_cmd[0] == NULL) //Se non c'è il comando esco subito
		return 1;

	char** final_cmd; //Creo un'altro contenitore di comandi
	final_cmd = check_redir(parsed_cmd, command_no); //final_cmd è uguale a parsed_cmd se non c'è una redirezione

	char* base_cmd = final_cmd[0];
	int check = execvpe(base_cmd, final_cmd, NULL); //Con execvpe non serve passare /bin/cmd, basta semplicemente cmd
	free(final_cmd); //Libero buffer nuovo dopo l'execvpe
	free(parsed_cmd); //Libero il vecchio buffer
	if(check == -1){
		perror("execvpe Error");
		return -1;
		}
	return 1;
}
