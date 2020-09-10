/*
 *	Welcome to PShell, that means Padilla Shell
 *	this shell was made by Jorge Padilla
 *	Hi teacher! This is my shell
 *	In order to make this shell original, there will be some references
 *	that I'll explain so that the code is fully understandable
 */

//Library Pictures of the Quickening Canoe
//That means, the Shell Libraries
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>

//The world first ever pshell_loop front flip
//this is basically the definition of the loop function
void pshell_loop(void);

//The cronorium? but I've already read it
//The read function saves the written text by using the getline() function
char *readL(void);

//Hey Patrick, nice SPLIT
//This function divides the recieved text into its parameters
char **split(char *txt);

//In my imagination your running a builtin command
//run() function decides wether it runs a builtin command or a process
int run(char **args);

//The lauch() date of the series x will be on November 10th
//launch() forks and executes the processes
int launch(char **args);

//builtin functions here
int pshell_cd(char **args);
int pshell_help(char **args);
int pshell_manifesto(char **args);
int pshell_arrow(char **args);
int pshell_exit(char **args);

//Function my dear creation
//funct() is called when a created function is called
//and calls the function
int (*funct[]) (char **) = {
        &pshell_cd,
	&pshell_help,
        &pshell_manifesto,
	&pshell_arrow,
        &pshell_exit
};

//Global variable that compares the input from builtin functions
char *builts[] = {
	"cd",
	"help",
	"manifesto",
	"arrow",
	"exit"
};

//The number of builted functions
int builtsNum(void){
	return sizeof(builts) / sizeof(char *);
}

//The one and only, main function
int main(int argc, char **argv){
	//The life cycle of the program is.. well.. a cycle
	pshell_loop();

	//Lmao die
	return EXIT_SUCCESS;
}

//I'm going back to pshell_loop, If it's a 7 hour floght or a 45 minutes drive
//This loop recieves the users text, it separates it into the parameters
//and then execute its process.. but remember that before exect
//we need a fork, that will be done in run() function :P
void pshell_loop(){
	//So, the first thing its the definition of our variables
	//Pointer of the text written
	char *txt;
	//Pointer of the args value
	char **args;
	//Integer that shows the activity of the cycle
	int stat;
	//String for current dir printed in prompt
	char cwd[PATH_MAX];

	//Since this is a "menu" thingy, a do while is accurate
	do{
		//Updating the current dir for prompt
		getcwd(cwd, sizeof(cwd));
		//U r in pshell
		//I know it looks weird, but this has color formating
		printf("\033[1m\x1B[32m%s\x1B[37m@\x1B[34m%s\x1B[37m$\x1B[33mpshell>\x1B[37m \033[0m", getenv("USER"), cwd);
		//Reading the inserted line
		txt = readL();
		//Separating the line into parameters
		args = split(txt);
		//Excecuting the instruction
		stat = run(args);
		//Free memory because my friend told me to do so
		free(args);
	//Stat defines if the loop will keep living
	}while(stat);

}

//Here it is, the read function
//This function basically reads the line and stores it
//Since i had to search how to do this in C... I'm sorry if this is not optimal
char *readL(void){
	//Initialize the value in NULL
	char *line = NULL;
	//Buffer size, this has to be its own data type
	ssize_t bufferSize = 0;
	//Ok so, getline() allocates the buffer size so..
	
	//Here we receive the line and allocate it
	if(getline(&line, &bufferSize, stdin) == -1){
		//This looks for the End Of File
		if(feof(stdin)){
			exit(EXIT_SUCCESS);
		}else{
			exit(EXIT_FAILURE);
		}
	}
	
	//And.. well.. returning the line
	return line;
}

