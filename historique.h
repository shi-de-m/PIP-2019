#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include "plateau.h"

/*--------------------------*/
    /* -- Structures -- */
/*--------------------------*/

/**Base structure for the history

   a stack holding a point added and the line added
*/
struct str_history_stack;
typedef struct str_history_stack* history_stack;

/**Concrete structure of history

   this holds two stacks for undo and redo   
*/
struct str_history_container;
typedef struct str_history_container* history;




/*-----------------------------------*/
    /* -- Auxiliary functions -- */
/*-----------------------------------*/

/**Allocates the memory for a stack history*/
history_stack createStack();

/**Deallocates memory for the stack history*/
void destroyStack(history_stack*);


/*-------------------------*/
    /* -- Functions -- */
/*-------------------------*/


/**Constructor for an history*/
history createHistory();

/**Destructor for the history h

   deallocates the memory for the structure
*/
void destroyHistory(history* h);

/**Adds a move to the history*/
void append(int x, int y,int l_x,int l_y, DIRECTION l_d, history h);

/**Function that undos the last move

   it is still possible to do a redo
   the function removes the last point added and the last line
   those are stored in the redo part of the history

   returns 1 if successful
   returns 0 if cannot undo anymore
*/
int undo(history h,board b);

/**Function that redos the last undoed move*

   returns 1 if successful
   returns 0 if cannot redo anymore
*/
int redo(history h,board b);

/**Adds a move to the history in the editor*/
void appendEditor(int x, int y,history h);

/**Undo in editor*/
int undoEditor(history h, board b);

/**Redo in editor*/
int redoEditor(history h ,board b);

#endif
