#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <string>
#include <stdlib.h>

#define nullptr 0

void absDelay(int delay)
{
	static int curtime = GetTickCount();
	static int pretime = GetTickCount();
	while (curtime - pretime < delay)
	{
		curtime = GetTickCount();
		Sleep(1);
	}
	pretime = curtime;
}

struct S_body
{
	int row, col;
	S_body* next;

	int Direct;
	S_body()
	{
		row = col = Direct = 0;
		next = nullptr;
	}
};

struct food
{
	int r, c;
	bool Exist;
}Food;

class Game
{
public:
	Game()
	{
		SPEED = 5;
		//创建长度为3的蛇
		S_body* b1 = new S_body;
		S_body* b2 = new S_body;
		S_body* b3 = new S_body;

		b1->next = b2;
		b2->next = b3;
		b3->next = nullptr;

		b1->row = b2->row = b3->row = 3;
		b1->col = 1; b2->col = 2; b3->col = 3;
		b1->Direct = b2->Direct = b3->Direct = 3;

		head = b3;
		tail = b1;

		Direction = 3;
		Length = 3;

		//生成一个食物
		Food.r = 10;
		Food.c = 10;
		Food.Exist = true;
	}
	~Game()
	{
		S_body* p = nullptr;
		while (tail)
		{
			p = tail->next;
			delete tail;
			tail = p;
		}
	}

	void UpdateLogs()
	{
		std::cout << "更新日志：\n";
		std::cout << "Ver1.0   简单地实现了蛇的运动和进食.\n";
		std::cout << "Ver2.0   增加了蛇行进速度的选择.\n";
		std::cout << "Ver3.0   将帧数提高到了60及以上.\n";
		std::cout << "Ver4.0   提高了绘画的效率，响应更快！\n";
		system("PAUSE");
	}
	void Begin()
	{
		IMAGE start;
		loadimage(&start, "IMG", "Cat");
		LOGFONT f;
		gettextstyle(&f);                     // 获取当前字体设置
		f.lfHeight = 30;                      // 设置字体高度为 48
//		_tcscpy_s(f.lfFaceName, _T("黑体"));    // 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
		settextstyle(&f);                     // 设置字体样式
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);

		putimage(0, 0, &start);
		outtextxy(150, 80, "贪吃蛇ver4.0");
		outtextxy(125, 450, "按任意键以开始游戏~");
		outtextxy(125, 500, "'wasd'控制方向。");
		outtextxy(125, 550, "'p'  暂停。");
		
