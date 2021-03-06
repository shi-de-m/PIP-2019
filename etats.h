#ifndef ETATS_H
#define ETATS_H

#include"contexte.h"

/**State function for the start of the game

This function prints the greeting 
*/
void startState(context* c);

/**State function for intial input

Prints the possible options
expects an input
the player can choose between :
    -load : to open a file and play
    -random : to play a random board
    -editor : to open the editor

*/
void waitInitState(context* c);

/**State function for the initialisation of the board from a file

This function expects an input
the player must enter a file name
The board is then initialised from the file
*/
void initFileState(context* c);

/**State function for the initialisation of the board from a file

This function expects the input to have already happened
The board is initialised from the file
*/
void initFileDirectState(context* c);

/**State function for launching the editor

This function initialise a 5*5 empty board
*/
void initEditorState(context* c);

/**State function for the initialisation of the board randomly

This function expects an input
the player must enter a board size
The board is then initialised randomly
*/
void initRandomState(context* c);

void waitMoveEditorState(context* c);

/**State function for playing a move

Prints the possible options
expects an input
   -play or p,to enter a move
   -save, to save the map
   -show, to show the possible moves
*/
void waitMoveState(context* c);

/**State function for entering the first coordinate in a move

expects an input
proper checks are made after the second coordinate is entered 
*/
void enterXState(context* c);

/**State function for entering the second coordinate in a move

expects an input 
makes move checks
*/
void enterYState(context* c);

void enterXEditorState(context* c);

void enterYEditorState(context* c);

void saveBoardEditorState(context* c);

void saveBoardState(context* c);

void resolveXState(context* c);

void resolveYState(context* c);

#endif
