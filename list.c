#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "os.h"
#include "list.h"


void writelistnode(FILE* fp,int loc, SListNode tempnode[1]) {
	//�����node��Ϣд���ļ��е�ָ��λ��loc
	fseek(fp,PIECE_BITSIZE*loc+sizeof(node),SEEK_SET);
	fwrite(tempnode,sizeof(SListNode),1,fp);
	fflush(fp);
	rewind(fp);
}

void readlistnode(FILE* fp, int loc, SListNode tempnode[1]) {
	//���ļ��ƶ�λ��p����Ϣ����ڵ�tempnode��
	rewind(fp);
	fseek(fp,loc*PIECE_BITSIZE+sizeof(node),SEEK_SET);
	fread(tempnode,sizeof(SListNode),1,fp);
	rewind(fp);
}

void SListPrint(int phead,FILE* fp) {
	//��ʾ��������
	int cur = phead;
	SListNode tempNode[1];
	printf("List data: ");
	while (cur != -40) {
		readlistnode(fp,cur,tempNode);
		printf("%d->", tempNode->data);
		cur = tempNode->next;
	}
	printf("\n");
}

void SListPushBack(int* phead, SLTDataType x, FILE* fp, int* pav) {
	//β��
	//�����½ڵ�
	SListNode tempNode[1];
	tempNode->data=x;
	tempNode->next=-40;
	int loc=AllocBoundTag(pav,sizeof(SListNode),fp);
	writelistnode(fp, loc, tempNode);

	//�ڵ�Ϊ��ʱֱ������
	if (*phead == -40) {
		*phead = loc;
	} else {
		//��Ϊ��ʱ����β->����
		int p=*phead;
		readlistnode(fp,p,tempNode);
		while(tempNode->next!=-40) {
			p= tempNode->next;
			readlistnode(fp,p,tempNode);
		}
		printf("�ҵ�β���tail=%d\n",p);
		//����
		tempNode->next=loc;
		writelistnode(fp,p,tempNode);
		printf("�ɹ�������β����������x=%d\n",x);
	}
}

void SListPushFront(int* phead, SLTDataType x, FILE* fp, int* pav) {
	//ͷ��
	//�����½��

	SListNode tempNode[1];
	tempNode->data=x;
	tempNode->next=-40;
	int loc=AllocBoundTag(pav,sizeof(SListNode),fp);
	writelistnode(fp,loc,tempNode);
	if (*phead == -40) {
		*phead = loc;
	} else {
		readlistnode(fp,loc,tempNode);
		tempNode->next=*phead;
		*phead=loc;
		writelistnode(fp,loc,tempNode);
	}
}

void SListPopBack(int* phead, int* pav, FILE* fp) {
	//βɾ

	//����ڵ�Ϊ�գ���ô����Ҫɾ��
	if (*phead == -40) {
		return;
	} else {
		int p=*phead;
		SListNode tempNode[1];
		readlistnode(fp,p,tempNode);
		//���ͷ���Ϊ���һ����㣬�����ͷ�����ָ��Ŀռ䣬Ȼ��ͷ�����Ϊ�գ�-40����
		if(tempNode->next==-40) {
			*phead=-40;
			Recover(pav,p,fp);
		} else {
			//���ͷ���ǿգ������ҵ����һ��������ڵ����飬�������һ��������ڵ����򣬽������ڶ�������ָ���ÿգ�-40��
			int ppre=*phead;
			while(tempNode->next!=-40) {
				ppre=p;
				p=tempNode->next;
				readlistnode(fp,p,tempNode);
			}
			Recover(pav,p,fp);
			readlistnode(fp,ppre,tempNode);
			tempNode->next=-40;
			writelistnode(fp,ppre,tempNode);
		}
	}
}

void SListPopFront(int* phead,int* pav, FILE* fp) {
	//ͷɾ
	//�������Ϊ����������Ҫɾ��
	if (*phead == -40) {
		printf("������Ϊ�������޷�����ɾ������\n");
		return;
	} else {
		SListNode tempNode[1];
		readlistnode(fp, *phead, tempNode);
		Recover(pav,*phead,fp);
		*phead=tempNode->next;
		printf("ɾ��ͷ���ɹ��������µ�ͷ������\n");
	}
}

