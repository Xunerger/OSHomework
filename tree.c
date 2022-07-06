#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "os.h"
#include "tree.h"

void writetreenode(FILE* fp,int loc, BinTreeNode tempnode[1]) {
	//�����node��Ϣд���ļ��е�ָ��λ��loc
	rewind(fp);
	fseek(fp,PIECE_BITSIZE*loc+sizeof(node),SEEK_SET);
	fwrite(tempnode,sizeof(BinTreeNode),1,fp);
	fflush(fp);
	printf("�����Ϣ�ɹ�д���ļ���ָ��λ��loc=%d\n",loc);
}

void readtreenode(FILE* fp, int loc, BinTreeNode tempnode[1]) {
	//���ļ��ƶ�λ��p����Ϣ����ڵ�tempnode��
	rewind(fp);
	fseek(fp,loc*PIECE_BITSIZE+sizeof(node),SEEK_SET);
	fread(tempnode,sizeof(BinTreeNode),1,fp);
	printf("�ɹ���ȡ�ļ�ָ��λ��loc=%d����Ϣ�������\n",loc);
}

//����
int CreateBinTree( int* pav, FILE* fp) {
	printf("��������������ݣ���#������");
	ElemType x;
	scanf("%c",&x);
	int root=-50;
	BinTreeNode tempNode[1];
	if(x=='#') {
		printf("�ɹ������ն�����\n");
		return;
	} else {
		int loc=AllocBoundTag(pav,sizeof(BinTreeNode),fp);
		tempNode->data=x;
		tempNode->leftChild=-80;
		tempNode->rightChild=-80;
		root=loc;
		writetreenode(fp,loc,tempNode);
		scanf("%c",&x);
	}
	while (x!='#') {
		int loc=AllocBoundTag(pav,sizeof(BinTreeNode),fp);
		tempNode->data=x;
		tempNode->leftChild=-80;
		tempNode->rightChild=-80;
		int T=root;
		int Tparent=root;
		int tag=0;//ָʾ�½������һ������һ�࣬0��ʾ���ӣ�1��ʾ�Һ���
		BinTreeNode TNode[1];
		while(T!=-80) {
			readtreenode(fp,T,TNode);
			if(x<=TNode->data) {
				Tparent=T;
				T=TNode->leftChild;
				tag=0;
			} else {
				Tparent=T;
				T=TNode->rightChild;
				tag=1;
			}
		}
		readtreenode(fp,Tparent,TNode);
		if(tag==0)
			TNode->leftChild=loc;
		else
			TNode->rightChild=loc;
		writetreenode(fp,Tparent,TNode);
		scanf("%c",&x);
	}
	printf("�ɹ��������������������ĸ����Ϊroot=%d, ���������������Ϊ��\n",phead);
	InOrder(int root)��
	return root;
}

//�������
void PreOrder(int root, FILE* fp) {
	BinTreeNode tempNode[1];
	if(root!=-80) {
		readtreenode(fp,root,tempNode);
		printf("%c->",tempNode->data);
		int Tl=tempNode->leftChild;
		int Tr=tempNode->rightChild;
		PreOrder(Tl, fp);
		PreOrder(Tr, fp);
	}
}

//�������
void InOrder(int root, FILE* fp) {
	BinTreeNode tempNode[1];
	if(root!=-80) {
		readtreenode(fp,root,tempNode);
		int Tl=tempNode->leftChild;
		int Tr=tempNode->rightChild;
		InOrder(Tl,fp);
		printf("%c->",tempNode->data);
		InOrder(Tr,fp);
	}
}

//�������
void PostOrder(int root, FILE* fp) {
	BinTreeNode tempNode[1];
	if(root!=-80) {
		readtreenode(fp,root,tempNode);
		int Tl=tempNode->leftChild;
		int Tr=tempNode->rightChild;
		PostOrder(Tl, fp);
		PostOrder(Tr, fp);
		printf("%c->",tempNode->data);
	}
}

//����keyֵ���ڵĵ�һ�����
int BinTreeSearch(int root, FILE* fp) {
	//���rootΪ����
	if(root==-80) {
		printf("�ö�����Ϊ�������޷����ң�\n");
		return -80;
	}
	//���root��Ϊ����
	BinTreeNode tempNode[1];
	int T=root;
	while(T!=-80) {
		readtreenode(fp,root,tempNode);
		if(tempNode->data==x) {
			printf("�ҵ���key=%d���ڵĵ�һ����㣬�ý��λ���ļ��еĵ�%d��\n",key,T);
			return T;
		}
		if(x<=tempNode->data) {
			T=tempNode->leftChild;
		} else {
			T=tempNode->rightChild;
		}
	}
	//����Ҳ���key
	printf("�Ҳ���key���ڵĽ��\n");
	return -800; //-800��ʾ�ڶ��������Ҳ���key

}

//�ڶ������в���ֵΪkey�Ľ��
void InsertBTNode(int* root, ElemType x, int* pav, FILE* fp) {
	BinTreeNode tempNode[1];
	tempNode->data=x;
	tempNode->leftChild=-80;
	tempNode->rightChild=-80;
	int loc=AllocBoundTag(pav,sizeof(BinTreeNode),fp);
	writetreenode(fp,loc,tempNdoe);
	if(*root==-80) {
		*root=loc;
		return;
	}
	int tag=0;//ָʾ�½������һ������һ�࣬0��ʾ���ӣ�1��ʾ�Һ���
	int T=*root;
	int Tparent=*root;
	while(T!=-80) {
		readtreenode(fp,T,tempNode);
		,if(x<=tempNode->data) {
			tag=0;
			Tparent=T;
			T=tempNode->leftChild;
		} else {
			tag=1;
			Tparent=T;
			T=tempNode->rightChild;
		}
	}
	if(tag==-0) {
		readtreenode(fp,Tparent,tempNode);
		tempNode->leftChild=loc;
		writetreenode(fp,Tparent,tempNdoe);
	} else {
		readtreenode(fp,Tparent,tempNode);
		tempNode->rightChild=loc;
		writetreenode(fp,Tparent,tempNdoe);
	}
	printf("�ɹ�����key=%c\n",key);
}

//��ն�����
//�����ú���---��ʹ��ʱ�����ô˺�������
void BinTreeClear(int* root, int* pav, FILE* fp) {
	BinTreeClear_(root, pav, fp);
	*root=-80;
	printf("�ɹ���ն�����\n");
}
//�ڲ���ú���
void BinTreeClear_(int* root, int* pav, FILE* fp) {
	BinTreeNode tempNode[1];
	int T=*root;
	if(T!=-80) {
		readtreenode(fp,T,tempNode);
		int Tl=tempNode->leftChild;
		int Tr=tempNode->rightChild;
		BinTreeClear(&Tl, pav, fp);
		BinTreeClear(&Tr, pav, fp);
		Recover(pav,T,fp);
	}
}


