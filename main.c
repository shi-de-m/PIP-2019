#include <stdio.h>
#include <stdlib.h>

#include "plateau.h"
#include "historique.h"
#include "contexte.h"
#include "safeInput.h"
#include "etats.h"

int main(int argc, char** argv)
{
  /*Le contexte représente l'état du programme et contient les variables que doivent se passer les fonctions, les fonctions d'états lisent et modifient ces variables pour s'échanger des informations*/
  context m_context;
  if(argc == 1) m_context.nextcall = startState;
  else
    {
      if(argc > 2)
	{
	  fprintf(stderr,"%s : usage '%s' ou '%s filename.mor', terminaison... \n",argv[0],argv[0],argv[0]);
	  exit(1);
	}
      else
	{
	  if(checkFileExists(argv[1]))
	    {
	      m_context.m_string = argv[1];
	      m_context.nextcall = initFileDirectState;
	    }
	  else
	    {
	      fprintf(stderr,"%s : '%s' fichier non-reconnu, terminaison...\n",argv[0],argv[1]);
	      exit(1);
	    }
	}
    }
  /*A ce stade m_context n'est pas totalement initialisé mais 
    les transitions de la machine d'état garantissent qu'elle le sera à temps*/

  /*Cette boucle a pour vocation d'être terminée par un "exit" dans une des fonctions d'états*/
  while(42==42)
    {
      (*m_context.nextcall)(&m_context);
    }
  return 0;
}
