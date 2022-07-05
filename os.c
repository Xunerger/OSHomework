#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "os.h"

# define OS_FILENAME "os.dat"
# define OS_BITSIZE (1024*1024*100)
# define PIECE_BITSIZE (1024*512)


/////////////////////////////// ����ʵ�� //////////////////////////
void CreateOS() {
    FILE* fp = fopen(OS_FILENAME, "wb");
    if (fp == NULL) {
        printf("�ļ�����ʧ��\n");
        return;
    }
    fseek(fp, OS_BITSIZE + 1, SEEK_SET);
    fputc(32, fp);
    rewind(fp);
    // �������
    int i;
    node* temphead = (node*)malloc(sizeof(node));
    for (i = 0;i < OS_BITSIZE / PIECE_BITSIZE;i++) {
        printf("i=%d\n",i);
        temphead->llink = (i - 1) % (OS_BITSIZE / PIECE_BITSIZE);
        temphead->uplink = i;
        temphead->rlink = (i + 1) % (OS_BITSIZE / PIECE_BITSIZE);
        temphead->size = PIECE_BITSIZE;
        temphead->tag = 0;
        fwrite(temphead, sizeof(node), 1, fp);
        fseek(fp, PIECE_BITSIZE - sizeof(node), SEEK_CUR);
    }
    rewind(fp);
    fclose(fp);
}


node* getNode(FILE* fp, int n) {
    node* nd = (node*)malloc(sizeof(node));
    fseek(fp, n * PIECE_BITSIZE, SEEK_SET);
    fread(nd, sizeof(node), 1, fp);
    rewind(fp);
    return nd;
}


void writeNode(FILE* fp, int n, node* nd){
    fseek(fp,n * PIECE_BITSIZE,SEEK_SET);
    int ret = fwrite(nd,sizeof(node),1,fp);
    rewind(fp);
    fflush(fp);
}


int AllocBoundTag(int* pav, int n, FILE* fp) {
    int p, f;
    int _pav = *pav;
    node* nd = getNode(fp, *pav);
    //����ڱ������̣���ǰ���п���ڴ洢�������û�����ռ� n ֵС���ڸÿ��п����Һ��ӵ������ֱ������
    for (p = _pav; nd && nd->size < n && nd->rlink != (*pav); p = nd->rlink, nd = getNode(fp, p));
    //����ѭ���������ų�pΪ�պ�pָ��Ŀ��п�����С�� n �����
    if (!nd || nd->size < n) {
        rewind(fp);
        if(!nd) printf("No free space to alloc\n");
        if(nd->size < n) printf("Too big to alloc: only %ld, need %ld\n",nd->size, n);
        printf("alloc failed\n");
        return -100;
    }
    else {
        //����pavָ���λ�ã�Ϊ�´η�����׼��
        (*pav) = nd->rlink;
        if ((*pav) == p) {
            *pav = -1;
        }
        else {
            //ȫ�������û������ӿ����ÿռ����ɾ�� p ���п�
            getNode(fp, *pav)->llink = nd->llink;
            getNode(fp, nd->llink)->rlink = *pav;
        }
        nd->tag = 1;
        writeNode(fp,_pav,nd);
        fflush(fp);
        rewind(fp);
        return p;
    }
}


int Recover(int* pav, int loc, FILE* fp) {
    int p = loc;
    node* nd = getNode(fp,p);
    if(nd->tag == 0) return p;
    nd->tag = 0;
    //���pavָ�벻���ڣ�֤�������ÿռ��Ϊ�գ���ʱ����pΪͷָ�룬�����½���˫��ѭ������
    if (*pav == -1) {
        *pav = nd->llink = nd->rlink = p;
    }
    else {
        //������p���п���뵽pavָ��Ŀ��п�����
        int _pav = *pav;
        node* nd_pav = getNode(fp,*pav);
        int q = nd_pav->llink;
        node* nd_q = getNode(fp,q);
        nd->rlink = *pav;
        nd->llink = q;
        nd_q->rlink = nd_pav->llink = p;
        *pav = p;
        writeNode(fp,_pav,nd_pav);
        writeNode(fp,q,nd_q);
    }
    writeNode(fp,p,nd);
    rewind(fp);
    return p;
}


int findFirstFreeNode(FILE* fp) {
    int i = 0;
    node temphead;
    rewind(fp);
    for (i = 0;i < OS_BITSIZE/PIECE_BITSIZE;i++) {
        fread(&temphead, sizeof(node), 1, fp);
        if (temphead.tag == 0)
            return i;
        else
            fseek(fp, PIECE_BITSIZE - sizeof(node), SEEK_CUR);
    }
    rewind(fp);
    return -1;
}


void displayNodeTag(FILE* fp){
    fflush(fp);
    int i;
    node* temp = (node*)malloc(sizeof(node));
    rewind(fp);
    for(i=0;i<OS_BITSIZE/PIECE_BITSIZE;i++){
        fread(temp,sizeof(node),1,fp);
        printf("%d ",temp->tag);
        fseek(fp,PIECE_BITSIZE-sizeof(node),SEEK_CUR);
    }
    printf("\ndisplay over.\n");
    rewind(fp);
}

