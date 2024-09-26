#define _CRT_SECURE_NO_WARNINGS
#include"function.h"
//------------ȫ�ֱ�������-------
LNode* tempNode[200];//����ɾ���ڵ�Ļ�������
List L;              //ȫ�����ݱ�
HWND  hChildWnd;     //��������Ӵ���(��������ľ��)
   
//------------------------------------------------------����Ԥ����-------------------------------------------------------

void DrawRect(HDC hdc, int left, int top, int right, int bottom);                               //���ƾ���
void draw(HWND hwnd);                                                                           //����������
void AppendTextToEdit(const char* text);                                                        //�ڹ�������׷���ı�
void AppendTextToEdit_Clear(const char* text);                                                  //��չ��������ı�
void LoadData_Win(HWND hwnd, List& L);                                                          //�����ļ�����                           
void OutputData_Win(HWND hwnd, List& L);                                                        //����ļ�����
void OverwriteData_Win(HWND hwnd, List& L);                                                     //��д�ļ�����
int Statistic_Win(HWND hwnd, List L);                                                           //ͳ���ļ�����
void Traverse_Win(HWND hwnd, List& L);                                                          //�������ݣ����ڹ���������ʾ
void  Find_ordetNumber_Win(HWND hwnd, List L);                                                  //���ݶ����Ų�������
void Find_customerName_Win(HWND hwnd, List L);                                                  //���ݹ˿�������������
void Find_productName_Win(HWND hwnd, List L);                                                   //������Ʒ����������
void IncertData_Win(HWND hwnd, List& L);                                                        //��������
void DelData_Win(HWND hwnd, List &L);                                                           //ɾ������
void Clear_tempNode();                                                                          //���ָ�뻺������
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);                //���崦����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);//������

