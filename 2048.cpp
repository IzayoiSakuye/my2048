#include <stdio.h>
#include <graphics.h> // EasyX图形库头文件
#include <time.h> // 用于初始化随机数发生器
#include <conio.h> // 用于键盘操作
#include <string.h> // 用于字符数组操作

/*宏定义全局常量*/
// 布尔表达
#define true 1
#define false 0
// 宏定义窗口尺寸数据
#define GRID_WIDTH 100 // 格子宽度
#define INTERVALS 15 // 间隔大小
#define SIZE 4 // 格子数量
#define WINDOWS_WIDTH (GRID_WIDTH*SIZE+INTERVALS*(SIZE+1)) // 窗口宽
#define WINDOWS_HEIGHT (GRID_WIDTH*SIZE+INTERVALS*(SIZE+1))+200 //窗口高
// 宏定义结果显示框尺寸
#define RECWIDTH 400
#define RECHEIGHT 400
// 宏定义玩家名字长度
#define NAME 50

// 用枚举类型设置各种数字以及背景颜色
enum color {
	REC = RGB(187, 173, 160), // 矩形色
	BK = RGB(255, 224, 172), // 背景色
	ZERO = RGB(205, 193, 180), // 没有数字
	TWO = RGB(238, 228, 218), // 2
	FOUR = RGB(237, 224, 200), // 4
	EIGHT = RGB(242, 177, 121), // 8
	SIXTE = RGB(245, 149, 99), // 16
	THI_TW = RGB(246, 124, 95), // 32
	SIX_FO = RGB(246, 94, 59), // 64
	OH_TW_EI = RGB(238, 206, 106), // 128
	TH_FI_SI = RGB(237, 204, 97), // 256
	FH_TWEL = RGB(255, 0, 128), // 512
	OT_TW_FO = RGB(145, 0, 72), // 1024
	TT_FO_EI = RGB(242, 17, 158), // 2048
};

/*定义其他全局变量*/

// 使用链表记录玩家信息
typedef struct Player {
	char name[NAME];
	int score;
	struct Player *next;
} Player;
Player *ranking_head = NULL; // 初始化链表头

// 记录坐标
struct pos {
	long long x;
	long long y;
} grid_pos[SIZE][SIZE];
// 常用文件指针
FILE *score_f;
FILE *history;
FILE *gamestate;
// 记录各格数字
int nums[SIZE][SIZE];
// 临时记录分数
int score;
// 临时记录玩家名
char cur_name[NAME];
// 判断是否可以生成数字
int flag = false;
int flaghistory = false;

/*随机生成新数字用到的函数*/
// 随机产生数字
int randomNums() {
	time_t t1; // 用时间作为srand的seed，防止每次随机数都一样
	srand((unsigned)time(&t1)); // 初始化随机数发生器
	if (rand() % 8 == 1)
		return 4; // 20%的概率产生4
	else
		return 2; // 80%的概率产生2
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

// 判断是否可以产生数字，若有数字移动则产生数字
void numSummon() {
	if (flag) {
		randomPos();
		flag = false;
	}
}

/*游戏记录存储与读取相关函数*/
// 存储游戏记录
void saveGameState() {
	gamestate = fopen("game_state.txt", "w");
	fprintf(gamestate, "%d\n", flaghistory);// 保存历史记录状态
	fprintf(gamestate, "%s\n", cur_name);// 保存玩家姓名
	fprintf(gamestate, "%d\n", score);// 保存分数

	// 保存棋盘上的数字
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			fprintf(gamestate, "%d ", nums[i][j]);
		}
		fprintf(gamestate, "\n");
	}
	fclose(gamestate);
}

// 获取最高分数
int getScore(const char *i_name) {
	FILE *file = fopen("ranking.txt", "r");
	if (!file) {
		// 如果文件不存在，返回默认值
		return 0;
	}
	char name[NAME];
	int score = 0;
	int highest_score = 0;

	// 遍历文件内容
	while (fscanf(file, "%s %d", name, &score) == 2) {
		if (strcmp(name, i_name) == 0) {
			if (score > highest_score) {
				highest_score = score;
				break;
			}
		}
	}

	fclose(file);
	return highest_score;
}

