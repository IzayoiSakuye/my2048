#include <stdio.h>
#include <graphics.h> // EasyX图形库头文件
#include <time.h> // 用于初始化随机数发生器
#include <conio.h> // 用于键盘操作

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
}grid_pos[SIZE][SIZE];
// 记录各格数字
int nums[SIZE][SIZE];
// 判断是否可以生成数字
bool flag = false;

// 随机产生数字
int randomNums() {
	time_t t1; // 用时间作为srand的seed，防止每次随机数都一样
	srand((unsigned)time(&t1)); // 初始化随机数发生器
	if (rand() % 8 == 1) return 4;
	else return 2;
}

// 把产生的数字放到随机位置
void randomPos() {
	while (1) {
		time_t t2;
		srand((unsigned)time(&t2));
		int x = rand() % SIZE;
		int y = rand() % SIZE;

		if (nums[x][y] == 0) {
			nums[x][y] = randomNums();
			break;
		}

	}

}

// 判断是否可以产生数字，若可以变产生数字
void numSummon() {
	if (flag) {
		randomPos();
		flag = false;
	}
}

// 判断是否胜利
bool isGameWin() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (nums[i][j] == 2048) {
				return true;
			}
		}
	}
	return false;
}

// 判断是否结束
bool isGameOver() {
	for (int i = 1; i < SIZE-1; i++) {
		for (int j = 1; j < SIZE-1; j++) {
			// 如果有空格子，游戏未结束
			if (nums[i][j] == 0) {
				return false;
			}
			// 检查当前格子与四个方向是否相等
			if (nums[i][j] == nums[i][j + 1]) {
				return false;
			}
			if (nums[i][j] == nums[i + 1][j]) {
				return false;
			}
			if (nums[i][j] == nums[i - 1][j]) {
				return false;
			}
			if (nums[i][j] == nums[i][j - 1]) {
				return false;
			}
		}
	}
	return true;
}

// 初始化游戏窗口
void initWindows() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT, EX_SHOWCONSOLE); // 初始化窗口
	setbkcolor(BK); // 设置窗口背景色
	cleardevice(); // 用背景色清除窗口

	// 预处理每个格子的坐标
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			grid_pos[i][j].x = i * GRID_WIDTH + (i + 1) * INTERVALS;
			grid_pos[i][j].y = j * GRID_WIDTH + (j + 1) * INTERVALS;
		}
	}
	
	// 随机生成两个数字(2或4)
	randomPos();
	randomPos();
	nums[2][2] = 1024, nums[2][3] = 1024;
}


// 绘制游戏窗口
void drawBoard() {
	// 双缓冲处理画面
	BeginBatchDraw(); // 开始批量绘图
	cleardevice(); // 清屏

	// 二维数组遍历填充格子颜色,初始化为没有数字的颜色
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			// 按照数字填充格子颜色
			enum color grid_color;
			switch (nums[i][j])
			{
			case 0: grid_color = ZERO; break;
			case 2: grid_color = TWO; break;
			case 4: grid_color = FOUR; break;
			case 8: grid_color = EIGHT; break;
			case 16: grid_color = SIXTE; break;
			case 32: grid_color = THI_TW; break;
			case 64: grid_color = SIX_FO; break;
			case 128: grid_color = OH_TW_EI; break;
			case 256: grid_color = TH_FI_SI; break;
			case 512: grid_color = FH_TWEL; break;
			case 1024: grid_color = OT_TW_FO; break;
			case 2048: grid_color = TT_FO_EI; break;
			default:
				break;
			}
			setfillcolor(grid_color); //设置填充颜色
			
			// 填充无边框矩形，注意二维坐标从左上角开始，函数四个参数分别为左上角xy与右下角xy
			solidrectangle(grid_pos[i][j].x,grid_pos[i][j].y,grid_pos[i][j].x+GRID_WIDTH,grid_pos[i][j].y+GRID_WIDTH);
			// 设置每一格的文字
			if (nums[i][j]) {
				char number[5]; // 储存数字字符串
				if (nums[i][j] == 2 || nums[i][j] == 4) settextcolor(RGB(119, 110, 101));  // 给2和4换一个深色
				settextstyle(40, 0, "Times New Roman"); // 设置字号与字体
				setbkmode(TRANSPARENT); // 设置文字背景色为透明
				sprintf(number, "%d", nums[i][j]); // 将数组中的整数变为字符串存储
				int text_width = GRID_WIDTH/2 - textwidth(number)/2; // 计算字符宽度
				int text_height = GRID_WIDTH / 2 - textheight(number) / 2; // 计算字符高度
				outtextxy(grid_pos[i][j].x + text_width, grid_pos[i][j].y + text_height, number); // 居中显示数字
			}

		}
	}
	EndBatchDraw(); // 结束批量绘图

	
}
// 左移
void left() {
	for (int j = 0; j < SIZE; j++) {
		int temp = 0;
		for (int i = 1; i < SIZE; i++) {
			if (nums[i][j] != 0) {
				if (nums[temp][j] == 0) {
					nums[temp][j] = nums[i][j];
					nums[i][j] = 0;
					flag = true;
				}
				else if (nums[temp][j] == nums[i][j]) {
					nums[temp][j] *= 2;
					nums[i][j] = 0;
					temp++;
					flag = true;
				}
				else {
					temp++;
					if (temp != i) {
						nums[temp][j] = nums[i][j];
						nums[i][j] = 0;
						flag = true;
					}
				}
			}
		}
	}
	printf("left\n");
}

