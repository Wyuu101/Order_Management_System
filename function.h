
#define ID_BUTTON1 1001
#define ID_BUTTON2 1002
#define ID_BUTTON3 1003
#define ID_BUTTON4 1004
#define ID_BUTTON5 1005
#define ID_BUTTON6 1006
#define ID_BUTTON7 1007
#define ID_BUTTON8 1008
#define ID_BUTTON9 1009
#define ID_BUTTON10 1010
#define ID_BUTTON11 1011
#define ID_BUTTON12 1012
#define ID_BUTTON13 1013
#define ID_BUTTON14 1014
#define ID_BUTTON15 1015
#define ID_BUTTON16 1016
#define ID_BUTTON17 1017
//-----------����ΪWindows���ڰ�ťԤ����------------
#include<stdio.h>
#include<stdlib.h>
#include<string.h>	
#include<graphics.h>
#include<windows.h>
#include<conio.h>
#include<locale.h>
#include <malloc.h>
#include<time.h>	
#define ERROR 0
#define OK 1
#define TRUE 1
#define FALSE 0
#define customerNamesize 50//�޸����ֳ���
#define productNamesize 50//�޸����ֳ���


int length_tempNode = 0;//��ɾ���ڵ����ʱ�������ĳ���(��ϸ��Դ��DelData_Win����)
int Have_Loaded = 0;    //��ʶ�����ļ��Ƿ��ѱ�����(��ϸ��Դ��LoadData_Win����)
static int num_of_file = 1;//��̬ȫ�ֱ���������OutputData�����еı��
//------------ȫ�ֱ�������-------




//------------------------�ṹ������---------------------------
typedef int Status;
typedef struct ElemType
{
	long long orderNumber;                //�������
	char customerName[customerNamesize];  //�˿�����
	char productName[productNamesize];    //��Ʒ����
	double price;                         //�����۸�
}ElemType;
typedef struct L_node
{

	ElemType data;
	struct L_node* next;//����ָ��
	struct L_node* prior;
	struct L_node* Hash_table_next;//��ϣ��ָ��
	struct L_node* Hash_table_prior;
}LNode, * LinkedList;
typedef struct
{

	LinkedList Hash_table_number[10];//��ϣ����Ϊ %10 ��ȡ��λ
	long length;
	LinkedList head;
	LNode* tail;
}List;



//--------------------------------------ϵͳ��������------------------------------------------|
//                                                                                            |
//                                                                                            |
int Init(List& L);                             //------------��ʼ�����ݱ�                     |
long long LoadData(List& L, int* num_of_del);  //------------�����ļ�����                     |
long long OutputData(List L);                  //------------��������ļ�                     |
long long OverwriteData(List L);               //------------��д�ļ�����                     |
int Uprank_orderNumber(List& L);               //------------����������������                 |
int Downrank_orderNumber(List& L);             //------------�������Ž�������                 |
int Downrank_price(List& L);                   //------------���۸���������                   |
int Uprank_customerName(List& L);              //------------���۸�������                   |
int Downrank_customerName(List& L);            //------------��������������                   |
int Uprank_price(List& L);                     //------------���۸�������                   |           
LNode* Find_ordetNumber(List L, long long aim);//------------���ݶ����Ų��ҽڵ�               |
int destroy(List& L);                          //------------�������ݱ�                       |
int same_dele(List& L);                        //------------ɾ��������ͬ�����ŵ�����         |
int insert(List& L, ElemType e);               //------------��������(������)                 |
int insert_Hash(List& L, LNode* p);            //------------��������(����ϣ��)               |
int dele_node(LNode* p);                       //------------ɾ������                         |
//                                                                                            |
//--------------------------------------------------------------------------------------------|


int Init(List& L)
{
	L.head = (LinkedList)malloc(sizeof(LNode));
	if (!L.head)
	{
		printf("�����ڴ�ʧ��(Init)\n");
		return 0;
	}
	L.head->prior = NULL;
	L.head->next = NULL;
	L.length = 0;
	L.tail = L.head;
	for (int i = 0; i < 10; i++)
	{
		L.Hash_table_number[i] = (LinkedList)malloc(sizeof(LNode));
		L.Hash_table_number[i]->Hash_table_next = NULL;
	}
	return 1;
}//��ʼ��

