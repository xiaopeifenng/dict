#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<time.h>

#include"rbtree.h"
#include"query.h"
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
  return hash;
}

int main(int argc, char *argv[])
{
  long starttime = clock(), endtime;
  long count = 0,savedword = 0;
#define MAXWORD 30
  char word[MAXWORD];
  FILE *ifp,*ifp2,*ofp;

  struct inode root = {.i_rb_page_cache = {NULL}};
  struct page * p,tmp = {.word = word};

  if(argc != 4){
	fprintf(stderr, "%s : Usege: %s input input2 output",argv[0],argv[0]);
	exit(1);
  }

  if((ifp=fopen(argv[1],"r")) == NULL){
	fprintf(stderr, "%s : can not open file %s\n",argv[0], argv[1]);
	exit(1);
  }
  if((ifp2=fopen(argv[2],"r")) == NULL){
	fprintf(stderr, "%s : can not open file %s\n",argv[0], argv[2]);
	exit(1);
  }
  if((ofp=fopen(argv[3],"w")) == NULL){
	fprintf(stderr, "%s : can not open file %s\n",argv[0], argv[3]);
	exit(1);
  }

  while(fgets(word,MAXWORD,ifp)!=NULL){
	tmp.offset = BKDRhash(word);
	if(rb_search_page_cache(&root, &tmp)==NULL){
	  p = (struct page *) malloc(sizeof(struct page));
	  if(p == NULL){
		fprintf(stderr, "%s : Memory error.\n",argv[0]);
		exit(2);
	  }
	  p->word = (char *) malloc((strlen(word)+1)*sizeof(char));
	  if(p->word == NULL){
		fprintf(stderr, "%s : Memory error.\n",argv[0]);
		exit(2);
	  }
	  p->offset = tmp.offset;
	  strcpy(p->word, word);
	  rb_insert_page_cache(&root, p);
	  count ++;
	}
  }
  fclose(ifp);

  endtime = clock();
  printf("Loaded %ld words.\n\
     Time used : %ld ms. Speed : %f K words/s\n",
		 count,
		 endtime-starttime, (double)count/(endtime-starttime));

  printf("Runing ...     ");
  starttime = clock();
  count = 0;
  while(fgets(word,MAXWORD,ifp2)!=NULL){
	tmp.offset = BKDRhash(word);
	if(rb_search_page_cache(&root, &tmp)==NULL){
	  fputs(word,ofp);
	  savedword ++;
	}
	count ++;
  }
  endtime = clock();
  printf("Done!\n");
  printf("Compared %ld words.\nSaved %ld words.\n\
     Time used : %ld ms. Speed : %f K words/s\n",
		 count, savedword,
		 endtime-starttime, (double)count/(endtime-starttime));
  fclose(ifp2);
  fclose(ofp);
  return 0;
}

