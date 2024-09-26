
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
//-----------以上为Windows窗口按钮预处理------------
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
#define customerNamesize 50//修改名字长度
#define productNamesize 50//修改名字长度


int length_tempNode = 0;//被删除节点的临时存放数组的长度(详细见源码DelData_Win函数)
int Have_Loaded = 0;    //标识数据文件是否已被加载(详细见源码LoadData_Win函数)
static int num_of_file = 1;//静态全局变量，用于OutputData函数中的标记
//------------全局变量声明-------




//------------------------结构体声明---------------------------
typedef int Status;
typedef struct ElemType
{
	long long orderNumber;                //订单编号
	char customerName[customerNamesize];  //顾客姓名
	char productName[productNamesize];    //产品名称
	double price;                         //订单价格
}ElemType;
typedef struct L_node
{

	ElemType data;
	struct L_node* next;//长链指针
	struct L_node* prior;
	struct L_node* Hash_table_next;//哈希表指针
	struct L_node* Hash_table_prior;
}LNode, * LinkedList;
typedef struct
{

	LinkedList Hash_table_number[10];//哈希函数为 %10 即取个位
	long length;
	LinkedList head;
	LNode* tail;
}List;



//--------------------------------------系统操作函数------------------------------------------|
//                                                                                            |
//                                                                                            |
int Init(List& L);                             //------------初始化数据表                     |
long long LoadData(List& L, int* num_of_del);  //------------载入文件数据                     |
long long OutputData(List L);                  //------------输出数据文件                     |
long long OverwriteData(List L);               //------------覆写文件数据                     |
int Uprank_orderNumber(List& L);               //------------按订单号升序排列                 |
int Downrank_orderNumber(List& L);             //------------按订单号降序排列                 |
int Downrank_price(List& L);                   //------------按价格升序排列                   |
int Uprank_customerName(List& L);              //------------按价格降序排列                   |
int Downrank_customerName(List& L);            //------------按姓名升序排列                   |
int Uprank_price(List& L);                     //------------按价格降序排列                   |           
LNode* Find_ordetNumber(List L, long long aim);//------------根据订单号查找节点               |
int destroy(List& L);                          //------------销毁数据表                       |
int same_dele(List& L);                        //------------删除表中相同订单号的数据         |
int insert(List& L, ElemType e);               //------------插入数据(进链表)                 |
int insert_Hash(List& L, LNode* p);            //------------插入数据(进哈希表)               |
int dele_node(LNode* p);                       //------------删除数据                         |
//                                                                                            |
//--------------------------------------------------------------------------------------------|


int Init(List& L)
{
	L.head = (LinkedList)malloc(sizeof(LNode));
	if (!L.head)
	{
		printf("申请内存失败(Init)\n");
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
}//初始化

long long LoadData(List& L, int* num_of_del)
{
	FILE* fp = fopen("\\订单管理系统\data.txt", "r");//只写打开方式
	if (fp == NULL)
	{
		perror("fopen error");//相比printf,perror有更详细的报错信息，便于调试
		return -1;
	}
//----------------------------------------------打开文件------------------------------------------

	int count = 0;
	int file_row = 0;//文件行数
	int flag;//缓存
	while (!feof(fp))
	{
		flag = fgetc(fp);
		if (flag == '\n')
			count++;
	}//通过循环，获取文件行数，间接获取订单数
	if (feof(fp))
	{
		file_row = count + 1;
	}
	else if (ferror(fp))
	{
		file_row = -1;
		perror("file_row error");
		return -1;
	}//加上判断，确保文件被完全读入
	fseek(fp, 0, SEEK_SET);//将文件指针复位到开头

	//-------------------------------------------获取文件行数------------------------------------------

	for (int i = 0; i < file_row; i++)
	{
		LNode* p = new LNode;
		fscanf(fp, "%lld %s %s %lf", &p->data.orderNumber, p->data.customerName, p->data.productName, &p->data.price);
		//逐条读取数据，并存入链表节点中
		p->next = NULL;
		L.length++;
		p->prior = L.tail;
		L.tail->next = p;
		L.tail = p;

		insert_Hash(L, p);//链表节点入哈希表
	}

	//-------------------------------------------读取文件数据，并存入系统中------------------------------------------

	fclose(fp);
	fp = NULL;//关闭文件，销毁文件指针
	*num_of_del = same_dele(L);//最后删除重复的订单，并返回删除的数量
	L.length = file_row - *num_of_del;
	Have_Loaded = 1;//设置全局变量，表明数据已经载入
	return file_row;//返回文件行数(数据条数)

	//---------------------------------------------结束读取，优化数据------------------------------------------
}//加载外部数据，从外部文本中读入数据


long long OutputData(List L)
{
	char buffer[50] = { 0 };
	sprintf(buffer, "Output_data%d.txt", num_of_file);//可多次输出文件，且命名不同
	num_of_file++;
	//有序地输出文件
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
		//在文件中逐条打印数据
	}
	fclose(fp);
	fp = NULL;
	return L.length;
}//将数据表中的数据输出为一个新的文件


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
	printf("覆写成功！\n");
	return L.length;
}//表中的数据覆写到原数据文件中

