#define _CRT_SECURE_NO_WARNINGS
#include"function.h"
//------------全局变量声明-------
LNode* tempNode[200];//将被删除节点的缓存数组
List L;              //全局数据表
HWND  hChildWnd;     //主窗体的子窗体(滚动窗体的句柄)
   
//------------------------------------------------------函数预声明-------------------------------------------------------

void DrawRect(HDC hdc, int left, int top, int right, int bottom);                               //绘制矩形
void draw(HWND hwnd);                                                                           //绘制主窗体
void AppendTextToEdit(const char* text);                                                        //在滚动窗体追加文本
void AppendTextToEdit_Clear(const char* text);                                                  //清空滚动窗体文本
void LoadData_Win(HWND hwnd, List& L);                                                          //加载文件数据                           
void OutputData_Win(HWND hwnd, List& L);                                                        //输出文件数据
void OverwriteData_Win(HWND hwnd, List& L);                                                     //覆写文件数据
int Statistic_Win(HWND hwnd, List L);                                                           //统计文件数据
void Traverse_Win(HWND hwnd, List& L);                                                          //遍历数据，并在滚动窗口显示
void  Find_ordetNumber_Win(HWND hwnd, List L);                                                  //根据订单号查找数据
void Find_customerName_Win(HWND hwnd, List L);                                                  //根据顾客姓名查找数据
void Find_productName_Win(HWND hwnd, List L);                                                   //根据商品名查找数据
void IncertData_Win(HWND hwnd, List& L);                                                        //插入数据
void DelData_Win(HWND hwnd, List &L);                                                           //删除数据
void Clear_tempNode();                                                                          //清空指针缓存数组
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);                //窗体处理函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);//主函数

