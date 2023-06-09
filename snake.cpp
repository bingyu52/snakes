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
//�ߵĽṹ��
struct Snake {
	int size; //�ߵĽ���
	int dir;  //�ߵķ���
	int speed;//�ߵ��ٶ�
	POINT coor[SNAKE_NUM];//����
}snake;

struct Food
{
	int x;        //ʳ�������
	int y;
	int r;        //ʳ��İ뾶
	bool flag;    //ʳ���Ƿ񱻳Ե�
	DWORD color;  //ʳ�����ɫ
}food;
//���ݳ�ʼ��
void gameinit()
{
	//��ʼ��ͼ�δ���
	//initgraph(640, 480);

	srand(GetTickCount());
	//һ��ʼ��3��
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
	//��ʼ��ͼ�δ���
	initgraph(640, 480);
	//˫�����ͼ
	//BeginBatchDraw();
	//���ñ�����ɫ
	setbkcolor(RGB(28, 115, 119));//1.������ɫ
	cleardevice();                //2.��ջ�ͼ�豸
	IMAGE img;
	loadimage(&img, "./image//ui.jpg", 640, 480);
	putimage(0, 0, &img);
}

void eatfood()
{
	//��ͷ��Բ�ĺ�ʳ���Բ��֮��ľ���С�ڰ뾶�� ���Ե�ʳ��
	if (food.flag==true &&(snake.coor[0].x - food.x) * (snake.coor[0].x - food.x) +
		(snake.coor[0].y - food.y) * (snake.coor[0].y - food.y) < (food.r + 5) * (food.r + 5))
	{
		food.flag = false;
		snake.size+= food.r;
	}

	//���ʳ����ʧ ����������ʳ��
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

//��Ϸ��ͣ
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
	//˫�����ͼ
	BeginBatchDraw();
	//���ñ�����ɫ
	setbkcolor(RGB(28, 115, 119));//1.������ɫ
	cleardevice();                //2.��ջ�ͼ�豸

	//�÷֣�
	char sc[6];
	sprintf(sc, "%d", (snake.size - 3)* (snake.size - 3));
	char score[20] = "score : ";
	strcat(score, sc);
	settextstyle(15, 0, "����");
	settextcolor(WHITE);
	outtextxy(500, 10, score);

	//������
	setfillcolor(WHITE);
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y, 5);
	}

	//����ʳ��
	if (food.flag)
	{
		setfillcolor(food.color);
		solidcircle(food.x, food.y, food.r);
	}
	EndBatchDraw();
}

//�ƶ���
void snakemove()
{
	//���������ͷ�ƶ�
	for (int i = snake.size-1; i >0; i--)
	{
		snake.coor[i] = snake.coor[i - 1];
	}

	//�ƶ������귢���ı�
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

//ͨ�������ı��ߵ��ƶ�����
void keycontrol()
{
	if(_kbhit())  //������ް��°��� �� ����� ������TRUE
	{
		//�� �� �� �� ��ֵ
		//72 80 75 77
		switch (_getch())
		{
			//�ı䷽��
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
				MessageBox(hnd, "GAME OVER", "��ʾ", MB_OK);
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