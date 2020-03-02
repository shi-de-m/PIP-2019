#include"etats.h"
#include"safeInput.h"
#include"plateau.h"
#include"interface.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void startState(context* c)
{
  printf("Vous jouez à....\n");
  c->nextcall = &waitInitState;
}

void waitInitState(context* c)
{
  printf(" random,load,editor\n");
  char in[10];
  switch(inputAlphanum(in,10))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf(" random,load,editor\n");
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &startState;
      break;
    default :
      if(!strcmp("random",in))
	{
	  c->nextcall = &initRandomState;
	}
      else if(!strcmp("load",in))
	{
	  c->nextcall = &initFileState;
	}
      else if(!strcmp("editor",in))
	{
	  c->nextcall = &initEditorState;
	}
      else c->nextcall = &waitInitState;
    }
}

void initFileState(context* c)
{
  c->m_string = malloc(sizeof(char)*100);
  switch(inputAlphanum(c->m_string,100))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf(" entrer le chemin du fichier à charger\n");
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &startState;
      break;
    default :
      switch(checkFileExists(c->m_string))
	{
	case 0:
	  printf("Impossible d'accéder au fichier !\n");
	  c->nextcall = &waitInitState;
	  break;
	case 1:
	  c->nextcall = &initFileDirectState;
	  break;
	case 2:
	  c->nextcall = &waitInitState;
	}
    }
}

void initFileDirectState(context* c)
{
  c->m_board = createBoard();
  c->m_history = createHistory();
  if(initFile(c->m_board,c->m_string))
    {
      printf("Erreur dans la création d'un plateau à partir du fichier %s\n",c->m_string);
      exit(1);
    }
  printf("Regles du jeu");/*il faut appeler la fonction help du jeu*/
  c->nextcall = &waitMoveState;
}

void initEditorState(context* c)
{
  c->m_board = createBoard();
  c->m_history = createHistory();
  c->nextcall = &waitMoveEditorState;
}

void initRandomState(context* c)
{
  printf("Rentrez la taille souhaitée du plateau :");
  switch(inputInt(&(c->m_num1),10,50))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf("entrer un nombre entre 10 et 50\n");
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &waitInitState;
      break;
    default:
      c->m_board = createBoard();
      c->m_history = createHistory();
      c->m_string = NULL;
      if(!initRandom(c->m_board,c->m_num1))
	{
	  printf("Erreur dans la création d'un plateau aléatoire de taille %d",c->m_num1);
	  exit(1);
	}
      printf("Regles du jeu");/*il faut appeler la fonction help du jeu*/
      c->nextcall = &waitMoveState;
    }
}

void waitMoveEditorState(context* c)
{
  showBoard(c->m_board);
  char in[10];
  switch(inputAlphanum(in,10))
    {
    case HELP :
      printf("add,save,redo\n");
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      undoEditor(c->m_history,c->m_board);
      c->nextcall = &waitMoveEditorState;
      break;
    default :
      if(!strcmp("add",in))
	{
	  c->nextcall = &enterXEditorState;
	}
      else if(!strcmp("save",in))
	{
	  c->nextcall = &saveBoardEditorState;
	}
      else if(!strcmp("redo",in))
	{
	  if(!redoEditor(c->m_history,c->m_board))
	    {
	      printf("Rien à redo !\n");
	    }
	  c->nextcall = &waitMoveEditorState;
	}
      else c->nextcall = &waitMoveEditorState;
    }
}

void waitMoveState(context* c)
{
  showBoard(c->m_board);
  char in[10];
  switch(inputAlphanum(in,10))
    {
    case HELP :
      printf("play,save,show,redo\n");
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      undo(c->m_history,c->m_board);
      c->nextcall = &waitMoveState;
      break;
    default :
      if(!strcmp("play",in))
	{
	  c->nextcall = &enterXState;
	}
      else if(!strcmp("save",in))
	{
	  c->nextcall = &saveBoardState;
	}
      else if(!strcmp("redo",in))
	{
	  if(!redo(c->m_history,c->m_board))
	    {
	      printf("Rien à redo !\n");
	    }
	  c->nextcall = &waitMoveState;
	}
      else if(!strcmp("show",in))
	{
	  int* mv = getPossibleMoves(c->m_board);
	  int n = mv[0];
	  int i = 1;
	  printf("Il est encore possible de jouer en :\n");
	  for(i = 1; i<n;i++)
	    {
	      printf("(%d,%d)\n",mv[(2*i)-1],mv[(2*i)]);
	      c->nextcall = &waitMoveState;
	    }

	}
      else c->nextcall = &waitMoveState;
    }
}

void enterXState(context* c)
{
  switch(inputInt(&(c->m_num1),-1,getSize(c->m_board)))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf("entrer une coordonnée x entre -1 et %d\n",getSize(c->m_board));
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &waitMoveState;
      break;
    default:
      c->nextcall = &enterYState;
      break;
    }
}