//------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_ERASEBKGND:
	{
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 200, 180));  // 创建红色画刷
		HDC hdc = (HDC)wParam;
		RECT rect;
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, hBrush);  // 填充窗体矩形区域为红色
		DeleteObject(hBrush);  // 释放画刷资源
		return TRUE;
		//--------------------填充主窗口背景色---------------------
	}
	case WM_PAINT:
		draw(hwnd);
		break;
		//---------------------绘制主窗体线条---------------------
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_BUTTON5) {
			LoadData_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON1) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]未载入数据,无法输出！");
			}
			else
				OutputData_Win(hwnd, L);
		}
		if (LOWORD(wParam) == ID_BUTTON7) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]未载入数据,无法覆写！");
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
				AppendTextToEdit("\r\n[system]未载入数据,无法排序！");
			}
			else
			{
				Downrank_orderNumber(L);
				AppendTextToEdit("\r\n[system]按订单降序排列完毕！");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON4) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]未载入数据,无法排序！");
			}
			else
			{
				Uprank_orderNumber(L);
				AppendTextToEdit("\r\n[system]按订单升序排列完毕！");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON8) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]未载入数据,无法排序！");
			}
			else
			{
				Uprank_price(L);
				AppendTextToEdit("\r\n[system]按价格升序排列完毕！");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON9) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]未载入数据,无法排序！");
			}
			else
			{
				Downrank_price(L);
				AppendTextToEdit("\r\n[system]按价格降序排列完毕！");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON10) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]未载入数据,无法排序！");
			}
			else
			{
				Uprank_customerName(L);
				AppendTextToEdit("\r\n[system]按姓名升序排列完毕！");
			}
		}
		if (LOWORD(wParam) == ID_BUTTON11) {
			if (Have_Loaded == 0)
			{
				AppendTextToEdit("\r\n[system]未载入数据,无法排序！");
			}
			else
			{
				Downrank_customerName(L);
				AppendTextToEdit("\r\n[system]按姓名降序排列完毕！");
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
			AppendTextToEdit("[system]目标数据已经被删除!");
		}
		if (LOWORD(wParam) == ID_BUTTON17) {
			IncertData_Win(hwnd, L);
		}

		break;
		//-----------------------响应命令--------------------
	case WM_CLOSE: //关闭
		if (IDYES == MessageBox(hwnd, "是否退出出管理系统?", "提示", MB_YESNO | MB_ICONQUESTION))
		{
			DestroyWindow(hwnd);
		}
		PostQuitMessage(0);
		destroy(L);//销毁数据表，释放内存
		break;
		//---------------------关闭窗口指令-----------------
	case WM_DESTROY:
		PostQuitMessage(0);
		destroy(L);
		break;
		//---------------------销毁窗口指令-----------------
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
		//---------------------无命令，继续循环-------------
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	Init(L);
	memset(&wndclass, 0, sizeof(wndclass));
	wndclass.cbSize = sizeof(WNDCLASSEX);//定义窗口类型
	wndclass.lpfnWndProc = WndProc;//定义此窗口的消息处理函数
	wndclass.hInstance = hInstance;//程序的实例句柄（WinMain函数的第一个参数）
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);//设置鼠标光标形状
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口背景
	wndclass.lpszClassName = "WindowClass"; //定义窗口类的名称
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//设置窗口的图标
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//设置鼠标光标形状
	if (!RegisterClassEx(&wndclass)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
		"WindowClass",//窗口类的名称
		"订单管理系统",
		WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_BORDER,//窗口的风格
		CW_USEDEFAULT,//窗口的坐标X
		CW_USEDEFAULT,//窗口的坐标y
		1000,//窗口的宽度
		600,//窗口的高度
		NULL, NULL, hInstance, NULL);//父窗口句柄、窗口的菜单句柄、窗口的句柄、参数指针

	//--------------------------以上是设置窗体属性--------------------------------------
	if (!hwnd)
	{
		MessageBox(NULL, "主窗口创建失败！", "错误", MB_ICONERROR);
		return 1;
	}
	hChildWnd = CreateWindowEx(
		0,                          // 扩展风格
		TEXT("EDIT"),               // 类名
		"[System]订单管理系统初始化成功!",                       // 标题栏文本
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | WS_EX_CLIENTEDGE,  // 样式
		80, 80, 400, 380,        // 位置和大小
		hwnd,                       // 父窗口句柄
		NULL,                       // 菜单句柄
		hInstance,                  // 应用程序实例句柄
		NULL                        // 创建参数
	);

	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	CreateWindow("BUTTON", "导出数据", WS_VISIBLE | WS_CHILD,
		500, 260, 120, 40, hwnd, (HMENU)ID_BUTTON1, hInstance, NULL);
	CreateWindow("BUTTON", "数据统计", WS_VISIBLE | WS_CHILD,
		500, 320, 120, 40, hwnd, (HMENU)ID_BUTTON2, hInstance, NULL);
	CreateWindow("BUTTON", "遍历数据", WS_VISIBLE | WS_CHILD,
		500, 200, 120, 40, hwnd, (HMENU)ID_BUTTON3, hInstance, NULL);
	CreateWindow("BUTTON", "单号升序", WS_VISIBLE | WS_CHILD,
		670, 80, 80, 30, hwnd, (HMENU)ID_BUTTON4, hInstance, NULL);
	CreateWindow("BUTTON", "载入数据", WS_VISIBLE | WS_CHILD,
		500, 140, 120, 40, hwnd, (HMENU)ID_BUTTON5, hInstance, NULL);
	CreateWindow("BUTTON", "单号降序", WS_VISIBLE | WS_CHILD,
		780, 80, 80, 30, hwnd, (HMENU)ID_BUTTON6, hInstance, NULL);
	CreateWindow("BUTTON", "覆写数据", WS_VISIBLE | WS_CHILD,
		500, 80, 120, 40, hwnd, (HMENU)ID_BUTTON7, hInstance, NULL);
	CreateWindow("BUTTON", "价格升序", WS_VISIBLE | WS_CHILD,
		670, 150, 80, 30, hwnd, (HMENU)ID_BUTTON8, hInstance, NULL);
	CreateWindow("BUTTON", "价格降序", WS_VISIBLE | WS_CHILD,
		780, 150, 80, 30, hwnd, (HMENU)ID_BUTTON9, hInstance, NULL);
	CreateWindow("BUTTON", "姓名升序", WS_VISIBLE | WS_CHILD,
		670, 220, 80, 30, hwnd, (HMENU)ID_BUTTON10, hInstance, NULL);
	CreateWindow("BUTTON", "姓名降序", WS_VISIBLE | WS_CHILD,
		780, 220, 80, 30, hwnd, (HMENU)ID_BUTTON11, hInstance, NULL);
	CreateWindow("BUTTON", "查找订单", WS_VISIBLE | WS_CHILD,
		675, 340, 200, 40, hwnd, (HMENU)ID_BUTTON12, hInstance, NULL);
	CreateWindow("BUTTON", "查找姓名", WS_VISIBLE | WS_CHILD,
		675, 400, 200, 40, hwnd, (HMENU)ID_BUTTON13, hInstance, NULL);
	CreateWindow("BUTTON", "查找商品", WS_VISIBLE | WS_CHILD,
		675, 460, 200, 40, hwnd, (HMENU)ID_BUTTON14, hInstance, NULL);
	CreateWindow("BUTTON", ">>>>清空文本框<<<<", WS_VISIBLE | WS_CHILD,
		655, 285, 240, 40, hwnd, (HMENU)ID_BUTTON15, hInstance, NULL);
	CreateWindow("BUTTON", "删除数据", WS_VISIBLE | WS_CHILD,
		500, 380, 120, 40, hwnd, (HMENU)ID_BUTTON16, hInstance, NULL);
	CreateWindow("BUTTON", "插入数据", WS_VISIBLE | WS_CHILD,
		500, 440, 120, 40, hwnd, (HMENU)ID_BUTTON17, hInstance, NULL);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
