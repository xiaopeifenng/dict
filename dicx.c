#include<string.h>
#include"dicx.h"
#include"rbtree.h"

static inline void __dicx_write_head(dicx_head * h, FILE *dic)
{
  off64_t cur = ftello64(dic);
  fseeko64(dic,0LL,SEEK_SET);
  fwrite(h, sizeof(dicx_head), 1, dic);
  fseeko64(dic, cur, SEEK_SET);
}

static inline dicx_head * __dicx_get_head(dicx_head * h, FILE *dic)
{
  off64_t cur = ftello64(dic);
  if(h == NULL)
	h = (dicx_head *) malloc(sizeof(dicx_head));
  fseeko64(dic, 0LL, SEEK_SET);
  fread(h, sizeof(dicx_head), 1, dic);
  fseeko64(dic, cur, SEEK_SET);
  return h;
}

static inline dicx_data_block * __dicx_get_data_block(dicx_data_block *b, off64_t offset, FILE *dic)
{
  if(b==NULL)
	b = (dicx_data_block *) malloc(sizeof(dicx_data_block));
  fseeko64(dic, offset, SEEK_SET);
  fread(b, sizeof(dicx_data_block), 1, dic);
  return b;
}

static inline void __dicx_write_data_block(dicx_data_block *b, off64_t offset, FILE *dic)
{
  fseeko64(dic, offset, SEEK_SET);
  fwrite(b, sizeof(dicx_data_block), 1, dic);
}

int __make_dicx_data_block(struct rb_node *n,
						   int * count,
						   page words[],
						   int parent)
{
  page *p;

  if(n == NULL)
	return 0;
  if(*count>=BLOCKWORDS)
	return -1;
  p = container_of(n, page, rb_page_cache);

  memcpy(&(words[*count].word) , &(p->word), sizeof(struct word));

  if(parent == -1)
	rb_set_parent(&(words[*count].rb_page_cache),&(words[0].rb_page_cache));
  else{
	rb_set_parent(&(words[*count].rb_page_cache),&(words[parent].rb_page_cache));
	if(n == rb_parent(n)->rb_left)
	  words[parent].rb_page_cache.rb_left =
	                &(words[*count].rb_page_cache);
	else if(n == rb_parent(n)->rb_right)
	  words[parent].rb_page_cache.rb_right =
	                &(words[*count].rb_page_cache);
  }
  words[*count].rb_page_cache.rb_left = NULL;
  words[*count].rb_page_cache.rb_right = NULL;

  parent = *count;
  (*count) ++;

  if(__make_dicx_data_block(n->rb_left, count, words, parent)==-1 ||
	 __make_dicx_data_block(n->rb_right, count, words, parent)==-1)
	return -1;
  return 1;
}

dicx_data_block * make_dicx_data_block(page *p, dicx_data_block *d)
{
  int count = 0;
  int flag = 0;

  if(d == NULL){
	d = (dicx_data_block *) malloc(sizeof(dicx_data_block));
	if(d == NULL)
	  return NULL;
	flag = 1;
  }

  if(__make_dicx_data_block(&(p->rb_page_cache), &count, d->words, -1)==-1){
	if(flag == 1)
	  free(d);
	return NULL;
  }
  d->count = count;

  return d;
}

page * dicx_data_block_to_rbtree(dicx_data_block *d)
{
  int i;
  unsigned int oldoffset,newoffset,p;

  oldoffset = (unsigned int) rb_parent(&(d->words[0].rb_page_cache));
  newoffset = (unsigned int) &(d->words[0].rb_page_cache);
  rb_set_parent(&(d->words[0].rb_page_cache),NULL);
  for(i=0;i<d->count;i++){
	d->words[i];
	p = (unsigned int)rb_parent(&(d->words[i].rb_page_cache));
	p = (p==0) ? 0 : (p - oldoffset + newoffset);
	rb_set_parent(&(d->words[i].rb_page_cache),(struct rb_node *)p);

	p = (unsigned int)d->words[i].rb_page_cache.rb_left;
	p = (p==0) ? 0 : (p - oldoffset + newoffset);
	d->words[i].rb_page_cache.rb_left = (struct rb_node *)p;
	p = (unsigned int)d->words[i].rb_page_cache.rb_right;
	p = (p==0) ? 0 : (p - oldoffset + newoffset);
	d->words[i].rb_page_cache.rb_right = (struct rb_node *)p;
  }
  return & d->words[0];
}

