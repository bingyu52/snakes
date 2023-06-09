#define _CRT_SECURE_NO_WARNINGS 1
#include<graphics.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define SNAKE_NUM 500

int myabs(int a)
{
	return a >= 0 ? a : -a;
}
enum DIR {
	UP,
	DOWN,
	LEFT,
	RIGHT
};
//蛇的结构体
struct Snake {
	int size; //蛇的节数
	int dir;  //蛇的方向
	int speed;//蛇的速度
	POINT coor[SNAKE_NUM];//坐标
}snake;

struct Food
{
	int x;        //食物的坐标
	int y;
	int r;        //食物的半径
	bool flag;    //食物是否被吃掉
	DWORD color;  //食物的颜色
}food;
//数据初始化
void gameinit()
{
	//初始化图形窗口
	//initgraph(640, 480);

	srand(GetTickCount());
	//一开始有3节
	snake.size = 3;
	snake.speed = 3;
	snake.dir = RIGHT;
	for (int i = snake.size-1 ;i>=0 ; i--)
	{
		snake.coor[i].x = 40-(snake.speed)*i;
		snake.coor[i].y = 10;
	}
	food.x = rand() % 640;
	food.y = rand() % 480;
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	food.r = rand() % 10 + 5;
	food.flag = true;
}

void BGM()
{
	mciSendString("open ./BGM//LeadfootGetaway.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);

}

void startui()
{
	//初始化图形窗口
	initgraph(640, 480);
	//双缓冲绘图
	//BeginBatchDraw();
	//设置背景颜色
	setbkcolor(RGB(28, 115, 119));//1.设置颜色
	cleardevice();                //2.清空绘图设备
	IMAGE img;
	loadimage(&img, "./image//ui.jpg", 640, 480);
	putimage(0, 0, &img);
}

void eatfood()
{
	//蛇头的圆心和食物的圆心之间的距离小于半径和 ，吃掉食物
	if (food.flag==true &&(snake.coor[0].x - food.x) * (snake.coor[0].x - food.x) +
		(snake.coor[0].y - food.y) * (snake.coor[0].y - food.y) < (food.r + 5) * (food.r + 5))
	{
		food.flag = false;
		snake.size+= food.r;
	}

	//如果食物消失 ，重新生成食物
	if (!food.flag)
	{
		srand(GetTickCount());
		food.x = rand() % 630+5;
		food.y = rand() % 470+5;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = rand() % 10 + 5;
		food.flag = true;
	}
}

//游戏暂停
void stop()
{
	while (1)
	{
		if (_getch() == ' ')
		{
			break;
		}
	}
}

void gamedraw()
{
	//双缓冲绘图
	BeginBatchDraw();
	//设置背景颜色
	setbkcolor(RGB(28, 115, 119));//1.设置颜色
	cleardevice();                //2.清空绘图设备

	//得分：
	char sc[6];
	sprintf(sc, "%d", (snake.size - 3)* (snake.size - 3));
	char score[20] = "score : ";
	strcat(score, sc);
	settextstyle(15, 0, "黑体");
	settextcolor(WHITE);
	outtextxy(500, 10, score);

	//绘制蛇
	setfillcolor(WHITE);
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y, 5);
	}

	//绘制食物
	if (food.flag)
	{
		setfillcolor(food.color);
		solidcircle(food.x, food.y, food.r);
	}
	EndBatchDraw();
}

//移动蛇
void snakemove()
{
	//让身体跟着头移动
	for (int i = snake.size-1; i >0; i--)
	{
		snake.coor[i] = snake.coor[i - 1];
	}

	//移动是坐标发生改变
	switch (snake.dir)
	{
	case UP:
		snake.coor[0].y-=snake.speed;
		/*if (snake.coor[0].y < 0)
		{
			snake.coor[0].y = 480 + snake.coor[0].y;
		}*/
		break;
	case DOWN:
		snake.coor[0].y+=snake.speed;
		/*if (snake.coor[0].y > 480)
		{
			snake.coor[0].y = snake.coor[0].y -480;
		}*/
		break;
	case LEFT:
		snake.coor[0].x-=snake.speed;
		/*if (snake.coor[0].x < 0)
		{
			snake.coor[0].x = 640 + snake.coor[0].x;
		}*/
		break;
	case RIGHT:
		snake.coor[0].x+=snake.speed;
		/*if (snake.coor[0].x > 640)
		{
			snake.coor[0].x = snake.coor[0].x - 640 ;
		}*/
		break;
	}
	
}

//通过按键改变蛇的移动方向
void keycontrol()
{
	if(_kbhit())  //检测有无按下按键 ， 如果有 ，返回TRUE
	{
		//上 下 左 右 键值
		//72 80 75 77
		switch (_getch())
		{
			//改变方向
		case 'w':
		case 'W':
		case 72:
			if (snake.dir != DOWN)
			{
				snake.dir = UP;
			}
			break;
		case 's':
		case 'S':
		case 80:
			if (snake.dir != UP)
			{
				snake.dir = DOWN;
			}
			break;
		case 'a':
		case 'A':
		case 75:
			if (snake.dir != RIGHT)
			{
				snake.dir = LEFT;
			}
			break;
		case 'd':
		case 'D':
		case 77:
			if (snake.dir != LEFT)
			{
				snake.dir = RIGHT;
			}
			break;
		case 'q':
		case 'Q':
			if(snake.speed>1)
			{
				snake.speed--;
			}
			break;
		case 'e':
		case 'E':
			snake.speed++;
			break;
		case ' ':
		{
			stop();
		}
		break;
		}
	}
}

int main()
{
	while(1)
	{
		startui();
		BGM();
		ExMessage msg;
		while (1)
		{
			
			if (peekmessage(&msg,EX_MOUSE))
			{
				if (msg.message == WM_LBUTTONDOWN && msg.x >= 195 && msg.x <= 400 && msg.y >= 390 && msg.y <= 450)
				{
					break;
				}
				else if (msg.message == WM_LBUTTONDOWN && msg.x >= 430 && msg.x <= 550 && msg.y >= 410 && msg.y <= 450)
				{
					goto
						gameend;
				}
			}
		}
		gameinit();
		while (1)
		{
			gamedraw();
			snakemove();
			
			int i = 8;
			for (; i < snake.size; i++)
			{
				if ((snake.coor[0].x == snake.coor[i].x && myabs(snake.coor[0].y - snake.coor[i].y) < 8)
					|| (snake.coor[0].y == snake.coor[i].y && myabs(snake.coor[0].x - snake.coor[i].x) < 8))
				{
					break;
				}
			}
			if (i<snake.size || snake.coor[0].x < 5 || snake.coor[0].x > 635 || snake.coor[0].y < 5 || snake.coor[0].y > 475)
			{
				HWND hnd = GetHWnd();
				MessageBox(hnd, "GAME OVER", "提示", MB_OK);
				break;
			}

			keycontrol();
			eatfood();
			Sleep(20);

		}
	}
	gameend:
	return 0;
}