void DrawRect(HDC hdc, int left, int top, int right, int bottom)
{
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));	// 创建矩形画笔

	SelectObject(hdc, hPen);	                        // 创建透明画刷

	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, left, top, right, bottom);           // 绘制矩形

	DeleteObject(hPen);                                 // 释放画笔资源
}

void draw(HWND hwnd) {

	//--------------------------------------------------------绘图-----------------------------------------------------------
	HDC hdc;  // 设备上下文句柄
	PAINTSTRUCT ps;  // 绘图结构体
	RECT rect;  // 矩形结构体

	hdc = BeginPaint(hwnd, &ps);  // 开始绘图，获取设备上下文句柄和绘图结构体
	GetClientRect(hwnd, &rect);  // 获取窗口客户区的矩形

	DrawRect(hdc, rect.left + 50, rect.top + 50, rect.right - 50, rect.bottom - 50);  // 绘制矩形

	MoveToEx(hdc, 640, 50, NULL);  // 移动到指定位置
	LineTo(hdc, 640, 505);  // 绘制直线
	MoveToEx(hdc, 640, 280, NULL);  // 移动到指定位置
	LineTo(hdc, 910, 280);  // 绘制直线

	SetTextColor(hdc, RGB(255, 10, 10));  // 设置文本颜色为红色
	SetBkMode(hdc, TRANSPARENT);  // 设置背景模式为透明

	//----------------------------------------------------------文本-----------------------------------------------------------
	LOGFONT lf = { 0 };  // 字体结构体
	lf.lfHeight = 20;  // 设置字体高度为20
	lf.lfWeight = FW_NORMAL;  // 设置字体粗细为普通
	lstrcpy(lf.lfFaceName, TEXT("宋体"));  // 设置字体名称为宋体

	HFONT hFont = CreateFontIndirect(&lf);  // 创建字体对象
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);  // 选择字体对象到设备上下文
	TextOut(hdc, 200, 55, "信息显示台", lstrlen("信息显示台"));  // 绘制文本
	TextOut(hdc, 120, 470, "查找与插入时请在文本框中输入关键词", lstrlen("查找时请在文本框中输入关键词"));  // 绘制文本

	lf.lfHeight = 35;  // 设置字体高度为35
	hFont = CreateFontIndirect(&lf);  // 创建字体对象
	hOldFont = (HFONT)SelectObject(hdc, hFont);  // 选择字体对象到设备上下文
	SetTextColor(hdc, RGB(0, 0, 255));  // 设置字体颜色为蓝色
	TextOut(hdc, 300, 10, "订单管理系统", lstrlen("订单管理系统"));  // 绘制文本

	lf.lfHeight = 20;  // 设置字体高度为20
	hFont = CreateFontIndirect(&lf);  // 创建字体对象
	hOldFont = (HFONT)SelectObject(hdc, hFont);  // 选择字体对象到设备上下文
	TextOut(hdc, 515, 25, "(by:郑德凯&周志赋)", lstrlen("(by:郑德凯&周志赋)"));  // 绘制文本

	//----------------------------------------------------------结束-----------------------------------------------------------
	EndPaint(hwnd, &ps);  // 结束绘图，释放资源
	return;
}
void Clear_tempNode()
{
	for (int i = 0; i < length_tempNode; i++)
	{
		tempNode[i] = NULL;
	}
}//清空被删除节点的缓存数组
void AppendTextToEdit(const char* text)
{
	int textLength = GetWindowTextLength(hChildWnd);//获取窗口当前文本长度
	SendMessage(hChildWnd, EM_SETSEL, textLength, textLength);//向滚动窗口发送指令，将插入点设置在文本末
	SendMessage(hChildWnd, EM_REPLACESEL, 0, (LPARAM)TEXT(text));//向滚动窗口发送指令，将插入文本
	SendMessage(hChildWnd, WM_VSCROLL, SB_BOTTOM, 0); //将文本追加到滚动窗口
}             

