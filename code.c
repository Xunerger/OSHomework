<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>

typedef enum status {false,true} status;
# define BYTE 8

//////////////////////////////// ���ݽṹ //////////////////////////
typedef struct WORD{

}WORD,*Space;


//////////////////////////////// ����ԭ�� //////////////////////////
//��������ϵͳ
FILE CreateOS();

// �ڲ���ϵͳ�д����ڴ�ڵ�����
Space* InitSpaceLink(FILE);

// �����û�����
status CMD(char*);

// �ڴ�����㷨
Space AllocBoundTag(Space*, int n);

// �ڴ�����㷨
Space Recover(Space*, Space);

// ���ܺ���
Space FootLoc(Space);


int main(){


    return 0;
}
=======
#include<stdio.h>
>>>>>>> e4effdf04279e4f46e70ca9b2d56d9b96c3a8325