//The Split function, well, splits the text
//Again, I had to read how to do it in C
char **split(char *txt){
	//First, our variables
	//bufferSize again, and position which is a number
	int bufferSize = 64, pos = 0;
	//The tokens, that are basically the ' ' that divides parameters
	char **tokens = malloc(bufferSize * sizeof(char*));
	char *token;

	//If there are no tokens, well it dies
	if(!tokens){
		fprintf(stderr, "pshell: allocation error :(\n");
		exit(EXIT_FAILURE);
	}

	//Now we define our tokens as the separations
	token = strtok(txt, " \t\r\n\a");

	//And now we store every word separatelly
	while(token != NULL){
		//We store the token and update the position
		tokens[pos] = token;
		pos++;

		//OMG C IS EPIC WE HAVE TO REALLOCATE YAY :/
		if(pos >= bufferSize){
			bufferSize += 64;
			tokens = realloc(tokens, bufferSize * sizeof(char*));
			//If there are no tokens, well it dies
        		if(!tokens){
                		fprintf(stderr, "pshell: allocation error :(\n"); 
                	exit(EXIT_FAILURE);
			}
        	}

		//Keep going, next token
		token = strtok(NULL, " \t\r\n\a");
	}

	//NULL at the end, and returning our tokenized args
	tokens[pos] = NULL;
	return tokens;
}

//In another perfect life, we run(), we run(), we run(), RUUUUUUUUUN()
//Yeah, this function runs the typed process, that can be an existing one or one created by ya boi the dear student
int run(char **args){
	//First of all, bro you need some text around here
	if(args[0] == NULL){
		return 1;
	}

	//Comparing the input valiue with the builtin commands
	for(int i = 0; i < builtsNum(); i++){
		//Compares the command in this if with the list of builtins
		if(strcmp(args[0], builts[i]) == 0){
			//This is casting ish
			return (*funct[i])(args);
		}
	}

	//If it is a real process, it runs it
	return launch(args);
}

//Rocketlaunch() :)
//launch() runs when the texted process was, well, a real process and not a builtin
//So here we have the glorious fork() and exec() process
int launch(char **args){
	//Process ids yeye
	pid_t pid, wpid;
	int stat;

	//You already know this :)
	pid = fork();

	//R U a child?
	if(pid == 0){
		//Child process
		if(execvp(args[0], args) == -1){
			fprintf(stderr, "pshell: an error has occured in the exec process :( (maybe %s is a wrong command)\n", args[0]);
		}
		//Since an exec could occur, this line only runs if there was an error
		exit(EXIT_FAILURE);
	//Under zero is an error forking
	} else if(pid < 0){
		fprintf(stderr, "pshell: an error has occured in the fork process :(\n");
	//Parent process
	} else{
		//Basically waits
		do{
			wpid = waitpid(pid, &stat, WUNTRACED);
		} while(!WIFEXITED(stat) && !WIFSIGNALED(stat));
	}

	//All is well
	return 1;
}

//cd() or dvd()? the winner is blueray()
//I know.. you said that cd wasn't nesesary to be implemented
//But I found a nice implementation and I thought it could be good here
int pshell_cd(char **args){
	//It do be like that, requires arguments
	if(args[1] == NULL){
		fprintf(stderr, "pshell: expected argument to \"cd\"\n");
	}else{
		//Does the chdir syscall
		if(chdir(args[1]) != 0){
			fprintf(stderr, "pshell: an error occured :( (Matbe the directory does not exists)");
		}
	}
	return 1;
}

//help() i need somebody
//This prints a nice little text
int pshell_help(char **args){
	printf("Hello there, and welcome to pshell\n");
	printf("You can type any process that you want to execute\n");
	printf("Also, you can try my own built in processes:\n");

	for(int i = 0; i < builtsNum(); i++){
		printf(" --> %s\n", builts[i]);
	}

	return 1;
}

//The manifesto command prints, well just text
int pshell_manifesto(char **args){
	printf("This terminal was made for the OS Lecture\n");
	printf("This was a very fun activity, that made me do some research\n");
	printf("I'm still learning regular C, so there are new things for me\n");
	printf("Teacher, I hope you are doing great :D\n");
	printf("The main purpose of this shell was not only to get a 100\n");
	printf("But also, to learn more about how to develop a shell\n");
	printf("Many things here were done thanks to some research\n");
	printf("But at the end of the day, I wanted to put many things together\n");

}

//The arrows where basically not implemented :( it hard
int pshell_arrow(char **args){
	printf("I'm so sorry, but the arrows are not implemented :(\n");
}

//The exit() comand, well, it's trivial
int pshell_exit(char **args){
	return 0;
}
