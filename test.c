#include"safeInput.h"
#include<stdio.h>
#include<stdlib.h>

int main()
{
  char* t = malloc(sizeof(char)*30);
  inputAlphanum(t,30);
  printf("%s\n",t);
  char i[100];
  printf("Entrer un chemin: ");
  inputAlphanum(i,100);
  printf("Retour : %d\n",checkFileExists(i));
  return 0;
}

