/*
 * =====================================================================================
 *
 *       Filename:  tree.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年06月14日 13时55分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhao, Changqing ,changqing.1230@163.com
 *        Company:  NULL
 *
 * =====================================================================================
 */
#ifndef TREE_H
#define TREE_H

typedef struct node_s{
	struct node_s *next;
	int index;
	struct{
		char *data;
//		int resverd;
	}data;
	struct node_s *subnode[10];
}node_t;

typedef struct tree_s{
	node_t *root;
}tree_t;

tree_t* tree_creat(int level);
void tree_destroy(tree_t *);

int tree_en(tree_t *, char *);

typedef int (*MAP_FUNC_t)(tree_t*, unsigned int *data);
int tree_map(tree_t *, MAP_FUNC_t map_func);

#endif
