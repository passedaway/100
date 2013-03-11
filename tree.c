#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

/*  
typedef struct node_s{
	struct node_s *next;
	int index;
	struct node_s *subnode[10];
}node_t;

typedef struct tree_s{
	node_t *root;
}tree_t;
*/

static node_t* subnode_creat(int level)
#if 1
{
	if(level == 0)
	{
		node_t* leafnode = (node_t*)malloc(sizeof(node_t));
		if( !leafnode )
		{
			printf("Error : [%s][%d] no memory.\n", __FUNCTION__, __LINE__);
			exit(0);
		}
		memset(leafnode, 0, sizeof(node_t));
		leafnode->index = -2;
		return leafnode;
	}else{
		int i = 0;
		node_t* midnode = (node_t *)malloc(sizeof(node_t));
		if( !midnode)
		{
			printf("Error : [%s][%d] no memory.\n", __FUNCTION__, __LINE__);
			exit(0);
		}
		memset(midnode, 0, sizeof(node_t));
		for(i = 0; i < 10; i++)
		{
			midnode->index = i;
			midnode->subnode[i] = subnode_creat(level-1);
		}

		return midnode;
	}
}
#else
{
	int i = 0, j = 0;
	node_t* midnode = (node_t *)malloc(sizeof(node_t));
	if( !midnode)
	{
		printf("Error : [%s][%d] no memory.\n", __FUNCTION__, __LINE__);
		exit(0);
	}

	memset(midnode, 0, sizeof(node_t));
	for( ; i < level; i++)
	{
		for(j = 0; j < 10; j++)
		{

		}
	}
}
#endif

static void subnode_destroy(node_t *subnode)
{

}

tree_t* tree_creat(int level)
{
	int i = 0;
	tree_t *tree = (tree_t *)malloc(sizeof(tree_t));
	if( !tree )
	{
		printf("Error : [%s][%d] no memory.\n", __FUNCTION__, __LINE__);
		exit(0);
	}
	memset(tree, 0, sizeof(tree_t));
	tree->root = (node_t *)malloc(sizeof(node_t));
	if( !(tree->root) )
	{
		printf("Error :[%s][%d] no memory.\n", __FUNCTION__, __LINE__);
		exit(0);
	}
	memset(tree->root, 0, sizeof(node_t));

	tree->root->next = NULL;
	tree->root->index = -1;

#if 0
	for( i = 0; i < 10; i++)
	{
		tree->root->subnode[i] = subnode_creat(level);
	}
#else
	{
		int j = 0;
		for( j = 0; j < level; j++)
		{


		}
	}
#endif
}

void tree_destroy(tree_t *tree)
{
	if(tree)
	{
		int i = 0;
		for(i = 0; i < 10; i++)
		{

		}

	}
		


}

int tree_en(tree_t *tree, char *data)
{
	int i = 0, j = 0;
	if( data[10] < '0' || data[10] > '9' )
		return -1;

	if( tree && tree->root && tree->root->subnode )
	{
		node_t *tmp_node = 
			tree->root->subnode[data[1]-0]->subnode[data[2]-'0']
			->subnode[data[3]-0]
			->subnode[data[4]-'0']->subnode[data[5]-0]
			->subnode[data[6]-'0']->subnode[data[7]-0]
			->subnode[data[8]-'0']->subnode[data[9]-0]
			->subnode[data[10]-'0'];

		char *ret = tmp_node->data.data;

		if( ret )
			return 1;
		else
		{
			ret = data;
//			tmp_node->data.resverd = 1;
			return 0;
		}
	}

	return -1;

}


int tree_map(tree_t *tree, MAP_FUNC_t map_func)
{

}
