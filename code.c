#include <stdio.h>
#include <stdlib.h>

typedef enum status {false,true} status;
# define BYTE 8

//////////////////////////////// ���ݽṹ //////////////////////////
typedef struct WORD{

}WORD,*Space;


//////////////////////////////// ����ԭ�� //////////////////////////
//��������ϵͳ
FILE* CreateOS();

// �ڲ���ϵͳ�д����ڴ�ڵ�����
Space* InitSpaceLink(FILE*);

// �����û�����
status CMD(char*);

// �ڴ�����㷨
Space AllocBoundTag(Space*, int n);

// �ڴ�����㷨
Space Recover(Space*, Space);

// ���ܺ���
Space FootLoc(Space);

/////////////////////////////// ����ʵ�� //////////////////////////
FILE* CreateOS(){
    FILE* fp = fopen("os.dat","wb+");
    if(fp==NULL){
        printf("�ļ�����ʧ��\n");
        return NULL;
    } else{
        fseek(fp, 100*100*1024-1,SEEK_SET);
        fputc(621,fp);
        fclose(fp);
        return fp;
    }
}

int main(){
    CreateOS();
    printf("�����ɹ�\n");
    return 0;
}