//------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_ERASEBKGND:
	{
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 200, 180));  // ������ɫ��ˢ
		HDC hdc = (HDC)wParam;
		RECT rect;
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, hBrush);  // ��䴰���������Ϊ��ɫ
		DeleteObject(hBrush);  // �ͷŻ�ˢ��Դ
		return TRUE;
		//--------------------��������ڱ���ɫ---------------------
	}
	case WM_PAINT:
		draw(hwnd);
		break;
		//---------------------��������������---------------------
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_BUTTON5) {
			LoadData_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON1) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]δ��������,�޷������");
			}
			else
				OutputData_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON7) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]δ��������,�޷���д��");
			}
			else
				OverwriteData_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON2) {
			Statistic_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON3) {
			Traverse_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON6) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]δ��������,�޷�����");
			}
			else
			{
				Downrank_orderNumber(L);
				AppendTextToEdit("\r\n[system]����������������ϣ�");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON4) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]δ��������,�޷�����");
			}
			else
			{
				Uprank_orderNumber(L);
				AppendTextToEdit("\r\n[system]����������������ϣ�");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON8) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]δ��������,�޷�����");
			}
			else
			{
				Uprank_price(L);
				AppendTextToEdit("\r\n[system]���۸�����������ϣ�");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON9) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]δ��������,�޷�����");
			}
			else
			{
				Downrank_price(L);
				AppendTextToEdit("\r\n[system]���۸���������ϣ�");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON10) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]δ��������,�޷�����");
			}
			else
			{
				Uprank_customerName(L);
				AppendTextToEdit("\r\n[system]����������������ϣ�");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON11) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]δ��������,�޷�����");
			}
			else
			{
				Downrank_customerName(L);
				AppendTextToEdit("\r\n[system]����������������ϣ�");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON12) {
			Find_ordetNumber_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON13) {
			Find_customerName_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON14) {
			Find_productName_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON15) {
			SendMessage(hChildWnd, WM_SETTEXT, 0, (LPARAM)TEXT(""));
		}
		if (LOWORD(wParam) == ID_BUTTON16) {
			DelData_Win(hwnd, L);
			AppendTextToEdit("[system]Ŀ�������Ѿ���ɾ��!");
		}
		if (LOWORD(wParam) == ID_BUTTON17) {
			IncertData_Win(hwnd, L);
		}

		break;
		//-----------------------��Ӧ����--------------------
	case WM_CLOSE: //�ر�
		if (IDYES == MessageBox(hwnd, "�Ƿ��˳�������ϵͳ?", "��ʾ", MB_YESNO | MB_ICONQUESTION))
		{
			DestroyWindow(hwnd);
		}
		PostQuitMessage(0);
		destroy(L);//�������ݱ��ͷ��ڴ�
		break;
		//---------------------�رմ���ָ��-----------------
	case WM_DESTROY:
		PostQuitMessage(0);
		destroy(L);
		break;
		//---------------------���ٴ���ָ��-----------------
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
		//---------------------���������ѭ��-------------
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	Init(L);
	memset(&wndclass, 0, sizeof(wndclass));
	wndclass.cbSize = sizeof(WNDCLASSEX);//���崰������
	wndclass.lpfnWndProc = WndProc;//����˴��ڵ���Ϣ������
	wndclass.hInstance = hInstance;//�����ʵ�������WinMain�����ĵ�һ��������
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);//�����������״
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//���ô��ڱ���
	wndclass.lpszClassName = "WindowClass"; //���崰���������
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//���ô��ڵ�ͼ��
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//�����������״
	if (!RegisterClassEx(&wndclass)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
		"WindowClass",//�����������
		"��������ϵͳ",
		WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_BORDER,//���ڵķ��
		CW_USEDEFAULT,//���ڵ�����X
		CW_USEDEFAULT,//���ڵ�����y
		1000,//���ڵĿ��
		600,//���ڵĸ߶�
		NULL, NULL, hInstance, NULL);//�����ھ�������ڵĲ˵���������ڵľ��������ָ��

	//--------------------------���������ô�������--------------------------------------
	if (!hwnd)
	{
		MessageBox(NULL, "�����ڴ���ʧ�ܣ�", "����", MB_ICONERROR);
		return 1;
	}
	hChildWnd = CreateWindowEx(
		0,                          // ��չ���
		TEXT("EDIT"),               // ����
		"[System]��������ϵͳ��ʼ���ɹ�!",                       // �������ı�
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | WS_EX_CLIENTEDGE,  // ��ʽ
		80, 80, 400, 380,        // λ�úʹ�С
		hwnd,                       // �����ھ��
		NULL,                       // �˵����
		hInstance,                  // Ӧ�ó���ʵ�����
		NULL                        // ��������
	);

	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	CreateWindow("BUTTON", "��������", WS_VISIBLE | WS_CHILD,
		500, 260, 120, 40, hwnd, (HMENU)ID_BUTTON1, hInstance, NULL);
	CreateWindow("BUTTON", "����ͳ��", WS_VISIBLE | WS_CHILD,
		500, 320, 120, 40, hwnd, (HMENU)ID_BUTTON2, hInstance, NULL);
	CreateWindow("BUTTON", "��������", WS_VISIBLE | WS_CHILD,
		500, 200, 120, 40, hwnd, (HMENU)ID_BUTTON3, hInstance, NULL);
	CreateWindow("BUTTON", "��������", WS_VISIBLE | WS_CHILD,
		670, 80, 80, 30, hwnd, (HMENU)ID_BUTTON4, hInstance, NULL);
	CreateWindow("BUTTON", "��������", WS_VISIBLE | WS_CHILD,
		500, 140, 120, 40, hwnd, (HMENU)ID_BUTTON5, hInstance, NULL);
	CreateWindow("BUTTON", "���Ž���", WS_VISIBLE | WS_CHILD,
		780, 80, 80, 30, hwnd, (HMENU)ID_BUTTON6, hInstance, NULL);
	CreateWindow("BUTTON", "��д����", WS_VISIBLE | WS_CHILD,
		500, 80, 120, 40, hwnd, (HMENU)ID_BUTTON7, hInstance, NULL);
	CreateWindow("BUTTON", "�۸�����", WS_VISIBLE | WS_CHILD,
		670, 150, 80, 30, hwnd, (HMENU)ID_BUTTON8, hInstance, NULL);
	CreateWindow("BUTTON", "�۸���", WS_VISIBLE | WS_CHILD,
		780, 150, 80, 30, hwnd, (HMENU)ID_BUTTON9, hInstance, NULL);
	CreateWindow("BUTTON", "��������", WS_VISIBLE | WS_CHILD,
		670, 220, 80, 30, hwnd, (HMENU)ID_BUTTON10, hInstance, NULL);
	CreateWindow("BUTTON", "��������", WS_VISIBLE | WS_CHILD,
		780, 220, 80, 30, hwnd, (HMENU)ID_BUTTON11, hInstance, NULL);
	CreateWindow("BUTTON", "���Ҷ���", WS_VISIBLE | WS_CHILD,
		675, 340, 200, 40, hwnd, (HMENU)ID_BUTTON12, hInstance, NULL);
	CreateWindow("BUTTON", "��������", WS_VISIBLE | WS_CHILD,
		675, 400, 200, 40, hwnd, (HMENU)ID_BUTTON13, hInstance, NULL);
	CreateWindow("BUTTON", "������Ʒ", WS_VISIBLE | WS_CHILD,
		675, 460, 200, 40, hwnd, (HMENU)ID_BUTTON14, hInstance, NULL);
	CreateWindow("BUTTON", ">>>>����ı���<<<<", WS_VISIBLE | WS_CHILD,
		655, 285, 240, 40, hwnd, (HMENU)ID_BUTTON15, hInstance, NULL);
	CreateWindow("BUTTON", "ɾ������", WS_VISIBLE | WS_CHILD,
		500, 380, 120, 40, hwnd, (HMENU)ID_BUTTON16, hInstance, NULL);
	CreateWindow("BUTTON", "��������", WS_VISIBLE | WS_CHILD,
		500, 440, 120, 40, hwnd, (HMENU)ID_BUTTON17, hInstance, NULL);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
