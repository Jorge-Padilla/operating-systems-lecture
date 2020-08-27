/*Chown Syscall*/

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    uid_t uid;			//User id
    struct passwd *pwd;		//Pasword
    char *endptr;		//Reference to char

    if (argc != 3 || argv[1][0] == '\0') {
	//If there aren't enough parameters, it returns without changes
        fprintf(stderr, "%s <owner> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uid = strtol(argv[1], &endptr, 10);  //Obtaining the long value of the user

    if (*endptr != '\0') {        
	//Getting the password of the user
        pwd = getpwnam(argv[1]);
        if (pwd == NULL) {
	    //If the User doesn't exist, it returns witouth changes
            fprintf(stderr, "chown: invalid user: '%s'\n", argv[1]);
	    exit(EXIT_FAILURE);
        }

        uid = pwd->pw_uid;	//Gets the user id
    }

    //Esecuting the chown() syscall
    if (chown(argv[2], uid, -1) == -1) {
	//If there was an error, it returns without changes
        perror("chown");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
