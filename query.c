#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"rbtree.h"
#include"query.h"

struct page * rb_search_page_cache(struct inode * inode,
						 struct page * node)
{
	struct rb_node * n = inode->i_rb_page_cache.rb_node;
	struct page * page;

	while (n)
	{
		page = rb_entry(n, struct page, rb_page_cache);
		if (wordcmp(node->word,page->word)<0)
			n = n->rb_left;
		else if (wordcmp(node->word,page->word)>0)
			n = n->rb_right;
		else
			return page;
	}
	return NULL;
}

static inline struct page * __rb_insert_page_cache(struct inode * inode,
												   struct page * pnode,
												   struct rb_node * node)
{
	struct rb_node ** p = &inode->i_rb_page_cache.rb_node;
	struct rb_node * parent = NULL;
	struct page * page;

	while (*p)
	{
		parent = *p;
		page = rb_entry(parent, struct page, rb_page_cache);

		if (wordcmp(pnode->word,page->word) < 0)
			p = &(*p)->rb_left;
		else if (wordcmp(pnode->word,page->word) > 0)
			p = &(*p)->rb_right;
		else
			return page;
	}

	rb_link_node(node, parent, p);

	return NULL;
}

struct page * rb_insert_page_cache(struct inode * inode,
						 struct page * node)
{
	struct rb_node *n = &node->rb_page_cache;
	struct page * ret;
	if ((ret = __rb_insert_page_cache(inode, node, n)))
		goto out;
	rb_insert_color(n, &inode->i_rb_page_cache);
 out:
	return ret;
}

