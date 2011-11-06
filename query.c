#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"rbtree.h"
#include"query.h"

page * rb_search_page_cache(struct inode * inode, page * node)
{
	struct rb_node * n = inode->i_rb_page_cache.rb_node;
	page * p;

	while (n)
	{
		p = rb_entry(n, page, rb_page_cache);
		if (wordcmp(&node->word,&p->word)<0)
			n = n->rb_left;
		else if (wordcmp(&node->word,&p->word)>0)
			n = n->rb_right;
		else
			return p;
	}
	return NULL;
}

static inline page * __rb_insert_page_cache(struct inode * inode,
										  page * pnode,
										  struct rb_node * node)
{
	struct rb_node ** p = &inode->i_rb_page_cache.rb_node;
	struct rb_node * parent = NULL;
	page * pg;

	while (*p)
	  {
		parent = *p;
		pg = rb_entry(parent, page, rb_page_cache);
		if (wordcmp(&pnode->word,&pg->word) < 0)
			p = &(*p)->rb_left;
		else if (wordcmp(&pnode->word,&pg->word) > 0)
			p = &(*p)->rb_right;
		else
			return pg;
	}

	rb_link_node(node, parent, p);

	return NULL;
}

page * rb_insert_page_cache(struct inode * inode, page * node)
{
	struct rb_node *n = &node->rb_page_cache;
	page * ret;
	if ((ret = __rb_insert_page_cache(inode, node, n)))
		goto out;
	rb_insert_color(n, &inode->i_rb_page_cache);
 out:
	return ret;
}