// 载入历史数据
int loadSaveData() {
	int state = 0;
	gamestate = fopen("game_state.txt", "r");
	if (!gamestate) {
		// 文件不存在，初始化状态为未加载
		state = 0;
		return state;
	}
	fscanf(gamestate, "%d\n", &state); // 读取第一行的历史记录标志
	fclose(gamestate);
	// 如果有历史记录
	if (state == 1) {
		// 弹出窗口询问用户是否要加载历史记录
		setfillcolor(RGB(0, 0, 0)); // 设置填充颜色
		int x1 = (WINDOWS_WIDTH - RECWIDTH) / 2;
		int y1 = (WINDOWS_HEIGHT - RECHEIGHT) / 2;
		int x2 = x1 + RECWIDTH;
		int y2 = y1 + RECHEIGHT - 100;
		solidrectangle(x1 - 5, y1 - 5, x2 + 5, y2 + 5); // 填充显示结果矩形
		setfillcolor(BK);
		solidrectangle(x1, y1, x2, y2);

		// 提示信息
		settextcolor(RGB(255, 0, 0)); // 设置颜色
		settextstyle(35, 0, "黑体");
		setbkmode(TRANSPARENT); // 设置文字背景色为透明
		const char *message = "是否从上次的进度开始？";
		outtextxy(x1 + (RECWIDTH - textwidth(message)) / 2, y1 - 100 + RECHEIGHT / 2 - textheight(message), message);

		// 提示按键
		settextcolor(RGB(0, 0, 0)); // 设置颜色
		settextstyle(25, 0, "黑体");
		const char *TIP1 = "按\"Y\"或\"y\"读取进度";
		const char *TIP2 = "按\"N\"或\"n\"重新开始";
		outtextxy(x1 + (RECWIDTH - textwidth(TIP1)) / 2, y1 + RECHEIGHT / 2 - textheight(TIP1), TIP1);
		outtextxy(x1 + (RECWIDTH - textwidth(TIP2)) / 2, y1 + 50 + RECHEIGHT / 2 - textheight(TIP2), TIP2);

		// 等待用户输入
		ExMessage msg;
		while (1) {
			if (peekmessage(&msg, EX_KEY)) {
				// 加载历史记录
				if (GetAsyncKeyState('Y') || GetAsyncKeyState('y')) {
					gamestate = fopen("game_state.txt", "r");
					int state = 0;
					fscanf(gamestate, "%d\n", &state); // 读取状态
					fscanf(gamestate, "%s\n", cur_name); // 读取昵称
					fscanf(gamestate, "%d\n", &score); // 读取分数

					// 读取棋盘数据
					for (int i = 0; i < SIZE; i++) {
						for (int j = 0; j < SIZE; j++) {
							fscanf(gamestate, "%d ", &nums[i][j]);
						}
					}
					fclose(gamestate);
					flaghistory = state; // 恢复历史状态
					break;
				}
				// 不加载，开始新游戏
				else if (GetAsyncKeyState('N') || GetAsyncKeyState('n')) {
					state = 0;
					break;
				}
			}
			flushmessage(EX_KEY);
		}
	} else {
		return state; // 不需要加载进度 直接重新开始
	}
	return state;
}

/*排名相关函数*/
// 加载排名
void loadRankings() {
	FILE *file = fopen("ranking.txt", "r");
	if (!file) return; // 不存在文件则返回
	char name[NAME]; // 临时记录昵称
	int l_score = 0; // 临时记录分数

	while (fscanf(file, "%s %d", name, &l_score) == 2) { // 返回成功匹配与赋值的个数
		Player *new_node = (Player *)malloc(sizeof(Player)); // 创建一个链表用于读取文件中的数据并开辟空间
		strcpy(new_node->name, name); // 将读取到的名字存入链表
		new_node->score = l_score; // 将读取到的分数存入排名链表
		new_node->next = ranking_head; // 将排名链表头移到下一位
		ranking_head = new_node; // 当前位置填入数据
	}
	fclose(file);
}

// 保存排名
void saveRankings() {
	FILE *file = fopen("ranking.txt", "w");
	if (!file)
		return; // 不存在文件则返回
	Player *current = ranking_head; // 创建临时链表并指向当前排名链表头

	// 将排名链表中的数据写入文件并同时移动临时链表
	while (current) {
		fprintf(file, "%s %d\n", current->name, current->score);
		current = current->next;
	}
	fclose(file);
}

// 冒泡排序对链表排序
void listSort(Player *head) {
	if (head == NULL || head->next == NULL) {
		return; // 链表为空或只有一个节点，无需排序
	}

	Player *tail = NULL; // 每轮冒泡排序的边界
	Player *cur;

	while (head != tail) {
		cur = head;
		while (cur->next != tail) {
			if (cur->score < cur->next->score) { // 从高到低排序
				// 交换分数
				int tempScore = cur->score;
				cur->score = cur->next->score;
				cur->next->score = tempScore;

				// 交换名字
				char tempName[50];
				strcpy(tempName, cur->name);
				strcpy(cur->name, cur->next->name);
				strcpy(cur->next->name, tempName);
			}
			cur = cur->next;
		}
		tail = cur; // 更新尾部边界
	}
}

