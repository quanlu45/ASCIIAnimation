// ConsoleApplication2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

char map[10] = " .,:+*o@";
COORD  position;


void paint(HBITMAP hbmp)
{

	BITMAP bmp;
	unsigned char *px = NULL, *px2 = NULL, *p = NULL;
	int i, j, rgb_b, pixBytes,bmp_size;

	/*取得图片像素值*/
	GetObjectA(hbmp, sizeof(BITMAP), &bmp);
	bmp_size = bmp.bmHeight*bmp.bmWidthBytes;
	px = (unsigned char *)malloc(bmp_size * sizeof(unsigned char));
	GetBitmapBits(hbmp, bmp_size, px);

	/*求图片近似灰度*/
	pixBytes = bmp.bmBitsPixel / 8;
	px2 = (unsigned char *)malloc((bmp_size/pixBytes + 1) * sizeof(unsigned char));

	p = px2;
	for (i = 0; i<bmp.bmHeight; ++i)
		for (j = 0; j < bmp.bmWidth; ++j)
		{
			rgb_b = i * bmp.bmWidthBytes + j * pixBytes;
			*px2 = (px[rgb_b] * 2 + px[rgb_b + 1] * 5 + px[rgb_b + 2]) / 8;
			*px2 = map[(255 - *px2) * 10 / 256];
			px2++;
		}
	free(px);

	/*匹配字符，扫描*/
	for (i = 0; i < bmp.bmHeight; ++i)
	{
		for (j = 0; j < bmp.bmWidth; ++j)
			putchar(p[i*bmp.bmWidth + j]);
		printf("\n");
	}
	free(p);
}
/*设置光标起始位置减少闪烁*/
void set_cur_position(HANDLE hwnd)
{
	if(!SetConsoleCursorPosition(hwnd, position)) exit(0);
}

/*隐藏光标*/
void hide_cursor(HANDLE handle ,CONSOLE_CURSOR_INFO cursorinfo)
{
	cursorinfo.bVisible = false;
	SetConsoleCursorInfo(handle, &cursorinfo);
}

int main(void)
{
	HBITMAP hbmp[7];
	HANDLE handle;
	char name[20];
	char window_title[] = "q0q";
	CONSOLE_CURSOR_INFO cursorinfo;
	SetConsoleTitleA(window_title);
	handle = GetStdHandle(STD_OUTPUT_HANDLE); //得到控制台缓冲区句柄
	GetConsoleCursorInfo(handle, &cursorinfo);//得到光标属性
	system("mode con cols=150 lines=150"); //设置控制台窗口大小

	position.X = 0;
	position.Y = 0;


	/*加载图片*/
	for (int i = 0; i < 7; ++i)
	{
		sprintf_s(name, "imgs/girl%d.bmp", i);
		hbmp[i] = (HBITMAP)LoadImageA(NULL, name, IMAGE_BITMAP, 150, 50, LR_LOADFROMFILE);
		if (!hbmp)
		{
			printf("加载图片失败,5s后关闭");
			Sleep(5000);
			exit(0);
		}
	}
	
	/*循环播放*/
	for (int i = 0; i < 7; ++i)
	{
		paint(hbmp[i]);
		set_cur_position(handle);
		hide_cursor(handle, cursorinfo);
		//Sleep(100);
		//system("CLS");
		if (i == 6) i = 0;
	}
	getchar();
	return 0;
}