// 右移
void right() {
	for (int j = 0; j < SIZE; j++) {
		int temp = SIZE - 1;
		for (int i = SIZE - 2; i >= 0; i--) {
			if (nums[i][j] != 0) {
				if (nums[temp][j] == 0) {
					nums[temp][j] = nums[i][j];
					nums[i][j] = 0;
					flag = true;
				}
				else if (nums[temp][j] == nums[i][j]) {
					nums[temp][j] *= 2;
					nums[i][j] = 0;
					temp--;
					flag = true;
				}
				else {
					temp--;
					if (temp != i) {
						nums[temp][j] = nums[i][j];
						nums[i][j] = 0;
						flag = true;
					}
				}
			}
		}
	}
	printf("right\n");
}

// 上移
void up() {
	for (int i = 0; i < SIZE; i++) {
		int temp = 0;
		for (int j = 1; j < SIZE; j++) {
			if (nums[i][j] != 0) {
				if (nums[i][temp] == 0) {
					nums[i][temp] = nums[i][j];
					nums[i][j] = 0;
					flag = true;
				}
				else if (nums[i][temp] == nums[i][j]) {
					nums[i][temp] *= 2;
					nums[i][j] = 0;
					temp++;
					flag = true;
				}
				else {
					temp++;
					if (temp != j) {
						nums[i][temp] = nums[i][j];
						nums[i][j] = 0;
						flag = true;
					}
				}
			}
		}
	}
	printf("up\n");
}

// 下移
void down() {
	for (int i = 0; i < SIZE; i++) {
		int temp = SIZE - 1;
		for (int j = SIZE - 2; j >= 0; j--) {
			if (nums[i][j] != 0) {
				if (nums[i][temp] == 0) {
					nums[i][temp] = nums[i][j];
					nums[i][j] = 0;
					flag = true;
				}
				else if (nums[i][temp] == nums[i][j]) {
					nums[i][temp] *= 2;
					nums[i][j] = 0;
					temp--;
					flag = true;
				}
				else {
					temp--;
					if (temp != j) {
						nums[i][temp] = nums[i][j];
						nums[i][j] = 0;
						flag = true;
					}
				}
			}
		}
	}
	printf("down\n");
}
// 控制数字移动
void stdControl() {
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY)) {
		// 从窗口中读取按键信息(wasd或WASD或方向键)
		if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('w') || GetAsyncKeyState('W')) {
			up();
		}
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('a') || GetAsyncKeyState('A')) {
			left();
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('s') || GetAsyncKeyState('S')) {
			down();
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('d') || GetAsyncKeyState('D')) {
			right();
		}
	}
	flushmessage(EX_KEY);
	
	//Sleep(100); // 防止过多的输入误触
}

// 游戏结果展示
void showResult(const char* message) {
	settextcolor(RGB(255, 0, 0)); // 设置颜色
	settextstyle(50, 0, "Times New Roman"); // 设置字号与字体
	setbkmode(TRANSPARENT); // 设置文字背景色为透明
	outtextxy(WINDOWS_WIDTH / 2 - textwidth(message) / 2, WINDOWS_HEIGHT / 2 - textheight(message) / 2 - 40, message);
	settextcolor(RGB(0, 0, 0)); // 设置颜色
	settextstyle(25, 0, "Times New Roman"); // 设置字号与字体
	const char TIP[100] = "Press \"R\" or \"r\" to restart, press \"Esc\" to quit.";
	outtextxy(WINDOWS_WIDTH / 2 - textwidth(TIP) / 2, WINDOWS_HEIGHT / 2 -textheight(TIP)/2, TIP);
}

// 游戏结束后的操作判断
bool overInput() {
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY)) {
		if (GetAsyncKeyState('R') || GetAsyncKeyState('r')) {
			closegraph();
			initWindows();
			return true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			closegraph();
			exit(0);
		}
	}
	flushmessage(EX_KEY);
	return false;
}

int main(){

	initWindows();
	while (1) {
		drawBoard();
		if (isGameWin()) {
			showResult("You Win!");
			while (!overInput()); // 等待用户操作
		}
		else if (isGameOver()) {
			showResult("Game Over");
			while (!overInput()); // 等待用户操作
		}
		else {
			stdControl();
			numSummon();
		}
	}
	

	//system("pause"); // 使窗口保持开启
	return 0;
}