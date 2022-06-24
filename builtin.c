#include "shell_lib.h"

int cd(char* path){
  	
	if(path == NULL){
  		chdir(getenv("HOME")); //Cd senza path, vado alla directory home
		return 1;
		}
	
	int check = chdir(path); //Cd con path
	if(check == 0)
		return 1;
	return check;
}
