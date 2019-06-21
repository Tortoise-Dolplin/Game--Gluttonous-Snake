#include "acllib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
const int winwidth = 600, winheight = 480;
const char *Gluttonous_Snake = "̰���� (2).bmp";
const char *cover = "̰���߷���.bmp";
const char *chose_difficulty = "ѡ���Ѷ�ҳ��.bmp";
ACL_Image img;
ACL_Image picture;
ACL_Image photo;
ACL_Image img2;
ACL_Sound begin_bgm;
ACL_Sound chose_difficulty_bgm;
ACL_Sound get_food_bgm;
ACL_Sound collision_bgm;
int i, score = 0;
int speed = 0, choice = 0;
int iflife = 1;//��ʼ����״̬Ϊ��1��ʾ
typedef struct SNAKE
{
	int x[2000];
	int	y[2000];
	int length;//�ߵĳ��� 
	int direction;//�ߵ��ƶ�����
} snake;
typedef struct FOOD 
{
	int x,y;//ʳ���λ��;
	int exist;//�ж��Ƿ�Ҫ����ʳ��; 
} food;//����ʳ��Ľṹ��
food Food;
snake Snake;
void operation(food *m, snake *n);
void paint(food *m, snake *n);
void keyEvent(int key, int e);
void timer(int id);
void initial(void);
void gameover();
int foodnum = 0;
int foodlevel;
int gamelevel;
int storespeed=100;
int Setup()
{
	initWindow("̰����", DEFAULT, DEFAULT, winwidth, winheight); //����Ϸ����
	loadImage(cover, &picture);
	beginPaint();
	putImageScale(&picture, 0, 0, winwidth, winheight);//̰���߷���
	endPaint();
	loadSound("��Ϸ��ʼ��Ч.wav", &begin_bgm);
	playSound(begin_bgm, 1);
	registerKeyboardEvent(keyEvent);
	loadImage(Gluttonous_Snake, &img);
	loadImage(chose_difficulty, &photo);
	loadSound("ѡ���ѶȽ���.wav", &chose_difficulty_bgm);
	loadSound("̰���߳Ե�ʳ��.wav", &get_food_bgm);
	loadSound("��Ϸ������Ŷ.wav", &collision_bgm);
	return 0;
}
void operation(food *p, snake *q)//��Ϸ����
{
	srand((unsigned)time(NULL));
	if (Food.exist != 0)  //��һ�γ���ʳ��
	{
		Food.x = rand() % 531 + 30; //�������ʳ���λ��
		Food.y = rand() % 411 + 30; 
		while (Food.x % 10 != 0)
		{
			Food.x++; //�ж�ʳ���x�����Ƿ�Ϊ���������Ǿ͵��� 
		}
		while (Food.y % 10 != 0)
		{
			Food.y++; 
		}
		Food.exist = 0; //�����Ϊ���������ʳ�� 
	}
	for (i = Snake.length - 1; i > 0; i--) 
	{
		Snake.x[i] = Snake.x[i - 1]; //����ǰ�ƶ��ˣ�ǰһ�����������һ������
		Snake.y[i] = Snake.y[i - 1];
	}
	switch (Snake.direction)  //�ж���ͷ���ƶ�����1234��ʾ�������� 
	{//�ƶ�ʮ��
    case 1: Snake.x[0] += 10; break; 
	case 2: Snake.x[0] -= 10; break;
	case 3: Snake.y[0] -= 10; break;
	case 4: Snake.y[0] += 10; break;
	default: break;
	}
	for (i = 3; i < Snake.length; i++)
	{
		if (Snake.x[i] == Snake.x[0] && Snake.y[i] == Snake.y[0])//��ҧ���Լ�
		{
			playSound(collision_bgm, 0);
			iflife = 0;//��״̬Ϊ��
		}
	}
	if (Snake.x[0] < 40 || Snake.x[0]>winwidth-40 || Snake.y[0] < 30 || Snake.y[0]>winheight-30)//��ײǽ
	{
		playSound(collision_bgm, 0);
		iflife = 0;//��״̬Ϊ��
	}
	if (Snake.x[0] == Food.x&&Snake.y[0] == Food.y)//�߳Ե�ʳ��
	{
		foodnum += 1;
		playSound(get_food_bgm, 0);
		beginPaint();
		setPenColor(0);
		rectangle(Food.x, Food.y, Food.x + 10, Food.y + 10);
		endPaint();
		Food.exist = 1; //�����µ�ʳ��
		if (foodlevel == 1)
		{
			score += 10;
		}
		Snake.length++; //�ߵ�����ӳ�һ��
		if (foodlevel == 2)
		{
			score += 50;
		}
	}
}
void paint(food *m, snake *n)
{
	beginPaint();
	putImageScale(&img, 0, 0, winwidth, winheight);
	if (foodnum % 5 != 0||foodnum==0)
	{
		setBrushColor(RED);
		rectangle(Food.x, Food.y, Food.x + 10, Food.y + 10); //��ʳ��
		foodlevel = 1;
	}
	else
	{
		setBrushColor(BLACK);
		rectangle(Food.x, Food.y, Food.x + 10, Food.y + 10); //��ʳ��
		foodlevel = 2;
	}
	setBrushColor(BLUE);
	setPenColor(BLUE);
	rectangle(Snake.x[0], Snake.y[0], Snake.x[0] + 10, Snake.y[0] + 10);
	setBrushColor(BLACK);
	setPenColor(BLACK);
	for (i = 1; i < Snake.length; i++)//���� 
	{
		rectangle(Snake.x[i], Snake.y[i], Snake.x[i] + 10, Snake.y[i] + 10);
	} 
	setPenColor(WHITE);
	setBrushColor(WHITE);
	rectangle(Snake.x[Snake.length - 1], Snake.y[Snake.length - 1], Snake.x[Snake.length - 1] + 10, Snake.y[Snake.length - 1] + 10);
	endPaint();
}
void keyEvent(int key, int e)//ͨ���������Ұ��������ߵ��ƶ�����
{
	if (e != KEY_DOWN)
	{
		return;
	}
	else
	{
		switch (key)
		{
		case 0x52:
			stopSound(begin_bgm);
			playSound(chose_difficulty_bgm, 1);
			beginPaint();
			putImageScale(&photo, 0, 0, winwidth, winheight);//̰����ѡ���ѶȽ���
			endPaint();
			break;
		case VK_ESCAPE: startTimer(0, 10000000); break;
		case 0x53: startTimer(0, speed); break;
		case 0x31: speed = 200; initial(); break;
		case 0x61: speed = 200; initial(); break;
		case 0x32: speed = 150; initial(); break;
		case 0x62: speed = 150; initial(); break;
		case 0x33: speed = 100; initial(); break;
		case 0x63: speed = 100; initial(); break;
		case 0x34: speed = 30; initial(); break;
		case 0x64: speed = 30; initial(); break;
		case 0x42:
		{
			score = 0;
			cancelTimer(0);
			beginPaint();
			clearDevice();
			putImageScale(&photo, 0, 0, winwidth, winheight);
			endPaint();
			playSound(chose_difficulty_bgm, 1);
			iflife = 1;//�޸�����״̬
		}
		break;
		case VK_UP:
			if (Snake.direction != 4)
			{
				Snake.direction = 3;
				break;
			}
		case VK_DOWN:
			if (Snake.direction != 3)
			{
				Snake.direction = 4;
				break;
			}
		case VK_LEFT:
			if (Snake.direction != 1)
			{
				Snake.direction = 2;
				break;
			}
		case VK_RIGHT:
			if (Snake.direction != 2)
			{
				Snake.direction = 1;
				break;
			}
		}
	}
}
void timer(int id) 
{
	if (iflife == 1)//���ż�����ͼ��������Ϸ����
	{
		operation(&Food, &Snake);
		paint(&Food, &Snake);
		beginPaint();
		setTextColor(RED);
		setTextBkColor(GREEN);
		setTextSize(30);
		paintText(100, 0, "��ESC����ͣ��S��������Ϸ");
		setTextColor(RED);
		setTextBkColor(GREEN);
		char str[50];
		sprintf_s(str, "score:%d", score);
		setTextSize(30);
		paintText(440, 450, str);
		endPaint();
	}
	else
	{
		gameover();
	}
}
void gameover()
{
	beginPaint();
	clearDevice();
	putImageScale(&img, 0, 0, winwidth, winheight);
	setTextColor(RED);
	setTextBkColor(GREEN);
	setTextSize(50);
	paintText(200, 200, "��Ϸ����");
	paintText(100, 350, "��B�����¿�ʼ��Ϸ");
	char str[50];
	sprintf_s(str, "score:%d", score);
	setTextSize(30);
	paintText(230, 300, str);
	endPaint();
}
void initial(void)
{
	stopSound(chose_difficulty_bgm);
	Food.exist = 1;//��ʼ��һ��ʼû��ʳ��
	Snake.length = 5; //һ��ʼ�ߵĽ���
	Snake.direction = 1; //һ��ʼ�ߵķ���������
	for (i = 0; i < Snake.length; ++i)//һ��ʼ�ߵ�λ�����м䲿��
	{
		Snake.x[i] = 150 - 10 * i;
		Snake.y[i] = 240;
	}
	registerTimerEvent(timer);
	startTimer(0, speed);
}