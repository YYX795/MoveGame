#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#undef _UNICODE

#include <iostream>
#include<graphics.h>
#include<conio.h>
#include<thread>
#include<time.h>
using namespace std;

#define W 405
#define H 500
#define GRID 50
const int LEFT = 100;
const int TOP = 130;
int TIME = 200;

class Frame
{
public:
	Frame() :left(0), top(0), grid(GRID), lineColor(RGB(255, 255, 255)) {}
	//设置距左侧的长度
	void SetLeftMargin(int left) {
		this->left = left;
	}
	//设置距上方的长度
	void SetTopMargin(int top) {
		this->top = top;
	}
	//设置格子大小
	void SetGridMargin(int grid) {
		this->grid = grid;
	}
	//设置线框
	void SetFrame(int left, int top, int grid) {
		this->left = left;
		this->top = top;
		this->grid = grid;
	}
	//设置颜色
	void SetColorMargin(COLORREF color) {
		this->lineColor = color;
	}
	//绘制线框
	void Draw() {
		COLORREF old = getlinecolor();
		setlinecolor(lineColor);
		for (int i = 0; i < 5; i++) {
			line(left, top + i * grid, left + grid * 4, top + i * grid);
			line(left + i * grid, top, left + i * grid, top + 4 * grid);
		}
		setlinecolor(old);
	}
private:
	int left;//距左侧的长度
	int top;//距上方的长度
	int grid;//格子大小
	COLORREF lineColor;//线条颜色
};

class Number
{
public:
	Number(int n = 0) :num(n), cX(0), cY(0) {
		short rgb[3];
		int i = 0;
		for (i = 0; i < 3; i++) {
			rgb[i] = 50 + rand() % 200;
		}
		for (i = 0; i < 3; i++) {
			if (rgb[i] > 100) {
				break;
			}
		}
		if (i == 3) {
			rgb[rand() % 3] += 150;
		}
		color = RGB(rgb[0], rgb[1], rgb[2]);
		radius = (int)(0.4 * GRID);
	}
	//获取位置
	int GetX() {
		return cX;
	}
	int GetY() {
		return cY;
	}
	//设置位置
	void SetPostion(int x, int y) {
		cX = x;
		cY = y;
	}
	//获取颜色
	COLORREF GetColor() {
		return color;
	}
	//设置颜色
	void SetColor(COLORREF color) {
		this->color = color;
	}
	//获取数字
	int GetNum() {
		return num;
	}
	//设置数字
	void SetNum(int num) {
		this->num = num;
	}
	//绘制
	void Draw() {
		setfillcolor(color);
		fillcircle(cX, cY, radius);
		RECT rc;
		rc.left = cX - radius;
		rc.right = cX + radius;
		rc.top = cY - radius;
		rc.bottom = cY + radius;
		char str[3];
		itoa(num, str, 10);
		COLORREF old = gettextcolor();
		settextcolor(BLACK);
		drawtext(str, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		settextcolor(old);

	}
	//擦除
	void Clear() {
		setfillcolor(BLACK);
		bar(cX - radius, cY - radius, cX + radius, cY + radius);
	}

private:
	int cX;//圆心X
	int cY;//圆心Y
	int num;//数字
	int radius;//半径
	COLORREF color;//颜色
};

bool Win(Number* n) {
	for (int i = 0; i < 16; i++) {
		if (n[i].GetNum() != i + 1) {
			return false;
		}
	}
	return true;
}

void SwapNum(Number& a, Number& b) {
	Number num = a;
	a.SetColor(b.GetColor());
	a.SetNum(b.GetNum());
	b.SetColor(num.GetColor());
	b.SetNum(num.GetNum());
}

void Start() {
	srand((unsigned int)time(0));
	initgraph(W, H);
	SetWindowText(GetHWnd(), TEXT("Game"));
	setbkmode(TRANSPARENT);
	Frame frame;
	frame.SetLeftMargin(LEFT);
	frame.SetTopMargin(TOP);
	frame.SetColorMargin(RGB(0, 244, 244));
	frame.Draw();
BEGIN:
	Number num[16];
	for (int i = 0; i < 16; i++) {
		num[i].SetNum(i + 1);
		num[i].SetPostion((int)(LEFT + (0.5 + i % 4) * GRID), (int)(TOP + (0.5 + i / 4) * GRID));
	}
	int blank = 15;
	int target = 0;
	int random = 0;
	int last_target = 0;
	for (int i = 0; i < TIME; i++) {
		switch (blank) {
		case 0:
			target = (rand() % 2) ? 1 : 4;
			if (last_target == -target) {
				target = 5 - target;
			}
			break;
		case 3:
			target = (rand() % 2) ? -1 : 4;
			if (last_target == -target) {
				target = 3 - target;
			}
			break;
		case 12:
			target = (rand() % 2) ? 1 : -4;
			if (last_target == -target) {
				target = -3 - target;
			}
			break;
		case 15:
			target = (rand() % 2) ? -1 : -4;
			if (last_target == -target) {
				target = -5 - target;
			}
			break;
		case 1:
		case 2:
			random = rand() % 3;
			target = random == 0 ? -1 : (random == 1 ? 1 : 4);
			break;
		case 4:
		case 8:
			random = rand() % 3;
			target = random == 0 ? -4 : (random == 1 ? 1 : 4);
			break;
		case 7:
		case 11:
			random = rand() % 3;
			target = random == 0 ? -4 : (random == 1 ? -1 : 4);
			break;
		case 13:
		case 14:
			random = rand() % 3;
			target = random == 0 ? -4 : (random == 1 ? -1 : 1);
			break;
		default:
			random = rand() % 4;
			target = (random & 2) ? (random & 1 ? -4 : -1) : (random & 1 ? 1 : 4);
			if (last_target == -target) {
				target *= -1;
			}
			break;
		}
		SwapNum(num[blank], num[blank + target]);
		blank += target;
		last_target = target;
	}
	for (int i = 0; i < 16; i++) {
		if (num[i].GetNum() != 16) {
			num[i].Draw();
		}
		else {
			num[i].Clear();
		}
	}
	int key;
	while (key = _getch()) {
		if (key == 224) {
			key = _getch();
			if (key == 72) {//上
				if (blank <= 11) {
					SwapNum(num[blank], num[blank + 4]);
					num[blank].Draw();
					num[blank + 4].Clear();
					blank += 4;
					if (blank == 15 && num[11].GetNum() == 12 && Win(num)) {
						MessageBox(GetHWnd(), "Win", "Win", MB_OK);
						goto BEGIN;
					}
				}
			}
			else if (key == 75) {//左
				if (blank % 4 != 3) {
					SwapNum(num[blank], num[blank + 1]);
					num[blank].Draw();
					num[blank + 1].Clear();
					blank += 1;
					if (blank == 15 && num[14].GetNum() == 15 && Win(num)) {
						MessageBox(GetHWnd(), "Win", "Win", MB_OK);
						goto BEGIN;
					}
				}
			}
			else if (key == 77) {//右
				if (blank % 4 != 0) {
					SwapNum(num[blank], num[blank - 1]);
					num[blank].Draw();
					num[blank - 1].Clear();
					blank -= 1;
				}
			}
			else if (key == 80) {//下
				if (blank >= 4) {
					SwapNum(num[blank], num[blank - 4]);
					num[blank].Draw();
					num[blank - 4].Clear();
					blank -= 4;
				}
			}
		}
	}
}

int main()
{
	Start();
	return 0;
}