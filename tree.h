#ifndef __TREE_H__
#define __TREE_H__

typedef ElemType char; //��������ݵ�����

typedef TreeInfo int;

typedef struct BinTreeNode
{
	ElemType data;  
	int leftChild; 
	int rightChild;
}BinTreeNode;

#endif
