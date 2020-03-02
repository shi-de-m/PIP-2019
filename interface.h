#ifndef INTERFACE_H
#define INTERFACE_H

#include "historique.h"
#include "plateau.h"


/**Function that displays the alignments on the current board
 */
void alignments(board b);

  
/**Function that displays the board currently used, the history (moves and alignments) and if asked, a list of possible moves
 */ 
void showBoard(board b);


/**Function that displays help to the player

    it explains how the game work.
 */
void help();

#endif