// 插入(新的)或更新(已有)排名函数
void insertRanking(const char *i_name, int i_score) {
	Player *current = ranking_head;
	Player *prev = NULL;

	// 检查是否已经存在同名玩家
	while (current != NULL) {
		if (strcmp(current->name, i_name) == 0) {
			// 如果当前分数更高，更新分数
			if (current->score < i_score) {
				current->score = i_score;
			}
			// 排序链表
			listSort(ranking_head);
			return;
		}
		prev = current;
		current = current->next;
	}

	// 如果没有找到同名玩家，创建新节点
	Player *new_node = (Player *)malloc(sizeof(Player));
	// 将数据输入新节点
	strncpy(new_node->name, i_name, sizeof(new_node->name) - 1);
	new_node->name[sizeof(new_node->name) - 1] = '\0';
	new_node->score = i_score;
	new_node->next = NULL;

	// 插入到链表尾部
	if (!ranking_head) {
		ranking_head = new_node; // 如果链表为空则放在头部
	} else {
		prev->next = new_node; // 否则当前链表下一位
	}
	// 排序链表
	listSort(ranking_head);
}

// 显示排名
int displayRankings() {
	// 双缓冲
	BeginBatchDraw(); // 开始批量绘图
	cleardevice(); // 清屏

	// 显示标题
	settextcolor(BLACK);
	settextstyle(40, 0, "黑体");
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("排名") / 2, 20, "排名");

	// 遍历链表显示前10名玩家
	settextstyle(20, 0, "黑体");
	Player *current = ranking_head; // 结构体指向排名头
	int rank_h = 80; // 控制文字显示位置
	int rank = 1; // 显示排名
	while (current != NULL && rank <= 10) {
		char display_text[100];
		sprintf(display_text, "No%d. %s ------- %d分", rank, current->name, current->score); // 将链表中存储的数据输出
		outtextxy(50, rank_h, display_text);
		rank_h += 30; // 每次+30高以显示下一名
		rank++;
		current = current->next; // 链表头后移
	}

	// 等待用户按键关闭
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("按R或r重新开始") / 2, WINDOWS_HEIGHT - 100, "按R或r重新开始");
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("按Esc退出游戏") / 2, WINDOWS_HEIGHT - 60, "按Esc退出游戏");
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY)) {
		flushmessage(EX_KEY);
		if (GetAsyncKeyState('R') || GetAsyncKeyState('r')) { // 按r重新开始
			return true;
		} else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // 按esc退出
			exit(0);
		}
	}
	EndBatchDraw(); // 结束批量绘图
	return false;
}

// 释放链表内存
void freeChain() {
	Player *current = ranking_head;
	while (current) {
		Player *temp = current;
		current = current->next;
		free(temp);
	}
	ranking_head = NULL;
}

/*判断游戏结束条件函数*/
// 判断是否胜利
int isGameWin() {
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
int isGameOver() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			// 如果有空格子，游戏未结束
			if (nums[i][j] == 0) {
				return false;
			}
			// 检查当前格子与四个方向是否相等
			if (j + 1 < SIZE && nums[i][j] == nums[i][j + 1]) {
				return false;
			}
			if (i + 1 < SIZE && nums[i][j] == nums[i + 1][j]) {
				return false;
			}
			if (i - 1 > -1 && nums[i][j] == nums[i - 1][j]) {
				return false;
			}
			if (j - 1 > -1 && nums[i][j] == nums[i][j - 1]) {
				return false;
			}
		}
	}
	return true;
}

