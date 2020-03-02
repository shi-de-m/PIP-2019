#ifndef CONTEXTE_H
#define CONTEXTE_H

#include"plateau.h"
#include"historique.h"

struct str_context;

/**Game context for holding the memory of the state machine*/
typedef struct str_context
{
  void (*nextcall)(struct str_context *con);
  history m_history;
  board m_board;
  int m_num1;
  int m_num2;
  int m_num3;
  int m_num4;
  char* m_string;
  int m_score;
}
context;

/**Resets the context by deallocating the board, the history and the string*/
void reset(context c);

#endif
