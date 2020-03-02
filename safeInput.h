#ifndef H_SAFE_INPUT
#define H_SAFE_INPUT

/**Enum for the standard commands
   
   UNKNOWN is returned for undefined commands
*/
typedef enum {HELP,QUIT,ABORT,RETURN,UNKNOWN} COMMAND;

/**Checks wether the input string contains a command

   returns :
        HELP for /h
	QUIT for /q
	ABORT for /a
	RETURN for /r
	UNKNOWN otherwise
*/
COMMAND parser(char* s);

/**Idiot-proof input for a int

   the user must enter an integer between min and max
   the integer is stored inside n
*/
COMMAND inputInt(int* n,int min,int max);

/**Safe input for file names

   Will only accept [a-zA-Z0-9._-]
   this uses readline and thus allows for auto-completion
   only a limited set of caracters are allowed though, to simplify
   in-app handling(only ascii) and allow some portability
*/
COMMAND inputAlphanum(char* s,int size);

/**Checks wether the file s exists and is a .mor file

   returns 1 if file exits
           0 if file doesn't exist
	   2 if wrong extension
   Does not checks wether the caracters are correct
   Expects the input to have passed the  inputAlphanum checks
   It is possible that the file exists but is innaccessible, this is alright
   since we can't open it, writing must still be checked though
*/
int checkFileExists(char *s);

#endif
