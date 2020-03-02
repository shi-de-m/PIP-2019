#include"contexte.h"
#include<stdlib.h>

void reset(context c)
{
  if(c.m_string != NULL) free(c.m_string);
  destroyBoard(c.m_board);
  destroyHistory(&(c.m_history));
}