void DrawRect(HDC hdc, int left, int top, int right, int bottom)
{
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));	// �������λ���

	SelectObject(hdc, hPen);	                        // ����͸����ˢ

	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, left, top, right, bottom);           // ���ƾ���

	DeleteObject(hPen);                                 // �ͷŻ�����Դ
}

void draw(HWND hwnd) {

	//--------------------------------------------------------��ͼ-----------------------------------------------------------
	HDC hdc;  // �豸�����ľ��
	PAINTSTRUCT ps;  // ��ͼ�ṹ��
	RECT rect;  // ���νṹ��

	hdc = BeginPaint(hwnd, &ps);  // ��ʼ��ͼ����ȡ�豸�����ľ���ͻ�ͼ�ṹ��
	GetClientRect(hwnd, &rect);  // ��ȡ���ڿͻ����ľ���

	DrawRect(hdc, rect.left + 50, rect.top + 50, rect.right - 50, rect.bottom - 50);  // ���ƾ���

	MoveToEx(hdc, 640, 50, NULL);  // �ƶ���ָ��λ��
	LineTo(hdc, 640, 505);  // ����ֱ��
	MoveToEx(hdc, 640, 280, NULL);  // �ƶ���ָ��λ��
	LineTo(hdc, 910, 280);  // ����ֱ��

	SetTextColor(hdc, RGB(255, 10, 10));  // �����ı���ɫΪ��ɫ
	SetBkMode(hdc, TRANSPARENT);  // ���ñ���ģʽΪ͸��

	//----------------------------------------------------------�ı�-----------------------------------------------------------
	LOGFONT lf = { 0 };  // ����ṹ��
	lf.lfHeight = 20;  // ��������߶�Ϊ20
	lf.lfWeight = FW_NORMAL;  // ���������ϸΪ��ͨ
	lstrcpy(lf.lfFaceName, TEXT("����"));  // ������������Ϊ����

	HFONT hFont = CreateFontIndirect(&lf);  // �����������
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);  // ѡ����������豸������
	TextOut(hdc, 200, 55, "��Ϣ��ʾ̨", lstrlen("��Ϣ��ʾ̨"));  // �����ı�
	TextOut(hdc, 120, 470, "���������ʱ�����ı���������ؼ���", lstrlen("����ʱ�����ı���������ؼ���"));  // �����ı�

	lf.lfHeight = 35;  // ��������߶�Ϊ35
	hFont = CreateFontIndirect(&lf);  // �����������
	hOldFont = (HFONT)SelectObject(hdc, hFont);  // ѡ����������豸������
	SetTextColor(hdc, RGB(0, 0, 255));  // ����������ɫΪ��ɫ
	TextOut(hdc, 300, 10, "��������ϵͳ", lstrlen("��������ϵͳ"));  // �����ı�

	lf.lfHeight = 20;  // ��������߶�Ϊ20
	hFont = CreateFontIndirect(&lf);  // �����������
	hOldFont = (HFONT)SelectObject(hdc, hFont);  // ѡ����������豸������
	TextOut(hdc, 515, 25, "(by:֣�¿�&��־��)", lstrlen("(by:֣�¿�&��־��)"));  // �����ı�

	//----------------------------------------------------------����-----------------------------------------------------------
	EndPaint(hwnd, &ps);  // ������ͼ���ͷ���Դ
	return;
}
void Clear_tempNode()
{
	for (int i = 0; i < length_tempNode; i++)
	{
		tempNode[i] = NULL;
	}
}//��ձ�ɾ���ڵ�Ļ�������
void AppendTextToEdit(const char* text)
{
	int textLength = GetWindowTextLength(hChildWnd);//��ȡ���ڵ�ǰ�ı�����
	SendMessage(hChildWnd, EM_SETSEL, textLength, textLength);//��������ڷ���ָ���������������ı�ĩ
	SendMessage(hChildWnd, EM_REPLACESEL, 0, (LPARAM)TEXT(text));//��������ڷ���ָ��������ı�
	SendMessage(hChildWnd, WM_VSCROLL, SB_BOTTOM, 0); //���ı�׷�ӵ���������
}             

