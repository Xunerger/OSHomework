#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "os.h"
#include "tree.h"

void writetreenode(FILE* fp,int loc, BinTreeNode tempnode[1]) {
	//�����node��Ϣд���ļ��е�ָ��λ��loc
	fseek(fp,PIECE_BITSIZE*loc+sizeof(node),SEEK_SET);
	fwrite(tempnode,sizeof(BinTreeNode),1,fp);
	rewind(fp);
	fflush(fp);
}

void readtreenode(FILE* fp, int loc, BinTreeNode tempnode[1]) {
	//���ļ��ƶ�λ��p����Ϣ����ڵ�tempnode��
	fseek(fp,loc*PIECE_BITSIZE+sizeof(node),SEEK_SET);
	fread(tempnode,sizeof(BinTreeNode),1,fp);
	rewind(fp);
}

//����
int CreateBinTree( int* pav, FILE* fp, ElemType* elems, int count) {
	if(count != 0){
		int i = 0;
		BinTreeNode tempNode[1];
		int loc=AllocBoundTag(pav,sizeof(BinTreeNode),fp);
		tempNode->data=elems[0];
		tempNode->leftChild=-80;
		tempNode->rightChild=-80;
		int root=loc;
		writetreenode(fp,loc,tempNode);
		i++;	
		while (i<count) {
			printf("i: %d\n",i);
			int loc=AllocBoundTag(pav,sizeof(BinTreeNode),fp);
			tempNode->data=elems[i];
			tempNode->leftChild=-80;
			tempNode->rightChild=-80;
			writetreenode(fp,loc,tempNode);
			int T=root;
			int Tparent=root;
			int tag=0;//ָʾ�½������һ������һ�࣬0��ʾ���ӣ�1��ʾ�Һ���
			BinTreeNode TNode[1];
			while(T!=-80) {
				readtreenode(fp,T,TNode);
				if(TNode->data>=elems[i]) {
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
			i++;
		}
		return root;
	}
	return -80;
}

//�������
void PreOrder(int root, FILE* fp) {
	BinTreeNode tempNode[1];
	if(root!=-80) {
		readtreenode(fp,root,tempNode);
		printf("%d->",tempNode->data);
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
		printf("%d->",tempNode->data);
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
		printf("%d->",tempNode->data);
	}
}

//����keyֵ���ڵĵ�һ�����
int BinTreeSearch(int root, ElemType x, FILE* fp) {
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
			printf("�ҵ���key=%d���ڵĵ�һ����㣬�ý��λ���ļ��еĵ�%d��\n",x,T);
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
	return -80; //-800��ʾ�ڶ��������Ҳ���key

}

//�ڶ������в���ֵΪkey�Ľ��
void InsertBTNode(int* root, ElemType x, int* pav, FILE* fp) {
	BinTreeNode tempNode[1];
	tempNode->data=x;
	tempNode->leftChild=-80;
	tempNode->rightChild=-80;
	int loc=AllocBoundTag(pav,sizeof(BinTreeNode),fp);
	writetreenode(fp,loc,tempNode);
	if(*root==-80) {
		*root=loc;
		return;
	}
	int tag=0;//ָʾ�½������һ������һ�࣬0��ʾ���ӣ�1��ʾ�Һ���
	int T=*root;
	int Tparent=*root;
	while(T!=-80) {
		readtreenode(fp,T,tempNode);
		if(x<=tempNode->data) {
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
		writetreenode(fp,Tparent,tempNode);
	} else {
		readtreenode(fp,Tparent,tempNode);
		tempNode->rightChild=loc;
		writetreenode(fp,Tparent,tempNode);
	}
	printf("�ɹ�����key=%d\n",x);
}


void BinTreeClear(int* root, int* pav, FILE* fp) {
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

status cmd_Tree(FILE* fp, int* pav, char* commend, TreeInfo* tempRoot){
    printf("TreeCommend:%s END, pav : %d\n",commend,*pav);
    char init[] = "init";  
    char insert[] = "insert";  
    char clear[] = "clear";  
    char display[] = "display"; 
    char cmd[128] = { 0 };
    int i = 0;

    int tempflag = 1;
    while (commend[i] != ' ' && tempflag) {
        if(commend[i]==0) tempflag = 0;
        cmd[i] = commend[i];
        i++;
    }
    if(strcmp(cmd,init)==0){
        for (; isblank(commend[i]);i++);
        int count = 0;
        ElemType data[100] = {0};
        while( isdigit(commend[i]) || isblank(commend[i])){
            for (; isblank(commend[i]);i++);
            int n = 0;
            while (isdigit(commend[i])) {
                n = 10 * n + commend[i] - 48;
                i++;
            }
            data[count] = n;
            count++;
        }
        *tempRoot = CreateBinTree(pav,fp,data,count);
        printf("Successfully initTree, TreeInfo: [ root = %d ]\n",*tempRoot);  
        return true;
    }
    else if(strcmp(cmd,insert)==0){
        for (; isblank(commend[i]);i++);
        ElemType elem = 0;
        while (isdigit(commend[i])) {
            elem = 10 * elem + commend[i] - 48;
            i++;
        }
        InsertBTNode(tempRoot,elem,pav,fp);
        printf("Successfully InsertTree [ elem = %d ], TreeInfo: [ root = %d ]\n",elem,*tempRoot);
        return true;
    }
    else if(strcmp(cmd,clear)==0){
        BinTreeClear(tempRoot,pav,fp);
		*tempRoot=-80;
		printf("�ɹ���ն�����\n");
        return true;
    }
    else if(strcmp(cmd,display)==0){
        printf("PreOrder: ");PreOrder(*tempRoot,fp);printf("\n");
		printf("InOrder: ");InOrder(*tempRoot,fp);printf("\n");
		printf("PostOrder: ");PostOrder(*tempRoot,fp);printf("\n");
        return true;
    }
    else{
        printf("Stack dosen't have this commend.\n");
        return true;
    }
}

