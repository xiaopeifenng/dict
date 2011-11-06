#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<time.h>

#include"common.h"
#include"rbtree.h"
#include"query.h"
#include"dict.h"
#include"dicx.h"

int main(int argc, char *argv[])
{
  long starttime = clock(), endtime;
  long count = 0,savedword = 0;
  char word[MAXWORD];
  FILE *ifp,*ifp2,*ofp;
  dicx_data_block * b;

  struct inode root = {.i_rb_page_cache = {NULL}};
  page * p,* pp,tmp;

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
  if((ofp=fopen(argv[3],"wb")) == NULL){
	fprintf(stderr, "%s : can not open file %s\n",argv[0], argv[3]);
	exit(1);
  }

  while(fgetword(&tmp.word,ifp)!=NULL){
	if(rb_search_page_cache(&root, &tmp)==NULL){
	  p = (page *) malloc(sizeof(page));
	  if(p == NULL){
		fprintf(stderr, "%s : Memory error.\n",argv[0]);
		exit(2);
	  }
	  memcpy(p, &tmp, sizeof(page));
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

  b = make_dicx_data_block(rb_entry(root.i_rb_page_cache.rb_node,page,rb_page_cache), NULL);
  fwrite(b, sizeof(dicx_data_block),1,ofp);
  /*
  printf("Runing ...     ");
  starttime = clock();
  count = 0;
  while(fgetword(&w,ifp2)!=NULL){
	if(rb_search_page_cache(&root, &tmp)==NULL){
	  sprintword(word,&w);
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
		 endtime-starttime, (double)count/(endtime-starttime));*/
  fclose(ifp2);
  fclose(ofp);
  ifp2=fopen("dic.dict","rb");
  fread(b,sizeof(dicx_data_block),1,ifp2);
  pp = dicx_data_block_to_rbtree(b);
  root.i_rb_page_cache.rb_node = & pp->rb_page_cache;

  count = 0;
  ifp=fopen("dic2.txt","r");
  while(fgetword(&tmp.word,ifp)!=NULL){
	if(rb_search_page_cache(&root, &tmp)==NULL){
	  p = (page *) malloc(sizeof(page));
	  if(p == NULL){
		fprintf(stderr, "%s : Memory error.\n",argv[0]);
		exit(2);
	  }
	  memcpy(p, &tmp, sizeof(page));
	  rb_insert_page_cache(&root, p);
	  count ++;
	}
  }
  //fclose(ifp);

  endtime = clock();
  printf("Loaded %ld words.\n\
     Time used : %ld ms. Speed : %f K words/s\n",
		 count,
		 endtime-starttime, (double)count/(endtime-starttime));

  return 0;
}

