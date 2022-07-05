#ifndef _OS_H_
#define _OS_H_

# define OS_FILENAME "os.dat"
# define OS_BITSIZE (1024*1024*100)
# define PIECE_BITSIZE (1024*512)

//////////////////////////////// ���ݽṹ //////////////////////////
typedef struct WORD {
    int llink;
    int uplink;
    int tag;//�����,0��ʾΪ���п飻1��ʾΪռ�ÿ�
    int size;//��¼�ڴ��Ĵ洢��С
    int rlink;
}node;
typedef enum status{false,true} status; 

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