long long LoadData(List& L, int* num_of_del)
{
	FILE* fp = fopen("\\��������ϵͳ\data.txt", "r");//ֻд�򿪷�ʽ
	if (fp == NULL)
	{
		perror("fopen error");//���printf,perror�и���ϸ�ı�����Ϣ�����ڵ���
		return -1;
	}
//----------------------------------------------���ļ�------------------------------------------

	int count = 0;
	int file_row = 0;//�ļ�����
	int flag;//����
	while (!feof(fp))
	{
		flag = fgetc(fp);
		if (flag == '\n')
			count++;
	}//ͨ��ѭ������ȡ�ļ���������ӻ�ȡ������
	if (feof(fp))
	{
		file_row = count + 1;
	}
	else if (ferror(fp))
	{
		file_row = -1;
		perror("file_row error");
		return -1;
	}//�����жϣ�ȷ���ļ�����ȫ����
	fseek(fp, 0, SEEK_SET);//���ļ�ָ�븴λ����ͷ

	//-------------------------------------------��ȡ�ļ�����------------------------------------------

	for (int i = 0; i < file_row; i++)
	{
		LNode* p = new LNode;
		fscanf(fp, "%lld %s %s %lf", &p->data.orderNumber, p->data.customerName, p->data.productName, &p->data.price);
		//������ȡ���ݣ�����������ڵ���
		p->next = NULL;
		L.length++;
		p->prior = L.tail;
		L.tail->next = p;
		L.tail = p;

		insert_Hash(L, p);//����ڵ����ϣ��
	}

	//-------------------------------------------��ȡ�ļ����ݣ�������ϵͳ��------------------------------------------

	fclose(fp);
	fp = NULL;//�ر��ļ��������ļ�ָ��
	*num_of_del = same_dele(L);//���ɾ���ظ��Ķ�����������ɾ��������
	L.length = file_row - *num_of_del;
	Have_Loaded = 1;//����ȫ�ֱ��������������Ѿ�����
	return file_row;//�����ļ�����(��������)

	//---------------------------------------------������ȡ���Ż�����------------------------------------------
}//�����ⲿ���ݣ����ⲿ�ı��ж�������


long long OutputData(List L)
{
	char buffer[50] = { 0 };
	sprintf(buffer, "Output_data%d.txt", num_of_file);//�ɶ������ļ�����������ͬ
	num_of_file++;
	//���������ļ�
	FILE* fp = fopen(buffer, "w");
	if (fp == NULL)
	{
		perror("fopen error");
		return -1;
	}
	LNode* p = new LNode;
	p = L.head->next;
	for (int i = 0; i < L.length && p; i++, p = p->next)
	{
		fprintf(fp, "%lld %s %s %.2lf\n", p->data.orderNumber, p->data.customerName, p->data.productName, p->data.price);
		//���ļ���������ӡ����
	}
	fclose(fp);
	fp = NULL;
	return L.length;
}//�����ݱ��е��������Ϊһ���µ��ļ�


long long OverwriteData(List L)
{
	FILE* fp = fopen("data.txt", "w");
	if (fp == NULL)
	{
		perror("fopen error");
		return -1;
	}
	LNode* p = new LNode;
	p = L.head->next;
	while (p)
	{
		fprintf(fp, "%lld %s %s %.2lf\n", p->data.orderNumber, p->data.customerName, p->data.productName, p->data.price);
		p = p->next;
	}


	fclose(fp);
	fp = NULL;
	printf("��д�ɹ���\n");
	return L.length;
}//���е����ݸ�д��ԭ�����ļ���

int Uprank_orderNumber(List& L)
{
	if (!L.head || L.length == 0)
	{
		printf("������--(Uprank_orderNumber)\n");
		return 0;
	}
	else if (L.length == 1)
	{
		return 1;
	}
	LNode* p = new LNode;




	for (int i = 0; i < L.length; i++)
	{
		for (p = L.head; p->next->next; p = p->next)
		{
			if (p->next->data.orderNumber > p->next->next->data.orderNumber)
			{
				if (p->next->next->next)
				{
					p->next->next->next->prior = p->next;
				}

				p->next->prior = p->next->next;
				p->next->next->prior = p;//��ǰ��

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//���

			}
		}

	}

	printf("�����������������!\n");
	return 1;
}//���ݶ��������������--ð�ݷ�

int Downrank_orderNumber(List& L)
{
	if (!L.head || L.length == 0)
	{
		printf("������--(Downrank_orderNumber)\n");
		return 0;
	}
	else if (L.length == 1)
	{
		return 1;
	}
	LNode* p = new LNode;




	for (int i = 0; i < L.length; i++)
	{
		for (p = L.head; p->next->next; p = p->next)
		{
			if (p->next->data.orderNumber < p->next->next->data.orderNumber)
			{

				if (p->next->next->next)
				{
					p->next->next->next->prior = p->next;
				}

				p->next->prior = p->next->next;
				p->next->next->prior = p;//��ǰ��

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//���

			}
		}

	}

	printf("�����Ž����������!\n");
	return 1;
}//���ݶ�����Ž�������

int Uprank_price(List& L)
{
	if (!L.head || L.length == 0)
	{
		printf("������--(Uprank_price)\n");
		return 0;
	}
	else if (L.length == 1)
	{
		return 1;
	}

	LNode* p = new LNode;
	for (int i = 0; i < L.length; i++)
	{
		for (p = L.head; p->next->next; p = p->next)
		{
			if (p->next->data.price > p->next->next->data.price)
			{
				if (p->next->next->next)
				{
					p->next->next->next->prior = p->next;
				}

				p->next->prior = p->next->next;
				p->next->next->prior = p;//��ǰ��

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//���
			}
		}
	}
	printf("�۸������������!\n");
	return 1;
}//���ݼ۸���������


