#include "historique.h"
#include "plateau.h"
#include<stdlib.h>

    /* -- Structures -- */

struct str_history_container {
  history_stack undo_history;
  history_stack redo_history;
};

struct str_history_stack {
  int x;
  int y;
  int l_x;
  int l_y;
  DIRECTION l_d;
  history_stack next;
};

    /* -- Auxiliary functions -- */

history_stack createStack() {
  history_stack r = malloc(sizeof(struct str_history_stack));
  r->next = NULL;
  return r;
}

void destroyStack(history_stack* s)
{
  if(*s != NULL){
    if((*s)->next != NULL) destroyStack(&((*s)->next));
    free(*s);
    *s = NULL;
  }
}

    /* -- Functions -- */

history createHistory() {
  history h = malloc(sizeof(struct str_history_container));
  h->undo_history = createStack();
  h->redo_history = createStack();
  return h;
}

void destroyHistory(history* h)
{
  destroyStack(&((*h)->undo_history));
  destroyStack(&((*h)->redo_history));
  free(*h);
  *h = NULL;
}

void append(int x, int y,int l_x,int l_y,DIRECTION l_d, history h) {
  history_stack to_add = malloc(sizeof(struct str_history_stack));
  to_add->x = x;
  to_add->y = y;
  to_add->l_x = l_x;
  to_add->l_y = l_y;
  to_add->l_d = l_d;
  to_add->next = (h->undo_history);
  h->undo_history = to_add;
  destroyStack(&((h->redo_history)->next));
}

int undo(history h,board b)
{
  if((h->undo_history)->next == NULL) return 0;
  history_stack tmp = h->undo_history;
  h->undo_history = tmp->next;
  deleteLine(b,tmp->l_x,tmp->l_y,tmp->l_d);
  deletePoint(b,tmp->x,tmp->y);
  tmp->next = h->redo_history;
  h->redo_history = tmp;
  return 1;
}

int redo(history h,board b)
{
  if((h->redo_history)->next == NULL) return 0;
  history_stack tmp = h->redo_history;
  h->redo_history = tmp->next;
  addPoint(b,tmp->x,tmp->y);
  addLine(b,tmp->l_x,tmp->l_y,tmp->l_d);
  tmp->next = h->undo_history;
  h->undo_history = tmp;
  return 1;
}

void appendEditor(int x, int y,history h){
  history_stack to_add = malloc(sizeof(struct str_history_stack));
  to_add->x = x;
  to_add->y = y;
  to_add->next = h->undo_history;
  h->undo_history = to_add;
  destroyStack(&((h->redo_history)->next));
}

  
int undoEditor(history h, board b)
{
  if((h->undo_history)->next == NULL) return 0;
  history_stack tmp = h->undo_history;
  h->undo_history = tmp->next;
  deletePoint(b,tmp->x,tmp->y);
  tmp->next = h->redo_history;
  h->redo_history = tmp;
  return 1;
}

int redoEditor(history h ,board b)
{
  if((h->redo_history)->next == NULL) return 0;
  history_stack tmp = h->redo_history;
  h->redo_history = tmp->next;
  addPoint(b,tmp->x,tmp->y);
  tmp->next = h->undo_history;
  h->undo_history = tmp;
  return 1;
}
