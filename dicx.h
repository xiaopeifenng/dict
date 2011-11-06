#ifndef _DICX_H_
#define _DICX_H_

#include<stdio.h>
#include<stdlib.h>
#include"common.h"
#include"rbtree.h"
#include"query.h"

#define BLOCKWORDS 0x10000
#define MAXSPACE 1024

typedef struct dicx_head
{
  char flag[4];
  off64_t root_offset;
  off64_t data_offset;
  off64_t free_space_begin[MAXSPACE];
  off64_t free_space_end[MAXSPACE];
} dicx_head;


typedef struct dicx_data_block
{
  unsigned int hashval;
  unsigned int hashmask;
  off64_t next;
  int count;
  page words[BLOCKWORDS];
} dicx_data_block;
/*
typedef struct dicx_index_node
{
  unsigned int hashval;
  unsigned int hashmask;
  off64_t data_offset;
  off64_t leftchild, children[BLOCKWORDS];
  unsigned int count;
  dicx_data_block data;
} dicx_index_node;
*/
dicx_data_block * make_dicx_data_block(page *, dicx_data_block *);
page * dicx_data_block_to_rbtree(dicx_data_block *);

#endif /* _DICX_H_ */
