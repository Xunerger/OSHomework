#ifndef _OS_H_
#define _OS_H_
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////// ���ݽṹ //////////////////////////
typedef struct WORD {
    int llink;
    int uplink;
    int tag;//�����,0��ʾΪ���п飻1��ʾΪռ�ÿ�
    int size;//��¼�ڴ��Ĵ洢��С
    int rlink;
}node;

//////////////////////////////// ����ԭ�� //////////////////////////

//��������ϵͳ���ڴ�ڵ�����
extern void CreateOS();

// �ڴ�����㷨
extern int AllocBoundTag(int* firstFree, int, FILE*);

// �ڴ�����㷨
extern int Recover(int* firstFree, int, FILE*);

// ��������ҽڵ�
extern node* getNode(FILE*, int);

// ��ָ������д���ļ�
extern void writeNode(FILE*, int, node*);

// �ҵ���һ������ڵ�
extern int findfirstFreeNode(FILE*);

// �����ļ�����ʾ״̬
extern void displayNodeTag(FILE*);

#endif