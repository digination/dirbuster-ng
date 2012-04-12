#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include <fcntl.h>

#define NUL '\0'

typedef struct stringlist {

   char **strlist;
   int nb_strings;

} stringlist;


stringlist explode(char*,char);
stringlist explode_ex(char*,const char*);
int strlfree(stringlist *);
int setZero(char *);
int setZeroN(char *,int);
char* trim(char*);
int strippable(char c);
int stricmp(const char* s1, const char* s2);
char ** unqueue(char**,int,int);
int setnonblocking(int);

#endif
