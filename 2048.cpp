#include <stdio.h>
#include <graphics.h> // EasyX图形库头文件


// 宏定义窗口尺寸数据
#define GRID_WIDTH 100 // 格子宽度
#define INTERVALS 15 // 间隔大小
#define SIZE 4 // 格子数量
#define WINDOWS_WIDTH (GRID_WIDTH*SIZE+INTERVALS*(SIZE+1)) // 窗口宽
#define WINDOWS_HEIGHT (GRID_WIDTH*SIZE+INTERVALS*(SIZE+1))+120 //窗口高

// 用枚举类型设置各种数字以及背景颜色
enum color {
	BK = RGB(255, 224, 172), // 背景色
	ZERO = RGB(205,193,180), // 没有数字
	TWO = RGB(238,228,218), // 2
	FOUR = RGB(237,224,200), // 4
	EIGHT = RGB(242,177,121), // 8
	SIXTE = RGB(245,149,99), // 16
	THI_TW = RGB(246,124,95), // 32
	SIX_FO = RGB(246,94,59), // 64
	OH_TW_EI = RGB(242,177,121), // 128
	TH_FI_SI = RGB(237,204,97), // 256
	FH_TWEL = RGB(255,0,128), // 512
	OT_TW_FO = RGB(145,0,72), // 1024
	TT_FO_EI = RGB(242,17,158), // 2048

};
// 记录坐标
struct pos {
	long long x;
	long long y;
}grid_position[SIZE][SIZE];
// 记录各格数字
int nums[SIZE][SIZE];

// 初始化游戏窗口
void initWindows() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT, EX_SHOWCONSOLE); // 初始化窗口
	setbkcolor(BK); // 设置窗口背景色
	cleardevice(); // 用背景色清除窗口

	// 预处理每个格子的坐标
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			grid_position[i][j].x = i * GRID_WIDTH + (i + 1) * INTERVALS;
			grid_position[i][j].y = j * GRID_WIDTH + (j + 1) * INTERVALS;
		}
	}
}

// 绘制游戏窗口
void drawBoard() {
	// 二维数组遍历填充格子颜色,初始化为没有数字的颜色
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			// 按照数字填充格子颜色
			enum color grid_color;
			switch (nums[i][j])
			{
			case 0: grid_color = ZERO;
			case 2: grid_color = TWO;

			default:
				break;
			}
			setfillcolor(ZERO); //设置填充颜色
			// 填充无边框矩形，注意二维坐标从左上角开始，函数四个参数分别为左上角xy与右下角xy
			solidrectangle(grid_position[i][j].x,grid_position[i][j].y,grid_position[i][j].x+GRID_WIDTH,grid_position[i][j].y+GRID_WIDTH);

		}
	}

	
}
int main(){
	initWindows();
	drawBoard();

	system("pause"); // 使窗口保持开启
	return 0;
}