/*游戏画面绘制相关函数*/
// 初始化游戏窗口
void initWindows() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT); // 初始化窗口
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

	// 分数清零
	score = 0;
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
			switch (nums[i][j]) {
				case 0:
					grid_color = ZERO;
					break;
				case 2:
					grid_color = TWO;
					break;
				case 4:
					grid_color = FOUR;
					break;
				case 8:
					grid_color = EIGHT;
					break;
				case 16:
					grid_color = SIXTE;
					break;
				case 32:
					grid_color = THI_TW;
					break;
				case 64:
					grid_color = SIX_FO;
					break;
				case 128:
					grid_color = OH_TW_EI;
					break;
				case 256:
					grid_color = TH_FI_SI;
					break;
				case 512:
					grid_color = FH_TWEL;
					break;
				case 1024:
					grid_color = OT_TW_FO;
					break;
				case 2048:
					grid_color = TT_FO_EI;
					break;
				default:
					break;
			}
			setfillcolor(grid_color); //设置填充颜色

			// 填充无边框矩形，注意二维坐标从左上角开始，函数四个参数分别为左上角xy与右下角xy
			solidrectangle(grid_pos[i][j].x, grid_pos[i][j].y, grid_pos[i][j].x + GRID_WIDTH, grid_pos[i][j].y + GRID_WIDTH);
			// 设置每一格的文字
			if (nums[i][j]) {
				char number[5]; // 储存数字字符串
				settextcolor(RGB(255, 255, 255));
				if (nums[i][j] == 2 || nums[i][j] == 4)
					settextcolor(RGB(119, 110, 101));  // 给2和4换一个深色
				settextstyle(40, 0, "Times New Roman"); // 设置字号与字体
				setbkmode(TRANSPARENT); // 设置文字背景色为透明
				sprintf(number, "%d", nums[i][j]); // 将数组中的整数变为字符串存储
				int text_width = GRID_WIDTH / 2 - textwidth(number) / 2; // 计算字符宽度
				int text_height = GRID_WIDTH / 2 - textheight(number) / 2; // 计算字符高度
				outtextxy(grid_pos[i][j].x + text_width, grid_pos[i][j].y + text_height, number); // 居中显示数字
			}

		}
	}

	// 设置最高分数显示
	setfillcolor(REC);
	solidrectangle(WINDOWS_WIDTH / 2 + 50, WINDOWS_HEIGHT - 200, WINDOWS_WIDTH / 2 + 200, WINDOWS_HEIGHT - 120);
	settextcolor(RGB(0, 0, 0));
	settextstyle(20, 0, "黑体");
	outtextxy(WINDOWS_WIDTH / 2 + 50 + (150 - textwidth("HighestScores")) / 2,
	          WINDOWS_HEIGHT - 200 + textheight("HighestScores") / 2, "HighestScores");
	char score_h[100];
	if (getScore(cur_name) > score)
		sprintf(score_h, "%d", getScore(cur_name));
	else
		sprintf(score_h, "%d", score);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, "黑体");
	outtextxy(WINDOWS_WIDTH / 2 + 50 + (150 - textwidth(score_h)) / 2, WINDOWS_HEIGHT - 170 + textheight(score_h) / 2,
	          score_h);

	// 设置当前分数显示
	char score_c[100];
	sprintf(score_c, "%d", score);
	solidrectangle(WINDOWS_WIDTH / 2 - 200, WINDOWS_HEIGHT - 200, WINDOWS_WIDTH / 2 - 50, WINDOWS_HEIGHT - 120);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, "黑体");
	outtextxy(WINDOWS_WIDTH / 2 - 200 + (150 - textwidth("Scores")) / 2, WINDOWS_HEIGHT - 210 + textheight("Scores") / 2,
	          "Scores");
	outtextxy(WINDOWS_WIDTH / 2 - 200 + (150 - textwidth(score_c)) / 2, WINDOWS_HEIGHT - 170 + textheight(score_c) / 2,
	          score_c);

	// 其他提示文字
	const char TIPE[30] = "按Esc保存并退出游戏";
	outtextxy((WINDOWS_WIDTH - textwidth(TIPE)) / 2, WINDOWS_HEIGHT - 120 + textheight(TIPE) / 2, TIPE);
	EndBatchDraw(); // 结束批量绘图
}

/*控制移动相关函数*/
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
				} else if (nums[temp][j] == nums[i][j]) {
					score += nums[temp][j] * 2;
					nums[temp][j] *= 2;
					nums[i][j] = 0;
					temp++;
					flag = true;
				} else {
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
				} else if (nums[temp][j] == nums[i][j]) {
					score += nums[temp][j] * 2;
					nums[temp][j] *= 2;
					nums[i][j] = 0;
					temp--;
					flag = true;
				} else {
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
				} else if (nums[i][temp] == nums[i][j]) {
					score += nums[i][temp] * 2;
					nums[i][temp] *= 2;
					nums[i][j] = 0;
					temp++;
					flag = true;
				} else {
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
				} else if (nums[i][temp] == nums[i][j]) {
					score += nums[i][temp] * 2;
					nums[i][temp] *= 2;
					nums[i][j] = 0;
					temp--;
					flag = true;
				} else {
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
}