void AppendTextToEdit_Clear(const char* text)
{
	int textLength = GetWindowTextLength(hChildWnd);
	SendMessage(hChildWnd, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);

	// ��༭���� EM_REPLACESEL ��Ϣ�����ı�׷�ӵ������ı���ĩβ,
	SendMessage(hChildWnd, EM_REPLACESEL, FALSE, (LPARAM)text);
	SendMessage(hChildWnd, WM_VSCROLL, SB_BOTTOM, 0);
}//��չ������ڵ��ı�
void LoadData_Win(HWND hwnd, List& L)
{
	if (Have_Loaded == 1)
	{
		AppendTextToEdit("\r\n[system]�����ظ���������");
		return;
	}
	int num_of_del =0;//�Զ�ɾ����ͬ���ݵ�����
	long long key = LoadData(L,&num_of_del);//������������������key
	if (key == -1)
		AppendTextToEdit("\r\n[System]��������ʧ��,�����Ŀ¼�Ƿ��������ļ�");
	else
	{
		char str1[50] = { 0 };
		char str2[50] = { 0 };
		sprintf(str2, "\r\n[System]�Ѿ��Զ�ɾ��%d����ͬ����", num_of_del);
		sprintf(str1, "\r\n[System]�ɹ�����%d����Ч����!", key-num_of_del);
		AppendTextToEdit(str2);
		AppendTextToEdit(str1);

	}
	return;
}//�����ļ����ݵĺ���
void OutputData_Win(HWND hwnd, List& L)
{
	int key = OutputData(L);//������������������key

	if (key == -1)
		AppendTextToEdit("\r\n[System]��������ʧ��,������");
	else
	{
		char str[50] = { 0 };
		sprintf(str, "\r\n[System]�ɹ�����%d������,���ڸ�Ŀ¼����", key);
		AppendTextToEdit(str);
	}
	return;
}
void OverwriteData_Win(HWND hwnd, List& L)
{
	int key = OverwriteData(L);//������������������key

	if (key == -1)
		AppendTextToEdit("\r\n[System]��д����ʧ��,������");
	else
	{
		char str[50] = { 0 };
		sprintf(str, "\r\n[System]�ɹ���д%d������,data.txt�ļ��Ѹ�д", key);
		AppendTextToEdit(str);
	}
	return;
}
int Statistic_Win(HWND hwnd, List L)
{
	double sum = 0;//�����۸��ܺ�
	long long count;//������
	count = L.length;
	LNode* p = new LNode;
	int i;
	for (i = 0, p = L.head->next; i < count && p; p = p->next, i++)//˫��
	{
		sum += p->data.price;
	}
	FILE* fp = fopen("Statistic.txt", "w");
	fprintf(fp, "----------------ͳ����Ϣ-----------------\n");
	fprintf(fp, ">>>>>>>>��������: % d���Σ�\n", count);
	fprintf(fp, ">>>>>>>>�����ܶ�:%.2lf��Ԫ��\n", sum);
	AppendTextToEdit("\r\n-----------------------------------����ͳ�ƽ��-------------------------------------");
	char str1[50] = { 0 };
	char str2[50] = { 0 };
	sprintf(str1, "\r\n>>>>>>>>��������:%d���Σ�", count);
	sprintf(str2, "\r\n>>>>>>>>�����ܶ�:%.2lf��Ԫ��", sum);
	AppendTextToEdit(str1);
	AppendTextToEdit(str2);
	AppendTextToEdit("\r\n[System]����ͳ����Ϣ�Ѿ�����ΪStatistic.txt");
	AppendTextToEdit("\r\n-----------------------------------------------------------------------------------------------");
	fclose(fp);
	fp = NULL;
	printf("ͳ�����ж�������Ϣ��ɣ�");
	return 1;
}//ͳ�����ж�������Ϣ�������ļ���ʽ���
void Traverse_Win(HWND hwnd, List &L)
{
	LNode* temp = L.head;
	same_dele(L);
	char str1[200] = { 0 };
	char str2[200] = { 0 };
	long long count = 0;
	AppendTextToEdit("\r\n-----------------------------------���ݱ������------------------------------------");
	if (L.head->next == NULL)
	{
		AppendTextToEdit("\r\n[System]��ǰ�����ݣ�  ����ǰ��������");
		AppendTextToEdit("\r\n-----------------------------------���ݱ������------------------------------------");
		return;
	}
	while (L.head->next != NULL)
	{
		L.head = L.head->next;
		count++;
		sprintf(str1, "\r\n%lld>>>>>>>������:%lld  ����:%s  ��Ʒ��:%s  �����۸�:%.2lf", count, L.head->data.orderNumber, L.head->data.customerName, L.head->data.productName, L.head->data.price);
		AppendTextToEdit(str1);
	}
	L.head = temp;
	sprintf(str2, "\r\n--------------------------��%d���������ݱ������--------------------------", count);
	AppendTextToEdit(str2);
}
void  Find_ordetNumber_Win(HWND hwnd, List L)
{
	if (length_tempNode != 0)
		Clear_tempNode();
	char str1[200] = { 0 };
	char str2[200] = { 0 };
	long long count = 0;
	int textLength = GetWindowTextLength(hChildWnd);
	char* buffer = (char*)malloc((textLength + 1) * sizeof(char));
	GetWindowTextA(hChildWnd, buffer, textLength + 1);
	long long number = atoi(buffer);
	AppendTextToEdit("\r\n-----------------------------------���ݲ��ҽ��------------------------------------");
	LNode* p = new LNode;
	int Hash_number = number % 10;
	for (p = L.Hash_table_number[Hash_number]->Hash_table_next; p; p = p->Hash_table_next)
	{
		if (number == p->data.orderNumber)
		{
			tempNode[count] = p;
			count++;
			sprintf(str1, "\r\n%lld>>>>>>>������:%lld  ����:%s  ��Ʒ��:%s  �����۸�:%.2lf", count, p->data.orderNumber,p->data.customerName,p->data.productName,p->data.price);
			AppendTextToEdit(str1);
		}
	}
	sprintf(str2, "\r\n--------------------------��%d���������ݱ��ҵ�!-----------------------------", count);
	AppendTextToEdit(str2);
	free(buffer);
	length_tempNode = count;
}
void Find_customerName_Win(HWND hwnd, List L)
{
	if (length_tempNode != 0)
		Clear_tempNode();
	char str1[200] = { 0 };
	char str2[200] = { 0 };
	long long count = 0;
	int textLength = GetWindowTextLength(hChildWnd);
	char* buffer = (char*)malloc((textLength + 1) * sizeof(char));
	GetWindowTextA(hChildWnd, buffer, textLength + 1);
	AppendTextToEdit("\r\n-----------------------------------���ݲ��ҽ��------------------------------------");
	LNode* p = new LNode;
	for (p = L.head->next; p; p = p->next)
	{
		if (strcmp(buffer, p->data.customerName) == 0)
		{
			tempNode[count] = p;
			count++;
			sprintf(str1, "\r\n%lld>>>>>>>������:%lld  ����:%s  ��Ʒ��:%s  �����۸�:%.2lf", count, p->data.orderNumber, p->data.customerName, p->data.productName, p->data.price);
			AppendTextToEdit(str1);
		}
	}
	sprintf(str2, "\r\n--------------------------��%d���������ݱ��ҵ�!-----------------------------", count);
	AppendTextToEdit(str2);
	free(buffer);
	length_tempNode = count;
}
void Find_productName_Win(HWND hwnd, List L)
{
	if (length_tempNode != 0)
		Clear_tempNode();
	char str1[200] = { 0 };
	char str2[200] = { 0 };
	long long count = 0;
	int textLength = GetWindowTextLength(hChildWnd);
	char* buffer = (char*)malloc((textLength + 1) * sizeof(char));
	GetWindowTextA(hChildWnd, buffer, textLength + 1);
	AppendTextToEdit("\r\n-----------------------------------���ݲ��ҽ��------------------------------------");
	LNode* p = new LNode;
	for (p = L.head->next; p; p = p->next)
	{
		if (strcmp(buffer, p->data.productName) == 0)
		{
			tempNode[count] = p;
			count++;
			sprintf(str1, "\r\n%lld>>>>>>>������:%lld  ����:%s  ��Ʒ��:%s  �����۸�:%.2lf", count, p->data.orderNumber, p->data.customerName, p->data.productName, p->data.price);
			AppendTextToEdit(str1);
		}
	}
	sprintf(str2, "\r\n--------------------------��%d���������ݱ��ҵ�!-----------------------------", count);
	AppendTextToEdit(str2);
	free(buffer);
	length_tempNode = count;
}
void IncertData_Win(HWND hwnd, List &L)
{
	int count = 1;
	int key = 1;
	char ordernumber[100] = { 0 };
	char customername[100] = { 0 };
	char productname[100] = { 0 };
	char price[100] = { 0 };
	int textLength = GetWindowTextLength(hChildWnd);
	char* buffer = (char*)malloc((textLength + 1) * sizeof(char));
	GetWindowTextA(hChildWnd, buffer, textLength + 1);
	ElemType newdata;
	for (int i = 0; i < textLength + 1; i++)
	{
		if (count == 1 && key == 1)
		{
			sscanf(buffer, "%s", ordernumber);
			key = 0;
		}
		else if (count == 2 && key == 1)
		{
			sscanf(buffer, "%*s %s", customername);
			key = 0;
		}
		else if (count == 3 && key == 1)
		{
			sscanf(buffer, "%*s %*s %s", productname);
			key = 0;
		}
		else if (count == 4 && key == 1)
		{
			sscanf(buffer, "%*s %*s %*s %s", price);
			key = 0;
			break;
		}
		if (buffer[i] == ' ')
		{
			count++;
			key = 1;
		}
	}
	strcpy(newdata.customerName, customername);
	strcpy(newdata.productName, productname);
	newdata.orderNumber=atoi(ordernumber);
	int Hash_number = newdata.orderNumber % 10;
	int same = 0;
	LNode* p = new LNode;
	for (p = L.Hash_table_number[Hash_number]->Hash_table_next; p; p = p->Hash_table_next)
	{
		if (newdata.orderNumber == p->data.orderNumber)
		{
			same = 1;
		}
	}
	free(p);
	if (same == 1)
	{
		AppendTextToEdit("\r\n[system]������ͬ������,�޷�����");
		return;
	}
	newdata.orderNumber = atoi(ordernumber);
	newdata.price = atof(price);
	insert(L, newdata);
	AppendTextToEdit("\r\n[system]�������ݳɹ�!");
}
void DelData_Win(HWND hwnd, List &L)
{
	for (int i = 0; i < length_tempNode; i++)
	{
		dele_node(tempNode[i]);
		L.length--;
	}
	length_tempNode = 0;
}
