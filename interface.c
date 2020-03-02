#include "interface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void printChar(board b,int x, int y){
  while(isLine(b,x,y,RIGHT)){
    printf(" - ");
    x=x+2;
  }
}

typedef struct str_lien{
  int bas;
  int droite;
  int basDroite;
  int basGauche;
} lien;



lien** linkUse(board b){
  int x,y;
  lien** r = malloc(sizeof(lien*)*getSize(b));
  line* lines = getLines(b);

  
  for(int y = 0; y<getSize(b);y++){      
    r[y] = calloc(getSize(b),sizeof(lien));
  }
  
  for(int i=0;i<getLinesCount(b);i++){
    switch(getD(lines[i]))
      {
      case RIGHT:
	{
	  for(x=0;x<LINE_SIZE-1;x++)
	    {
	      r[(getX(lines[i]))+x][getY(lines[i])].droite = 1;
	    }
	  break;
	}

	
      case BOTTOM:
	{
	  for(y=0;y<LINE_SIZE-1;y++)
	    {
	      r[(getX(lines[i]))][getY(lines[i])+y].bas = 1;
	    }

	  break;
	}

	
      case BOTTOM_RIGHT:
	{
	  for(y=0;y<LINE_SIZE-1;y++)
	    {
	      r[getX(lines[i])+y][getY(lines[i])+y].basDroite = 1;
	    }
	  break;
	}
	

      case BOTTOM_LEFT:
	{
	  for(y=0;y<LINE_SIZE-1;y++)
	    {
	      r[getX(lines[i])-y/**/][getY(lines[i])+y].basGauche = 1;
	    }
	  break;
	} 	
      }
  }
  return r;
}


/**Function that displays the board currently used, the history (moves and alignments) and if asked, a list of possible moves
 */ 
void showBoard(board b){
  int x,y;
  lien** linkObj= linkUse(b);
  printf("\n    "); // Pour ne pas avoir les numéros de ligne au dessus de la première colonne
  for (int x = 0 ; x < getSize(b); x++){
      printf("%2i    ",x); // numéro colonnes
  }
  printf("\n\n");
  for (y=0;y<getSize(b);y=y+1){
    printf(" %2i ",y); // numéro lignes
    for(x=0;x<getSize(b);x=x+1){
      if (!(isPoint(b,x,y))) 
	printf(" . ");
      else
	printf(" O ");
      if (linkObj[x][y].droite /*isLine(b,x,y,RIGHT)*/)
	printf(" - ");	//printChar(b,x,y);
      else
	printf("   ");
    }
    printf("\n    ");
    if(y != getSize(b)-1)
      {
	for(x=0;x<getSize(b);x=x+1){
	  if(!(linkObj[x][y].bas))
	    printf("   ");
	  else
	    printf(" | ");
	  if(x+1<getSize(b) && linkObj[x+1][y].basGauche)
     	    {
	      if(linkObj[x][y].basDroite) printf(" X ");
	      else  printf(" / ");
	    }
	  else
	    {
	      if(linkObj[x][y].basDroite) printf(" \\ ");
	      else  printf("   ");
	    }
	  
	}
      }
    printf("\n");
  }
  /*todo : free*/
}

