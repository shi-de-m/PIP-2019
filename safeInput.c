#include"safeInput.h"
#include"stdlib.h"
#include<stdio.h>

COMMAND parser(char* s)
{
  if(s==NULL || s[0] == 0 || s[1] == 0 || s[2] != 10 ) return UNKNOWN;
  switch(s[1])
    {
    case 104:
      return HELP;
    case 113:
      return QUIT;
    case 97:
      return ABORT;
    case 114:
      return RETURN;
    }
  return UNKNOWN;
}

COMMAND inputInt(int* n,int min,int max)
{
  /*Ici on stocke la chaine entrée : au maximum dix chiffres, un caractère de fin de ligne et un caractère de fin de chaine*/
  char buf[12]={0,0,0,0,0,0,0,0,0,0,0};
  int i = 0;
  int t = 1;
  int ne = 0;
  int processed;
  COMMAND re = UNKNOWN;
  while(t)
    {
      t = 0;
      if(fgets(buf,11,stdin)==NULL)
	{
	  printf("Erreur de lecture d'entier!\n");
	  t = 1;
	  continue;
	}
      /*Si on détecte une commande on la remonte*/
      if((re = parser(buf))!=UNKNOWN) return re;

      
      /*On teste si l'utilisateur a juste sauté une ligne ou si il n'a rien rentré*/
      if(buf[0] == 10 || buf[0] == 0)
	{
	  t = 1;
	  continue;
	}
      /*on teste si le premier caractère indique le signe*/
      if(buf[0] == 43) i = 1;
      else if(buf[0] == 45)
	{
	  ne = 1;
	  i = 1;
	}
      else i = 0;
      /*on vérifie la validité du nombre,  caractère par caractère...*/
      while(buf[i] != 0 && buf[i] != 10)
	{
	  if(buf[i]<48 || buf[i] > 57)
	    {
	      t = 1;
	    }
	  i++;
	}
      /*Dans le cas où on a pas lu le caractère de retour à la ligne, on essaie de vider le buffer*/
      if(buf[i] == 0)
	{
	  t=1;
	  char eraser[1000];
	  fgets(eraser,1000,stdin);
	}
      if(t!=1)
	{
	  if(ne) processed = -atoi(&buf[1]);
	  else processed = atoi(buf);
	  if(processed < min || processed > max)
	    {
	      t=1;
	      printf("Veuillez entrer un nombre entre %d et %d !\n",min,max);
	    }
	}
      else printf("Veuillez entrer un nombre entre %d et %d !\n",min,max);
    }
  *n = processed;
  return UNKNOWN;
}

/*
#include<readline/readline.h>
#include<readline/history.h>

TBD
*/
COMMAND inputAlphanum(char *s,int maxSize)
{
  /*Ici on stocke la chaine entrée : au maximum maxSize - 2  caractères, un caractère de retour à la ligne et un caractère de fin de chaine*/
  int i = 0;
  int t = 1;
  COMMAND re = UNKNOWN;
  while(t)
    {
      t = 0;
      if(fgets(s,maxSize,stdin)==NULL)
	{
	  printf("Erreur de lecture de chaine!\n");
	  t = 1;
	  continue;
	}
      /*Si on détecte une commande on la remonte*/
      if((re = parser(s))!=UNKNOWN) return re;

      
      /*On teste si l'utilisateur a juste sauté une ligne ou si il n'a rien rentré*/
      if(s[0] == 10 || s[0] == 0)
	{
	  t = 1;
	  continue;
	}
      i = 0;
      /*on vérifie la validité de la chaine,  caractère par caractère...*/
      while(s[i] != 0 && s[i] != 10)
	{
	  if(!((s[i]>=65&&s[i]<=90)||(s[i]>=97&&s[i]<=122)||(s[i]>=45&&s[i]<=57)||s[i]==126||s[i]==95||s[i]==32)&&t!=1)
	    {
	      printf("Caractère non-valide : '%c'\nSont uniquement reconnus les alpha-numériques et '/','.','_','-','~'\n",s[i]);
	      t = 1;
	    }
	  i++;
	}
      /*Dans le cas où on a pas lu le caractère de retour à la ligne, on essaie de vider le buffer*/
      if(s[i] == 0)
	{
	  if(t!=1) printf("Chaine de caractère trop longue, %d caractères autorisés au maximum\n",maxSize-2);
	  t=1;
	  char eraser[1000];
	  fgets(eraser,1000,stdin);
	}
      /*On supprime le caractère de retour à la ligne*/
      else s[i] = 0;
    }
  return UNKNOWN;
}

int checkFileExists(char *s)
{
  /*On essaie d'ouvrir le fichier en lecture*/
  FILE* file;
  if((file = fopen(s,"rb")))
    {
      fclose(file);
      return 1;
    }
  return 0;
}