		_getch();
		absDelay(500);
	}
	void ChooseSpeed()
	{
		cleardevice();
		MOUSEMSG msg;

		outtextxy(75, 50, "请选择速度(鼠标点击)：");
		outtextxy(220, 130, "慢");
		rectangle(100, 100, 400, 200);
		outtextxy(220, 280, "中等");
		rectangle(100, 250, 400, 350);
		outtextxy(220, 430, "快");
		rectangle(100, 400, 400, 500);
		
		FlushMouseMsgBuffer();
		while (true)
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				if (msg.x < 400 && msg.x>100 && msg.y < 200 && msg.y>100)
				{
					SPEED = 1;
					break;
				}
				else if (msg.x < 400 && msg.x>100 && msg.y < 350 && msg.y>250)
				{
					SPEED = 3;
					break;
				}
				else if (msg.x < 400 && msg.x>100 && msg.y < 500 && msg.y>400)
				{
					SPEED = 5;
					break;
				}
				else
					msg = GetMouseMsg();
			}
		}
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		absDelay(500);
	}
	void InitImages()
	{
		setfillcolor(YELLOW);
		fillrectangle(1, 1, 21, 21);
		getimage(&sbody, 1, 1, 20, 20);

		setfillcolor(CYAN);
		fillrectangle(1, 1, 21, 21);
		getimage(&_food, 1, 1, 20, 20);

		setfillcolor(GREEN);
		fillrectangle(1, 1, 21, 21);
		getimage(&shead, 1, 1, 20, 20);

		loadimage(&background, "IMG", "idiot");
	}
	void Update()
	{
		char ch = '#';
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'p')
			{
				ch = '#';
				while (ch != 'p')
				{
					ch = _getch();
					Sleep(100);
				}
			}
			if (ch == 'w' && (Direction == 2 || Direction == 3))
			{
				Direction = 0;
			}
			if (ch == 's' && (Direction == 2 || Direction == 3))
			{
				Direction = 1;
			}
			if (ch == 'a' && (Direction == 0 || Direction == 1))
			{
				Direction = 2;
			}
			if (ch == 'd' && (Direction == 0 || Direction == 1))
			{
				Direction = 3;
			}
		}

		S_body* neo = new S_body;
		S_body* temp = tail;
		if (Direction == 0)
		{
			neo->row = head->row - 1;
			neo->col = head->col;
			if (neo->row < 0)
				neo->row = 24;
		}
		if (Direction == 1)
		{
			neo->row = head->row + 1;
			neo->col = head->col;
			if (neo->row > 24)
				neo->row = 0;
		}
		if (Direction == 2)
		{
			neo->row = head->row;
			neo->col = head->col - 1;
			if (neo->col < 0)
				neo->col = 24;
		}
		if (Direction == 3)
		{
			neo->row = head->row;
			neo->col = head->col + 1;
			if (neo->col > 24)
				neo->col = 0;
		}

		neo->Direct = Direction;
		neo->next = nullptr;
		head->next = neo;
		head = neo;

		if (head->row == Food.r && head->col == Food.c)
		{
			Food.Exist = false;
			Length++;
		}
		else
		{
			tail = tail->next;
			delete temp;
		}
	}
	void Draw()
	{
		IMAGE temp;
		BeginBatchDraw();
		for (int i = 0; i <= 20; i+=SPEED)
		{
			cleardevice();
			putimage(0, 0, &background);
			setlinecolor(BLACK);
			line(0, 100, 500, 100);
			outtextxy(50, 50, "简陋贪吃蛇       By Geed");
			char score[10];
			std::string str;
			itoa(Length, score, 10);
			str = "得分"; str += score; str += "  (50即可获胜)\0";
			outtextxy(0, 0, str.c_str());

			putimage(Food.c * 20, 100 + Food.r * 20, &_food);

			S_body* p = tail;
			while (p != nullptr)
			{
				if (p != head)
					temp = sbody;
				else
					temp = shead;

				if (p->Direct == 0)
					putimage(p->col * 20, 100 + (p->row + 1) * 20 - i, &temp);
				else if (p->Direct == 1)
					putimage(p->col * 20, 100 + (p->row - 1) * 20 + i, &temp);
				else if (p->Direct == 2)
					putimage((p->col + 1) * 20 - i, 100 + p->row * 20, &temp);
				else if (p->Direct == 3)
					putimage((p->col - 1) * 20 + i, 100 + p->row * 20, &temp);
				
				p = p->next;
			}
			absDelay(10);
			FlushBatchDraw();
		}
	}
	void ProduceFood()
	{
		if (Food.Exist == false)
		{
			srand((unsigned int)time(NULL));
			int x, y;
			int flag = 0;
			while (flag != 1)
			{
				S_body* p = tail;

				x = rand() % 25;
				y = rand() % 25;
				while (p)
				{
					if (p->row == x && p->col == y)
						break;
					p = p->next;
				}
				if (p)
					flag = 0;
				else
					flag = 1;
			}
			Food.r = x;
			Food.c = y;

			Food.Exist = true;
		}
	}
	void Check()
	{
		HWND hwnd = GetHWnd();
		if (Length == 50)
		{
			MessageBox(hwnd, _T("你赢了！"), _T("You Win!"), MB_OK);
			EndBatchDraw();
			closegraph();
			exit(0);
		}
		else
		{
			S_body* p = head;
			S_body* q = tail;
			while (q != head)
			{
				if (p->row == q->row && p->col == q->col)
				{
					MessageBox(hwnd, _T("你输了！"), _T("You Lose!"), MB_OK);
					EndBatchDraw();
					closegraph();
					exit(0);
				}
				else
					q = q->next;
			}
		}
	}
private:
	S_body* head;
	S_body* tail;

	// 0:up  1:down  2:left  3:right
	int Direction;
	int Length;
	int SPEED;

	//to save the snake body and food image
	IMAGE _food, sbody, shead, background;
};

int main()
{
	Game game;
	game.UpdateLogs();

	initgraph(500, 600);
	setbkcolor(LIGHTGRAY);
	game.InitImages();
	FlushBatchDraw();
	game.Begin();
	game.ChooseSpeed();
	
	while (true)
	{
		game.Update();
		game.Draw();
		game.ProduceFood();
		game.Check();
	}
	return 0;
}