int SListFind(int* phead, SLTDataType x, FILE* fp) {
	//���ҽ��
	int p=*phead;
	SListNode tempNode[1];
	while(p!=-40) {
		readlistnode(fp,p,tempNode);
		if(tempNode->data==x)
			return p;
		else
			p=tempNode->next;
	}
	return -40;
}

void SListInsert(int* phead, int pos, SLTDataType x,int* pav,FILE* fp) {
	//��pos���ǰ����һ���µĽ��
	SListNode tempNode[1];
	tempNode->data=x;
	tempNode->next=-40;
	int loc=AllocBoundTag(pav,sizeof(SListNode),fp);
	writelistnode(fp,loc,tempNode);
	if(*phead==-40) {
		//ͷ���Ϊ��,�Ҳ���pos��㣬�޷����в������
		printf("ͷ���Ϊ�գ��Ҳ���pos��㣬�޷����в������\n");
		return;
	} else {
		int p=*phead;
		SListNode pNode[1];
		readlistnode(fp,p,pNode);
		if(*phead==pos) {
			SListPushFront(phead, x, fp, pav);
			printf("�ɹ�������x=%d������pos֮ǰ�����²���Ľ��Ϊͷ���\n",x);
		} else {
			while(pNode->next!=pos&&pNode->next!=-40) {
				p=pNode->next;
				readlistnode(fp,p,pNode);
			}
			if(pNode->next==-40) {
				printf("�������Ҳ���������pos=%d�еĽ��\n",pos);
				return;
			}
			readlistnode(fp,loc,tempNode);
			pNode->next=loc;
			tempNode->next=pos;
			writelistnode(fp,loc,tempNode);
			writelistnode(fp,p,pNode);
			printf("�ɹ�������x=%d������pos֮ǰ\n",x);
		}
	}
}

void SListTrueInsert(int* phead, int index, SLTDataType x,int* pav,FILE* fp) {
	int p = *phead;
	SListNode tempNode[1];
	if(p!=-40) readlistnode(fp,p,tempNode);
	int i;
	for(i=0;i<index;i++){
		p = tempNode->next;
		if(p==-40){
			printf("No such index.\n");
			return;
		}
		readlistnode(fp,p,tempNode);
	}
	SListInsert(phead,p,x,pav,fp);
}

void SListErase(int* phead, int pos, int* pav, FILE* fp) {
	//ɾ��pos���ǰ��Ľ��
	//�ڵ�Ϊ�ռ�����ɾ��
	if (*phead == -40) {
		printf("ͷ���Ϊ�գ��޷�����ɾ������\n");
		return;
	} else if(*phead==pos) {
		printf("pos=%dΪͷ��㣬�޷�����ɾ������\n");
		return;
	} else {
		SListNode pNode[1];
		int p=*phead;
		int ppre=*phead;
		readlistnode(fp,p,pNode);
		if(pNode->next==pos) {
			SListPopFront( phead, pav, fp);
			printf("�ɹ�ɾ��pos=%d֮ǰ�Ľ�㣬��\n",pos);
			return;
		}
		while(pNode->next!=pos&&pNode->next!=-40) {
			ppre=p;
			p=pNode->next;
			readlistnode(fp,p,pNode);
		}
		if(pNode->next==-40) {
			printf("�������Ҳ���������pos=%d�еĽ��\n",pos);
			return;
		}
		SListNode ppreNode[1];
		readlistnode(fp,ppre,ppreNode);
		ppreNode->next=pos;
		Recover(pav,p,fp);
		writelistnode(fp,ppre,ppreNode);
		printf("�ɹ�ɾ��pos=%d֮ǰ�Ľ��\n",pos);
	}
}

void SListTrueErase(int* phead, int index, int* pav, FILE* fp) {
	int p = *phead;
	SListNode tempNode[1];
	readlistnode(fp,p,tempNode);
	int i;
	for(i=0;i<index;i++){
		p = tempNode->next;
		if(p==-40){
			printf("No such index.\n");
			return;
		}
		readlistnode(fp,p,tempNode);
	}
	p = tempNode->next;
	SListErase(phead,p,pav,fp);
}


