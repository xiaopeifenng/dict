#include"dicx.h"

void dicx_write_head(dicx_head * h, FILE *dic)
{
  off64_t cur = ftello64(dic);
  fseeko64(dic,0LL,SEEK_SET);
  fwrite(h, sizeof(dicx_head), 1, dic);
  fseeko64(dic, cur, SEEK_SET);
}

dicx_head * dicx_get_head(dicx_head * h, FILE *dic)
{
  off64_t cur = ftello64(dic);
  if(h == NULL)
	h = (dicx_head *) malloc(sizeof(dicx_head));
  fseeko64(dic, 0LL, SEEK_SET);
  fread(h, sizeof(dicx_head), 1, dic);
  fseeko64(dic, cur, SEEK_SET);
  return h;
}

dicx_block * dicx_get_block(dicx_block *b, off64_t offset, FILE *dic)
{
  if(b==NULL)
	b = (dicx_block *) malloc(sizeof(dicx_block));
  fseeko64(dic, offset, SEEK_SET);
  fread(b, sizeof(dicx_block), 1, dic);
  return b;
}

void dicx_write_block(dicx_block *b, off64_t offset, FILE *dic)
{
  fseeko64(dic, offset, SEEK_SET);
  fwrite(b, sizeof(dicx_block), 1, dic);
}

int __make_dicx_data_block(struct rb_node *n,
										 int * count,
										 page words[])
{
  page *p;

  if(count>=BLOCKWORDS)
	return -1;
  p = container_of(n, page, rb_page_cache);

  words[*count].word.hashval = p->word.hashval;
  words[*count].word.index = p->word.index;
  words[*count].word.nice = p->word.nice;
  memcpy(words[*count].word.data, p->word.data, sizeof(p->word.data));

  words[*count].rb_page_cache->rb_left = n->rb_left;
  words[*count].rb_page_cache->rb_right = n->rb_right;
  wordc[*count].rb_page_cache->rb_parent_color = n->rb_parent_color;

  *count ++;

  if(__make_dicx_data_block(n->rb_left, count, words)==-1 ||
	__make_dicx_data_block(n->rb_right, count, words)==-1)
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
  if(rb_parent(p->rb_page_cache)!=NULL)
	return NULL;
  if(__make_dicx_data_block(p->rb_page_cache, &count, d->words)==-1){
	if(flag == 1)
	  free(d);
	return NULL;
  }
  d->count = count;

  for(; count>0; count--){
	;
  }
  return d;
}

page * dicx_data_block_to_rbtree(dicx_data_block *d)
{
  int i;
  for(i=0;i<BLOCKWORDS;i++){
	d->words[i];
  }
}