int Downrank_price(List& L)
{
	if (!L.head || L.length == 0)
	{
		printf("������--(Downrank_price)\n");
		return 0;
	}
	else if (L.length == 1)
	{
		return 1;
	}
	LNode* p = new LNode;




	for (int i = 0; i < L.length; i++)
	{
		for (p = L.head; p->next->next; p = p->next)
		{
			if (p->next->data.price < p->next->next->data.price)
			{
				if (p->next->next->next)
				{
					p->next->next->next->prior = p->next;
				}

				p->next->prior = p->next->next;
				p->next->next->prior = p;//��ǰ��

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//���
			}
		}

	}

	printf("�۸����������!\n");
	return 1;
}//���ݼ۸�������

int Uprank_customerName(List& L)
{
	setlocale(LC_ALL, "");//���ñ��ػ���������strcoll�����ȽϹ���(����Ϊ���ֱȽϷ�ʽ)
	if (!L.head || L.length == 0)
	{
		printf("������--(Uprank_customerName)\n");
		return 0;
	}
	else if (L.length == 1)
	{
		return 1;
	}

	LNode* p = new LNode;
	for (int i = 0; i < L.length; i++)
	{
		for (p = L.head; p->next->next; p = p->next)
		{
			if (strcoll(p->next->data.customerName, p->next->next->data.customerName) == 1)
			{
				if (p->next->next->next)
				{
					p->next->next->next->prior = p->next;
				}

				p->next->prior = p->next->next;
				p->next->next->prior = p;//��ǰ��

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//���
			}
		}
	}
	printf("���������������!\n");
	return 1;
}//���ݿͻ��������������ֵ���������

int Downrank_customerName(List& L)
{
	setlocale(LC_ALL, "");
	if (!L.head || L.length == 0)
	{
		printf("������--(Downrank_customerName)\n");
		return 0;
	}
	else if (L.length == 1)
	{
		return 1;
	}

	LNode* p = new LNode;
	for (int i = 0; i < L.length; i++)
	{
		for (p = L.head; p->next->next; p = p->next)
		{
			if (strcoll(p->next->data.customerName, p->next->next->data.customerName) == -1)
			{
				if (p->next->next->next)
				{
					p->next->next->next->prior = p->next;
				}

				p->next->prior = p->next->next;
				p->next->next->prior = p;//��ǰ��

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//���
			}
		}
	}
	printf("���������������!\n");
}//���ݿͻ��������������ֵ併������


LNode* Find_ordetNumber(List L, long long aim)
{
	//long long aim;
	//printf("������Ҫ���ҵĶ�����:");
	//scanf("%lld", &aim);
	//printf("���ҽ��:\n");
	LNode* p = new LNode;
	int Hash_number = aim % 10;
	for (p = L.Hash_table_number[Hash_number]->Hash_table_next; p; p = p->Hash_table_next)
	{
		if (aim == p->data.orderNumber)
		{
			//printf("%lld %s %s %.2lf\n", p->data.orderNumber, p->data.customerName, p->data.productName, p->data.price);
			return p;
		}
	}
	return NULL;
}



int destroy(List& L)
{
	LNode* p = new LNode;
	LNode* q = new LNode;
	q = L.head;
	p = q->next;
	while (p)
	{

		q = p;
		p = p->next;
		free(q);
	}
	L.tail = L.head;
	L.tail->next = NULL;
	L.length = 0;

	return 1;
}

int insert(List& L, ElemType e)
{
	LNode* p = new LNode;
	//��p������
	p->data = e;

	p->next = NULL;
	p->prior = L.tail;
	L.tail->next = p;
	L.tail = p;
	L.length++;
	insert_Hash(L, p);
	return 1;
}

int same_dele(List& L)
{
	int num_of_del = 0;
	LNode* p = new LNode;

	p = L.head->next;

	while (p)
	{
		LNode* q = new LNode;
		q = Find_ordetNumber(L, p->data.orderNumber);
		if (q != p)
		{
			dele_node(q);
			num_of_del++;
		}
		p = p->next;
	}
	return num_of_del;
}

int insert_Hash(List& L, LNode* p)
{
	int Hash_number = p->data.orderNumber % 10;
	if (L.Hash_table_number[Hash_number]->Hash_table_next)
	{
		L.Hash_table_number[Hash_number]->Hash_table_next->Hash_table_prior = p;
	}
	p->Hash_table_prior = L.Hash_table_number[Hash_number];
	p->Hash_table_next = L.Hash_table_number[Hash_number]->Hash_table_next;//ǰ�巨
	L.Hash_table_number[Hash_number]->Hash_table_next = p;
	return 1;
}

int dele_node(LNode* p)
{
	if (!p) return 0;
	p->prior->next = p->next;
	if (p->next)
	{
		p->next->prior = p->prior;
	}

	p->Hash_table_prior->Hash_table_next = p->Hash_table_next;
	if (p->Hash_table_next)
		p->Hash_table_next->Hash_table_prior = p->Hash_table_prior;
	free(p);
	return 1;
}