void SListInsertAfter(int* phead, int pos, SLTDataType x, int* pav, FILE* fp) {
	//��pos���������һ�����
	//ͷ���Ϊ�գ��Ҳ���pos��㣬�޷����в������
	if (*phead == -40) {
		printf("ͷ���Ϊ�գ��Ҳ���pos��㣬�޷����в������\n");
		return;
	} else {
		SListNode newNode[1];
		int loc=AllocBoundTag(pav,sizeof(SListNode),fp);
		newNode->data=x;
		SListNode posNode[1];
		readlistnode(fp,pos,posNode);
		newNode->next = posNode->next;
		posNode->next=loc;
		writelistnode(fp,loc,newNode);
		writelistnode(fp,pos,posNode);
		printf("�ɹ���pos�ڵ������µĽ��\n");
	}
}

void SListEraseAfter(int* phead, int pos, int* pav, FILE* fp) {
	// ɾ��pos�������һ�����
	//���pos���Ϊ���һ����㣬���޷�����ɾ������
	SListNode posNode[1];
	readlistnode(fp,pos,posNode);
	if(posNode->next==-40) {
		printf("posΪ���һ����㣬�޷�����ɾ������\n");
		return;
	} else {
		int next=posNode->next;
		SListNode nextNode[1];
		readlistnode(fp,next,nextNode);
		posNode->next=nextNode->next;
		Recover(pav,next,fp);
		writelistnode(fp,pos,posNode);
		printf("�ɹ�ɾ��pos�����һ�����\n");
	}
}

void SListDestory(int* phead, int* pav, FILE* fp) {
	//�ͷ�����
	int cur = *phead;
	SListNode tempNode[1];
	while (cur!=-40) {
		readlistnode(fp,cur,tempNode);
		Recover(pav,cur,fp);
		cur=tempNode->next;
	}
	*phead=-40;
	printf("�ɹ��ͷ�����\n");
}

int InitList(int* pav, FILE* fp, SLTDataType* elems, int count){
	//��ʼ������ 
	int i=0;
	int phead=-40;
	while (i<count){
		SListPushBack(&phead, elems[i], fp, pav);
		i++;
	}
	return phead; 
}


status cmd_List(FILE* fp, int* pav, char* commend, ListInfo* tempphead){
    printf("ListCommend:%s END, pav : %d\n",commend,*pav);
    char init[] = "init";  
    char insert[] = "insert";  
    char erase[] = "erase"; 
	char find[] = "find"; 
    char display[] = "display";
	char destroy[] = "destroy"; 
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
        SLTDataType data[100] = {0};
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
        *tempphead =  InitList(pav,fp,data,count);
        printf("Successfully initList, ListInfo: [ phead = %d ]\n",*tempphead);      
        return true;
    }
    else if(strcmp(cmd,insert)==0){
        for (; isblank(commend[i]);i++);
        int index = 0;
        while (isdigit(commend[i])) {
            index = 10 * index + commend[i] - 48;
            i++;
        }
        for (; isblank(commend[i]);i++);
        SLTDataType elem = 0;
        while (isdigit(commend[i])) {
            elem = 10 * elem + commend[i] - 48;
            i++;
        }
        SListTrueInsert(tempphead,index,elem,pav,fp);
        printf("Successfully insertList [ index = %d, elem = %d ], ListInfo: [ phead = %d ]\n",index,elem,*tempphead);
        return true;
    }
    else if(strcmp(cmd,erase)==0){
        for (; isblank(commend[i]);i++);
        int index = 0;
        while (isdigit(commend[i])) {
            index = 10 * index + commend[i] - 48;
            i++;
        }
        SListTrueErase(tempphead,index,pav,fp);
        printf("Successfully eraseList [ index = %d ], ListInfo: [ phead = %d ]\n",index,*tempphead);
        return true;
    }
	else if(strcmp(cmd,find)==0){
		for (; isblank(commend[i]);i++);
        SLTDataType elem = 0;
        while (isdigit(commend[i])) {
            elem = 10 * elem + commend[i] - 48;
            i++;
        }
        int loc = SListFind(tempphead,elem,fp);
        printf("Successfully findList [ elem = %d ], result: [ loc = %d ], ListInfo: [ phead = %d ]\n",elem,loc,*tempphead);
        return true;
    }
    else if(strcmp(cmd,display)==0){
        SListPrint(*tempphead,fp);
        return true;
    }
	else if(strcmp(cmd,destroy)==0){
        SListDestory(tempphead,pav,fp);
        return true;
    }
    else{
        printf("Stack dosen't have this commend.\n");
        return true;
    }
}