void enterYState(context* c)
{
  line* possible;
  switch(inputInt(&(c->m_num2),-1,getSize(c->m_board)))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf("entrer une coordonnée y entre -1 et %d\n",getSize(c->m_board));
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &enterXState;
      break;
    default:
      switch(isValidMove(c->m_board,c->m_num1,c->m_num2))
        {
            case 0:
	      possible = getPossibleLines(c->m_board,c->m_num1,c->m_num2);
                if(possible[1]==NULL)
                {
		  int tmpsize = getSize(c->m_board);
		  addPoint(c->m_board,c->m_num1,c->m_num2);
		  if(tmpsize == getSize(c->m_board))
		    {
		      addLine(c->m_board,getX(possible[0]),getY(possible[0]),getD(possible[0]));
		      append(c->m_num1,c->m_num2,getX(possible[0]),getY(possible[0]),getD(possible[0]),c->m_history);
		    }
		  else
		    {
		      addLine(c->m_board,getX(possible[0])+1,getY(possible[0])+1,getD(possible[0]));
		      append(c->m_num1+1,c->m_num2+1,getX(possible[0])+1,getY(possible[0])+1,getD(possible[0]),c->m_history);
		    }
		  c->m_score +=1;
		  if(isOver(c->m_board))
                    {
		      printf("Partie terminée !\n%d points ajoutés",c->m_score);
                        reset(*c);
                        c->nextcall = &startState;
                    }
		  else
                    {
		      c->nextcall = &waitMoveState;
                    }
                }
                else
                {
		  int count = 0;
		  printf("Plusieurs lignes possibles pour ce point : \n");
		  
		  while(count < MAX_LINES_ON_POINT && possible[count]!=NULL)
		    {
		      printf("X : %d Y : %d\n",getX(possible[count]),getY(possible[count]));
		      count ++;
		    }
		  c->nextcall = &resolveXState;
                }
                free(possible);
                break;
            case 1:
                printf("Impossible de dessiner une ligne sur ce point!\n");
                c->nextcall = &enterXState;
                break;
            case 2:
                printf("Il y a déjà un point ici!\n");
                c->nextcall = &enterXState;
                break;
            case 3:
                printf("Impossible d'atteindre la case!\n");
                c->nextcall = &enterXState;
                break;
        }
        break;
    }
}

void enterXEditorState(context* c)
{
  switch(inputInt(&(c->m_num1),-100,getSize(c->m_board)+100))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf("entrer une coordonnée x entre -100 et %d\n",getSize(c->m_board)+100);
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &waitMoveEditorState;
      break;
    default:
      c->nextcall = &enterYEditorState;
    }
}

void enterYEditorState(context* c)
{
  switch(inputInt(&(c->m_num2),-100,getSize(c->m_board)+100))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf("entrer une coordonnée y entre -100 et %d\n",getSize(c->m_board)+100);
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &enterXEditorState;
      break;
    default:
      addPoint(c->m_board,c->m_num1,c->m_num2);
      appendEditor(c->m_num1,c->m_num2,c->m_history);
      c->nextcall = &waitMoveEditorState;
      break;
    }
}

void saveBoardState(context* c)
{
  switch(inputAlphanum(c->m_string,100))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf("Entrer le chemin du fichier à sauvegarder,aucun avertissement n'est fait en cas d'écrasement\n");
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &waitMoveState;
      break;
    default :
      if(!saveBoard(c->m_board,c->m_string))
	{
	  printf("Echec de la sauvegarde !\n");
	}
      else printf("Sauvegarde réussie !\n");
      c->nextcall = &waitMoveState;
      break;
    }
}


void saveBoardEditorState(context* c)
{
    switch(inputAlphanum(c->m_string,100))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf("Entrer le chemin du fichier à sauvegarder,l'extension est ajouté automatiquement,aucun avertissement n'est fait en cas d'écrasement\n");
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &waitMoveEditorState;
      break;
    default :
      if(!saveBoard(c->m_board,c->m_string))
        {
            printf("Echec de la sauvegarde !\n");
        }
      else printf("Sauvegarde réussie !\n");
      c->nextcall = &waitMoveEditorState;
        break;
    }
}

void resolveXState(context* c)
{
    switch(inputInt(&(c->m_num3),-1,getSize(c->m_board)+1))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf("entrer une coordonnée x parmis celles proposées\n");
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &waitMoveState;
      break;
    default:
      c->nextcall = &resolveYState;
      break;
    }
}

void resolveYState(context* c)
{
  line* possible;
    switch(inputInt(&(c->m_num4),-1,getSize(c->m_board)+1))
    {
    case HELP :
      /*a terme voire les fonctions d'aide*/
      printf("entrer une coordonnée y parmis celles proposées");
      break;
    case QUIT:
      exit(0);
      break;
    case ABORT:
      reset(*c);
      c->nextcall = &startState;
      break;
    case RETURN :
      c->nextcall = &resolveXState;
      break;
    default:
        possible = getPossibleLines(c->m_board,c->m_num1,c->m_num2);
        int i = 0;
        while(i<MAX_LINES_ON_POINT && possible[i]!=NULL)
        {
	  if(getX(possible[i])==c->m_num3 && getY(possible[i]) == c->m_num4)
	    {
	      int tmpsize = getSize(c->m_board);
	      addPoint(c->m_board,c->m_num1,c->m_num2);
	      if(tmpsize == getSize(c->m_board))
		{
		  addLine(c->m_board,getX(possible[i]),getY(possible[i]),getD(possible[i]));
		  append(c->m_num1,c->m_num2,getX(possible[i]),getY(possible[i]),getD(possible[i]),c->m_history);
		}
	      else
		{
		  addLine(c->m_board,getX(possible[i])+1,getY(possible[i])+1,getD(possible[i]));
		  append(c->m_num1+1,c->m_num2+1,getX(possible[i])+1,getY(possible[i])+1,getD(possible[i]),c->m_history);
		}

	      c->m_score +=1;
	      showBoard(c->m_board);
	      if(isOver(c->m_board))
		{
		  printf("Partie terminée !\n%d points ajoutés",c->m_score);
		  reset(*c);
		  c->nextcall = &startState;
		  return;
		}
	      else
		{
		  c->nextcall = &waitMoveState;
		  return;
		}
	    }
	  i++;
        }
	printf("Coordonnées incorrectes !\n");
        c->nextcall = &resolveXState;
        break;
    }
}