// 控制数字移动
void stdControl() {
	ExMessage msg;
	int processed = false; // 标记是否已经移动过
	while (peekmessage(&msg, EX_KEY)) {
		// 从窗口中读取按键信息(wasd或WASD或方向键或退出)
		if (!processed && GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('w') || GetAsyncKeyState('W')) {
			up();
			processed = true;
		} else if (!processed && GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('a') || GetAsyncKeyState('A')) {
			left();
			processed = true;
		} else if (!processed && GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('s') || GetAsyncKeyState('S')) {
			down();
			processed = true;
		} else if (!processed && GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('d') || GetAsyncKeyState('D')) {
			right();
			processed = true;
		} else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			flaghistory = true;
			saveGameState();
			exit(0);
		}
	}
	flushmessage(EX_KEY);
}

/*结束画面相关函数*/
// 游戏结果展示
void showResult(const char *message) {
	// 创建矩形窗口
	setfillcolor(RGB(0, 0, 0)); //设置填充颜色
	int x1 = (WINDOWS_WIDTH - RECWIDTH) / 2;
	int y1 = (WINDOWS_HEIGHT - RECHEIGHT) / 2 - 50;
	int x2 = x1 + RECWIDTH;
	int y2 = y1 + RECHEIGHT - 50;
	solidrectangle(x1 - 5, y1 - 5, x2 + 5, y2 + 5); // 填充显示结果矩形
	setfillcolor(BK);
	solidrectangle(x1, y1, x2, y2);

	// 输出结束信息
	settextcolor(RGB(255, 0, 0)); // 设置颜色
	settextstyle(50, 0, "Times New Roman"); // 设置字号与字体
	setbkmode(TRANSPARENT); // 设置文字背景色为透明
	outtextxy(x1 + (RECWIDTH - textwidth(message)) / 2, y1 - 100 + RECHEIGHT / 2 - textheight(message), message);
	settextcolor(RGB(0, 0, 0)); // 设置颜色
	settextstyle(25, 0, "Times New Roman"); // 设置字号与字体
	const char TIP1[100] = "按R或r重新开始";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP1)) / 2, y1 - 50 + RECHEIGHT / 2 - textheight(TIP1), TIP1);
	const char TIP2[100] = "按Esc退出";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP2)) / 2, y1 - 20 + RECHEIGHT / 2 - textheight(TIP2), TIP2);
	const char TIP3[100] = "按L或l显示排名";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP3)) / 2, y1 + 10 + RECHEIGHT / 2 - textheight(TIP3), TIP3);

	// 修改并输出排名
	insertRanking(cur_name, score); // 更新排名
	saveRankings(); // 保存到文件

	// 输出分数
	settextstyle(30, 0, "Times New Roman"); // 设置字号与字体
	char score1[100];
	sprintf(score1, "Scores: %d", score);
	outtextxy(x1 + (RECWIDTH - textwidth(score1)) / 2, y1 + 50  + RECHEIGHT / 2 - textheight(score1), score1);

	char score_c[20];
	sprintf(score_c, "Highest Scores:%d", getScore(cur_name));
	//sprintf(score2, "Highest Scores: %d", historyScore());
	outtextxy(x1 + (RECWIDTH - textwidth(score_c)) / 2, y1 + 80 + RECHEIGHT / 2 - textheight(score_c), score_c);
}

// 游戏结束后的操作判断
int overInput() {
	flaghistory = false;
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY)) {
		if (GetAsyncKeyState('R') || GetAsyncKeyState('r')) {
			closegraph();
			initWindows();
			return true;
		} else if (GetAsyncKeyState('L') || GetAsyncKeyState('l')) {
			while (!displayRankings());
			closegraph();
			initWindows();
			return true;
		} else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			closegraph();
			exit(0);
		}
	}
	flushmessage(EX_KEY);
	return false;
}

int main() {
	int is_load = 0; // 判断有无历史数据
	initWindows(); // 初始化窗口
	is_load = loadSaveData(); // 根据有无历史数据判断是否需要加载
	loadRankings(); // 加载排名
	// 不需要加载时输入姓名
	if (!is_load) {
		printf("请输入您的昵称：");
		scanf("%s", cur_name);
	}
	// 游戏操作
	while (1) {
		drawBoard();
		if (isGameWin()) { // 游戏胜利
			showResult("You Win!");
			while (!overInput()); // 等待用户操作
		} else if (isGameOver()) { // 游戏结束
			showResult("Game Over");
			while (!overInput()); // 等待用户操作
		} else {
			stdControl(); // 控制
			numSummon(); // 生成数字
			saveGameState(); // 保存记录
		}
	}
	freeChain(); // 释放链表内存
	return 0;
}