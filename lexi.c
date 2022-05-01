/*

Travis Winterton

C Shell Project for CS 321

Based on Brennan.io tutorial 

https://brennan.io/2015/01/16/write-a-shell-in-c/





*/


//Header Files

#include <sys/wait.h>
	// Waitpid() and associated macros

#include <unistd.h>
	// chdir(), fork(), exec(), pid_t
	
#include <stdlib.h>
	// malloc(), realloc(), free(), exit(), execvp(), EXIT_SUCCESS & EXIT_FAILURE
	
#include <stdio.h>
	// fprintf(), printf(), stderr, getchar(), perror()

#include <string.h>
	// strcmp(), strtok()
	

/*
 Function Declarations 
*/

int lexi_cd(char **args);
int lexi_help( char **args);
int lexi_exit(char **args);
int lexi_print(char **args);


/*
	Builtin Commands
*/
char *builtin_str[] = {
	"cd",
	"help",
	"exit",
	"print"
};

int (*builtin_func[]) (char **) = {
	&lexi_cd,
	&lexi_help,
	&lexi_exit,
	&lexi_print
};

int lexi_num_builtins() {
	return sizeof(builtin_str) / sizeof( char * );
}

/*
	Builtin function implmentations
*/

int lexi_cd(char **args)
{
	if (args[1] == NULL) 
	{
		fprintf(stderr, "lexi: Expected argument to \"cd\"n");
	}
	else
	{
		if(chdir(args[1]) != 0 )
		{
			perror("lsh");
		}
	}
	return 1;
}

int lexi_help(char **args)
{
	int i;
	printf("Hi, Welcome to Lexi!\n");
	printf("Type programs names and arguments, and hit enter.\n");
	printf("The following are built in :\n");
	
	for (i = 0; i < lexi_num_builtins(); i++) {
		printf(" %s\n", builtin_str[i]);
	}
	
	printf("use the man command for information on other programs.\n");
	return 1;
}


int lexi_exit(char **args)
{
	printf("++++++++\n");
	
	printf("GoodBye!\n");
	
	printf("++++++++\n");
	
	
	return 0;
}

int lexi_print(char **args)
{
	printf("Process ID: %d\n", getpid() );
	return 1;
	
}


int lexi_launch(char **args)
{
	pid_t pid, wpid;
	int status;
	
	pid = fork();
	if (pid == 0) {
		//child process
		if (execvp(args[0], args) == -1) {
		perror("lsh");
	}
	exit(EXIT_FAILURE);
	} else if (pid < 0 ) {
		// Error Forking
		perror("lsh");
	} else {
		//Parent Process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	
	return 1;

}

int lexi_execute(char **args)
{
	int i;
	
	if(args[0] == NULL) 
	{
		//Empty command
		return 1;
	}
	
	for(i = 0; i < lexi_num_builtins(); i++) 
	{
		if(strcmp(args[0], builtin_str[i]) == 0)
		{
			return(*builtin_func[i])(args);
		}
	}
	
	return lexi_launch(args);
	
}



#define LSH_RL_BUFSIZE 1024

char *lexi_read_line(void)
{
	int bufsize = LSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;
	
	if(!buffer)
	{
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	while(1) 
	{
		//Read in  character
		c = getchar();
		
		//If hit EOF, replace w/ null character and return
		if(c == EOF || c == '\n') 
		{
			buffer[position] = '\0';
			return buffer;
		}
		else
		{
			buffer[position] = c;
		}
		position++;
		
	// If exceeded buffer, reallocate
	if(position > 100) 
	{
		fprintf(stderr, "Lexi: Input is too long (100 Chars max). Truncating... \n");
		buffer[position] = '\0';
		return buffer;
	}
	}
}


#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lexi_split_line(char *line)
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;
	
	if(!tokens) 
	{
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL) 
	{
		tokens[position] = token;
		position++;
	
		if(position >= bufsize) {
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			
			if(!tokens) {
				fprintf(stderr, "Allocation Error\n");
				exit(EXIT_FAILURE);
			}
		}
		
		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}





void lexi_loop(void)
{
	char *line;
	char **args;
	int status;
	
	do 
	{
		printf("> ");
		line = lexi_read_line();
		args = lexi_split_line(line);
		status = lexi_execute(args);
		
		free(line);
		free(args);
		
	} while (status);
	
}


int main(int argc, char **argv)
{
	printf("++++++++++++++++++++++++\n");
	

	printf("Hello! Welcome To Lexi!\n");
	
	printf("++++++++++++++++++++++++\n");
	//load confic files,
	
	//Run loop
	
	lexi_loop();
	
	//preform shutdown/cleanup
	
	return EXIT_SUCCESS;
}
