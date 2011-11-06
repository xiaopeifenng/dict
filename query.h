#ifndef _QUERY_H_
#define _QUERY_H_

#include"rbtree.h"
#include"dict.h"

typedef struct page
{
  word word;
  struct rb_node rb_page_cache;
} page;

struct inode{
  struct rb_root i_rb_page_cache;
};

page * rb_search_page_cache(struct inode * inode, page * node);
page * rb_insert_page_cache(struct inode * inode, page * node);

#endif   /* _QUERY_H_ */
