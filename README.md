# minishell

uBash - Unige 2020

Gruppo:
- Samuele Crea 
- Mirco Vella 
- Camillo Ballestrasse

Programma che simula una versione semplificata della bash.
Note sintassi:
- Redirezioni Input: <path
- Redirezioni Output: >path
- Pipe: cmd1 | cmd2 | cmd3
- Variabili d'ambiente: $name

Il parser si occupa di dividere il comando in sottocomandi, in base all'eventuale
presenza dalla pipe. Dopodichè può passare il comando direttamente alla my_fork, ma 
solo nel caso in cui non sia presente pipe, che provvederà a richiamare la my_exec.
In caso di pipe il comando passa al pipe_handler, che lo divide in "primo da eseguire" 
e "resto", per poi passarli alla my_pipe, che decide se richiamare la funzione
pipe_handler, in caso di pipe multipla, oppure se chiamare direttamente la my_exec.

File presenti:
- shell.c:
		MAIN
- builtin.c:
		cd(path)
- exec.c: 
		my_exec(parsed_cmd, command_no)
- fork.c:
		my_fork(parsed_cmd, command_no)
- parser.c:	
		parser(COMMAND)
		syntax(COMMAND, command_no, pipe_no)
		redirection(path, in_out)
		translate(parsed_cmd, command_no)
- pipe.c:
		pipe_handler(parsed_cmd, command_no, pipe_no)
		my_pipe(first_cmd, new_cmd, counter, dim, pipe_no)
- utils.c:
		prompt()
		check_redir(parsed_cmd, command_no)
- shell_lib.c: 
		LIBRERIA GENERALE
- makefile
