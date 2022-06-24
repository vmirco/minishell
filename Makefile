MAIN = shell
PARSER = parser
FORK = fork
EXEC = exec
BUILTIN = builtin
PIPE = pipe
UTILS = utils
DEBUG = debug
VALGRIND = valgrind

all : $(MAIN).c $(PARSER).c $(FORK).c $(EXEC).c $(BUILTIN).c $(PIPE).c $(UTILS).c shell_lib.h
	gcc -std=gnu11 -g *.c -o minishell

$(DEBUG) : $(MAIN).c $(PARSER).c $(FORK).c $(EXEC).c $(BUILTIN).c $(PIPE).c $(UTILS).c shell_lib.h
	gcc -std=gnu11 -g -fsanitize=address -Wall -Werror -pedantic *.c -o debug
	
$(VALGRIND) : $(MAIN).c $(PARSER).c $(FORK).c $(EXEC).c $(BUILTIN).c $(PIPE).c $(UTILS).c shell_lib.h
	gcc -std=gnu11 -g -Wall -Werror -pedantic *.c -o debug
	valgrind --leak-check=full ./debug

clean:
	rm -f minishell
	rm -f debug