int Uprank_orderNumber(List& L)
{
	if (!L.head || L.length == 0)
	{
		printf("无链表--(Uprank_orderNumber)\n");
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
				p->next->next->prior = p;//变前置

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//变后

			}
		}

	}

	printf("订单号升序排列完毕!\n");
	return 1;
}//根据订单编号升序排列--冒泡法

int Downrank_orderNumber(List& L)
{
	if (!L.head || L.length == 0)
	{
		printf("无链表--(Downrank_orderNumber)\n");
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
				p->next->next->prior = p;//变前置

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//变后

			}
		}

	}

	printf("订单号降序排列完毕!\n");
	return 1;
}//根据订单编号降序排列

int Uprank_price(List& L)
{
	if (!L.head || L.length == 0)
	{
		printf("无链表--(Uprank_price)\n");
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
				p->next->next->prior = p;//变前置

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//变后
			}
		}
	}
	printf("价格升序排列完毕!\n");
	return 1;
}//根据价格升序排列


int Downrank_price(List& L)
{
	if (!L.head || L.length == 0)
	{
		printf("无链表--(Downrank_price)\n");
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
				p->next->next->prior = p;//变前置

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//变后
			}
		}

	}

	printf("价格降序排列完毕!\n");
	return 1;
}//根据价格降序排列

int Uprank_customerName(List& L)
{
	setlocale(LC_ALL, "");//设置本地环境，更改strcoll函数比较规则(更改为汉字比较方式)
	if (!L.head || L.length == 0)
	{
		printf("无链表--(Uprank_customerName)\n");
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
				p->next->next->prior = p;//变前置

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//变后
			}
		}
	}
	printf("姓名升序排列完毕!\n");
	return 1;
}//根据客户姓名按照中文字典升序排列

int Downrank_customerName(List& L)
{
	setlocale(LC_ALL, "");
	if (!L.head || L.length == 0)
	{
		printf("无链表--(Downrank_customerName)\n");
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
				p->next->next->prior = p;//变前置

				p->next->next = p->next->next->next;
				p->next->prior->next = p->next;
				p->next = p->next->prior;//变后
			}
		}
	}
	printf("姓名降序排列完毕!\n");
}//根据客户姓名按照中文字典降序排列


LNode* Find_ordetNumber(List L, long long aim)
{
	//long long aim;
	//printf("请输入要查找的订单号:");
	//scanf("%lld", &aim);
	//printf("查找结果:\n");
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
	//给p放数据
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
	p->Hash_table_next = L.Hash_table_number[Hash_number]->Hash_table_next;//前插法
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