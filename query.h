#ifndef _QUERY_H_
#define _QUERY_H_

#include"rbtree.h"
#include"dict.h"

struct page
{
  struct word * word;
  struct rb_node rb_page_cache;
};

struct inode{
  struct rb_root i_rb_page_cache;
};

struct page * rb_search_page_cache(struct inode * inode,
								    struct page * node);
struct page * rb_insert_page_cache(struct inode * inode,
					   			    struct page * node);

#endif   /* _QUERY_H_ */
