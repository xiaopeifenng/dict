#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"dict.h"

/**
 * 返回值：字符串的BKDR哈希值
 */
unsigned long BKDRhash(char *str)
{
  unsigned long seed = 131; // 31 131 1313 13131 131313 etc..
  unsigned long hash = 0;
  while (*str)
    hash = hash * seed + (*str++);
  return hash & 0x7fffffff;
}

int wordcmp(struct word *w1, struct word *w2)
{
  int i=0;

  if(w1->hashval != w2->hashval)
	return (int)w1->hashval - w2->hashval;

  for(i=0; i<WORDDATA; i++)
	if(w1->data[i] != w2->data[i])
	  return (int)w1->data[i] - w2->data[i];

  return 0;
}

inline struct word * string2word(struct word *w, char *s)
{
  unsigned int i,j;

  if(w == NULL &&
	 (w = (struct word *) malloc(sizeof(struct word))) == NULL){
	fprintf(stderr,"Memmory error\n");
	exit(2);
  }

  w->hashval = BKDRhash(s);
  for(i=0; i<WORDDATA; ++i){
	w->data[i] = 0;
	for(j=0; j<(sizeof w->data[i])-1; j++){
	  w->data[i] += (*s == '\0')? *s : (*s++);
	  w->data[i] <<= 8;
	}
	w->data[i] += (*s == '\0')? *s : (*s++);
  }
  return w;
}

struct word * fgetword(struct word *w, FILE *ifp)
{
  char str[MAXWORD];

  if(fgets(str, MAXWORD, ifp)==NULL)
	return NULL;
  return string2word(w,str);
}

void sprintword(char *s, struct word *w)
{
  int i,j;
  unsigned int c;

  for(i=0; i<WORDDATA; ++i){
	for(j=(sizeof w->data[i])-1; j>=0; --j){
	  c = (w->data[i] >> j*8) & 0xff;
	  *s = (char) c;
	  if(*s == '\0')
		return;
	  ++s;
	}
  }
  return;
}