void AppendTextToEdit_Clear(const char* text)
{
	int textLength = GetWindowTextLength(hChildWnd);
	SendMessage(hChildWnd, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);

	// 向编辑框发送 EM_REPLACESEL 消息，将文本追加到现有文本的末尾,
	SendMessage(hChildWnd, EM_REPLACESEL, FALSE, (LPARAM)text);
	SendMessage(hChildWnd, WM_VSCROLL, SB_BOTTOM, 0);
}//清空滚动窗口的文本
void LoadData_Win(HWND hwnd, List& L)
{
	if (Have_Loaded == 1)
	{
		AppendTextToEdit("\r\n[system]请勿重复载入数据");
		return;
	}
	int num_of_del =0;//自动删除相同数据的数量
	long long key = LoadData(L,&num_of_del);//函数返回数据条数到key
	if (key == -1)
		AppendTextToEdit("\r\n[System]载入数据失败,请检查根目录是否有数据文件");
	else
	{
		char str1[50] = { 0 };
		char str2[50] = { 0 };
		sprintf(str2, "\r\n[System]已经自动删除%d条相同数据", num_of_del);
		sprintf(str1, "\r\n[System]成功载入%d条有效数据!", key-num_of_del);
		AppendTextToEdit(str2);
		AppendTextToEdit(str1);

	}
	return;
}//载入文件数据的函数
void OutputData_Win(HWND hwnd, List& L)
{
	int key = OutputData(L);//函数返回数据条数到key

	if (key == -1)
		AppendTextToEdit("\r\n[System]导出数据失败,请重试");
	else
	{
		char str[50] = { 0 };
		sprintf(str, "\r\n[System]成功导出%d条数据,请在根目录查找", key);
		AppendTextToEdit(str);
	}
	return;
}
void OverwriteData_Win(HWND hwnd, List& L)
{
	int key = OverwriteData(L);//函数返回数据条数到key

	if (key == -1)
		AppendTextToEdit("\r\n[System]覆写数据失败,请重试");
	else
	{
		char str[50] = { 0 };
		sprintf(str, "\r\n[System]成功覆写%d条数据,data.txt文件已覆写", key);
		AppendTextToEdit(str);
	}
	return;
}
int Statistic_Win(HWND hwnd, List L)
{
	double sum = 0;//订单价格总和
	long long count;//订单数
	count = L.length;
	LNode* p = new LNode;
	int i;
	for (i = 0, p = L.head->next; i < count && p; p = p->next, i++)//双重
	{
		sum += p->data.price;
	}
	FILE* fp = fopen("Statistic.txt", "w");
	fprintf(fp, "----------------统计信息-----------------\n");
	fprintf(fp, ">>>>>>>>订单总数: % d（次）\n", count);
	fprintf(fp, ">>>>>>>>交易总额:%.2lf（元）\n", sum);
	AppendTextToEdit("\r\n-----------------------------------数据统计结果-------------------------------------");
	char str1[50] = { 0 };
	char str2[50] = { 0 };
	sprintf(str1, "\r\n>>>>>>>>订单总数:%d（次）", count);
	sprintf(str2, "\r\n>>>>>>>>交易总额:%.2lf（元）", sum);
	AppendTextToEdit(str1);
	AppendTextToEdit(str2);
	AppendTextToEdit("\r\n[System]订单统计信息已经导出为Statistic.txt");
	AppendTextToEdit("\r\n-----------------------------------------------------------------------------------------------");
	fclose(fp);
	fp = NULL;
	printf("统计所有订单的信息完成！");
	return 1;
}//统计所有订单的信息，并以文件形式输出
void Traverse_Win(HWND hwnd, List &L)
{
	LNode* temp = L.head;
	same_dele(L);
	char str1[200] = { 0 };
	char str2[200] = { 0 };
	long long count = 0;
	AppendTextToEdit("\r\n-----------------------------------数据遍历结果------------------------------------");
	if (L.head->next == NULL)
	{
		AppendTextToEdit("\r\n[System]当前无数据，  请提前载入数据");
		AppendTextToEdit("\r\n-----------------------------------数据遍历完毕------------------------------------");
		return;
	}
	while (L.head->next != NULL)
	{
		L.head = L.head->next;
		count++;
		sprintf(str1, "\r\n%lld>>>>>>>订单号:%lld  姓名:%s  商品名:%s  订单价格:%.2lf", count, L.head->data.orderNumber, L.head->data.customerName, L.head->data.productName, L.head->data.price);
		AppendTextToEdit(str1);
	}
	L.head = temp;
	sprintf(str2, "\r\n--------------------------共%d条数据数据遍历完毕--------------------------", count);
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
	AppendTextToEdit("\r\n-----------------------------------数据查找结果------------------------------------");
	LNode* p = new LNode;
	int Hash_number = number % 10;
	for (p = L.Hash_table_number[Hash_number]->Hash_table_next; p; p = p->Hash_table_next)
	{
		if (number == p->data.orderNumber)
		{
			tempNode[count] = p;
			count++;
			sprintf(str1, "\r\n%lld>>>>>>>订单号:%lld  姓名:%s  商品名:%s  订单价格:%.2lf", count, p->data.orderNumber,p->data.customerName,p->data.productName,p->data.price);
			AppendTextToEdit(str1);
		}
	}
	sprintf(str2, "\r\n--------------------------共%d条数据数据被找到!-----------------------------", count);
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
	AppendTextToEdit("\r\n-----------------------------------数据查找结果------------------------------------");
	LNode* p = new LNode;
	for (p = L.head->next; p; p = p->next)
	{
		if (strcmp(buffer, p->data.customerName) == 0)
		{
			tempNode[count] = p;
			count++;
			sprintf(str1, "\r\n%lld>>>>>>>订单号:%lld  姓名:%s  商品名:%s  订单价格:%.2lf", count, p->data.orderNumber, p->data.customerName, p->data.productName, p->data.price);
			AppendTextToEdit(str1);
		}
	}
	sprintf(str2, "\r\n--------------------------共%d条数据数据被找到!-----------------------------", count);
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
	AppendTextToEdit("\r\n-----------------------------------数据查找结果------------------------------------");
	LNode* p = new LNode;
	for (p = L.head->next; p; p = p->next)
	{
		if (strcmp(buffer, p->data.productName) == 0)
		{
			tempNode[count] = p;
			count++;
			sprintf(str1, "\r\n%lld>>>>>>>订单号:%lld  姓名:%s  商品名:%s  订单价格:%.2lf", count, p->data.orderNumber, p->data.customerName, p->data.productName, p->data.price);
			AppendTextToEdit(str1);
		}
	}
	sprintf(str2, "\r\n--------------------------共%d条数据数据被找到!-----------------------------", count);
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
		AppendTextToEdit("\r\n[system]已有相同订单号,无法插入");
		return;
	}
	newdata.orderNumber = atoi(ordernumber);
	newdata.price = atof(price);
	insert(L, newdata);
	AppendTextToEdit("\r\n[system]插入数据成功!");
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
