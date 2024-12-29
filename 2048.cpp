#include <stdio.h>
#include <graphics.h> // EasyX图形库头文件
#include <time.h> // 用于初始化随机数发生器
#include <conio.h> // 用于键盘操作
#include <string.h> // 用于与文件数据比较

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
	BK = RGB(255,224,172), // 背景色
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
// 使用链表记录玩家信息
typedef struct Player {
	char name[NAME];
	int score;
	struct Player* next;
} Player;
Player* ranking_head = NULL; // 初始化链表头
// 记录坐标
struct pos {
	long long x;
	long long y;
}grid_pos[SIZE][SIZE];
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

// 随机产生数字
int randomNums() {
	time_t t1; // 用时间作为srand的seed，防止每次随机数都一样
	srand((unsigned)time(&t1)); // 初始化随机数发生器
	if (rand() % 8 == 1) return 4; // 20%的概率产生4
	else return 2; // 80%的概率产生2
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

// 获取历史分数
int historyScore() {
	int h_score=0;
	score_f = fopen("score.txt", "r");
	if (score_f) {
		if (fscanf(score_f, "%d", &h_score) != 1) {
			h_score = 0; // 如果读取失败，默认分数为0
		}
		else fscanf(score_f, "%d", &h_score); // 读取分数
		fclose(score_f);
	}
	else {
		score_f = fopen("score.txt", "w");
		fprintf(score_f, "0"); // 初始化文件
		fclose(score_f);
	}
	return h_score;

}
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

// 存储游戏记录
void saveGameState() {
	gamestate = fopen("game_state.txt", "w");
	fprintf(gamestate, "%d\n", flaghistory);// 保存历史记录状态
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

// 加载排名
void loadRankings() {
	FILE* file = fopen("ranking.txt", "r");
	if (!file) return;

	char name[NAME];
	int l_score = 0;
	while (fscanf(file, "%s %d", name, &l_score) == 2) {
		Player* new_node = (Player*)malloc(sizeof(Player));
		strcpy(new_node->name, name);
		new_node->score = l_score;
		new_node->next = ranking_head;
		ranking_head = new_node;
	}
	fclose(file);
}

// 保存排名
void saveRankings() {
	FILE* file = fopen("ranking.txt", "w");
	if (!file) return;

	Player* current = ranking_head;
	while (current) {
		fprintf(file, "%s %d\n", current->name, current->score);
		current = current->next;
	}
	fclose(file);
}

void insertRanking(const char* i_name, int i_score) {
	Player* current = ranking_head;
	Player* prev = NULL;
	while (current != NULL) {
		if (strcmp(current->name, i_name) == 0) {
			// 如果找到相同名字的玩家，则更新成绩
			if (current->score < i_score) {
				current->score = i_score;
			}
			return;
		}
		prev = current;
		current = current->next;
	}

	// 如果没有找到相同名字的玩家，则插入新玩家
	Player* new_node = (Player*)malloc(sizeof(Player));
	strcpy(new_node->name, i_name);
	new_node->score = i_score;
	new_node->next = NULL;

	// 插入到链表的合适位置
	if (!ranking_head || ranking_head->score < i_score) {
		new_node->next = ranking_head;
		ranking_head = new_node;
		return;
	}

	current = ranking_head;
	while (current->next && current->next->score >= i_score) {
		current = current->next;
	}
	new_node->next = current->next;
	current->next = new_node;
}

// 显示排名
int displayRankings() {
	BeginBatchDraw(); // 开始批量绘图
	cleardevice(); // 清屏

	// 显示标题
	settextcolor(BLACK);
	settextstyle(40, 0, "黑体");
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("排名") / 2, 20, "排名");

	// 遍历链表显示前 MAX_DISPLAY 名玩家
	settextstyle(20, 0, "黑体");
	Player* current = ranking_head;
	int rank_h = 80; 
	int rank = 1;
	while (current != NULL && rank <= 10) {
		char display_text[100];
		sprintf(display_text, "No%d. %s ------- %d分", rank, current->name, current->score);
		outtextxy(50, rank_h, display_text);
		rank_h += 30;
		rank++;
		current = current->next;
	}

	// 等待用户按键关闭
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("Press \"r\" or \"R\" to restart...") / 2, WINDOWS_HEIGHT-100, "Press \"r\" or \"R\" to restart...");
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("Press \"Esc\" to quit...") / 2, WINDOWS_HEIGHT - 60, "Press \"Esc\" to quit...");


	ExMessage msg;
	while (peekmessage(&msg, EX_KEY)) {
		flushmessage(EX_KEY);
		if (GetAsyncKeyState('R') || GetAsyncKeyState('r')) {
			return true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 ) {
			exit(0);
		}
	}
	//flushmessage(EX_KEY);
	EndBatchDraw(); // 结束批量绘图
	return false;
	
	

}

// 释放链表内存
void freeChain() {
	Player* current = ranking_head;
	while (current) {
		Player* temp = current;
		current = current->next;
		free(temp);
	}
	ranking_head = NULL;
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

	// 分数清零
	score = 0; 
}
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
		// 弹出 GUI 询问用户是否要加载历史记录
		setfillcolor(RGB(0, 0, 0)); // 设置填充颜色
		int x1 = (WINDOWS_WIDTH - RECWIDTH) / 2;
		int y1 = (WINDOWS_HEIGHT - RECHEIGHT) / 2 ;
		int x2 = x1 + RECWIDTH;
		int y2 = y1 + RECHEIGHT - 100;
		solidrectangle(x1 - 5, y1 - 5, x2 + 5, y2 + 5); // 填充显示结果矩形
		setfillcolor(BK);
		solidrectangle(x1, y1, x2, y2);

		// 提示信息
		settextcolor(RGB(255, 0, 0)); // 设置颜色
		settextstyle(35, 0, "黑体");
		setbkmode(TRANSPARENT); // 设置文字背景色为透明
		const char* message = "是否从上次的进度开始？";
		outtextxy(x1 + (RECWIDTH - textwidth(message)) / 2, y1 - 100 + RECHEIGHT / 2 - textheight(message), message);

		// 提示按键
		settextcolor(RGB(0, 0, 0)); // 设置颜色
		settextstyle(25, 0, "黑体");
		const char* TIP1 = "按\"Y\"或\"y\"读取进度";
		const char* TIP2 = "按\"N\"或\"n\"重新开始";
		outtextxy(x1 + (RECWIDTH - textwidth(TIP1)) / 2, y1  + RECHEIGHT / 2 - textheight(TIP1), TIP1);
		outtextxy(x1 + (RECWIDTH - textwidth(TIP2)) / 2, y1 +50 + RECHEIGHT / 2 - textheight(TIP2), TIP2);

		// 等待用户输入
		ExMessage msg;
		while (1) {
			if (peekmessage(&msg, EX_KEY)) {
				// 按 'Y' 键表示加载历史记录
				if (GetAsyncKeyState('Y') || GetAsyncKeyState('y')) {
					gamestate = fopen("game_state.txt", "r");
					int state = 0;
					fscanf(gamestate, "%d\n", &state);

					// 读取分数
					fscanf(gamestate, "%d\n", &score);

					// 读取棋盘数据
					for (int i = 0; i < SIZE; i++) {
						for (int j = 0; j < SIZE; j++) {
							fscanf(gamestate, "%d ", &nums[i][j]);
						}
					}
					fclose(gamestate);

					// 恢复历史状态
					flaghistory = state;
					break;
				}
				// 按 'N' 键表示不加载，开始新游戏
				else if (GetAsyncKeyState('N') || GetAsyncKeyState('n')) {
					state = 0;
					break;
				}
			}
			flushmessage(EX_KEY);
		}
	}
	else {
		return state; // 不需要加载进度 直接重新开始
	}
	return state;
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
				solidrectangle(grid_pos[i][j].x, grid_pos[i][j].y, grid_pos[i][j].x + GRID_WIDTH, grid_pos[i][j].y + GRID_WIDTH);
				// 设置每一格的文字
				if (nums[i][j]) {
					char number[5]; // 储存数字字符串
					settextcolor(RGB(255, 255, 255));
					if (nums[i][j] == 2 || nums[i][j] == 4) settextcolor(RGB(119, 110, 101));  // 给2和4换一个深色
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
	outtextxy(WINDOWS_WIDTH / 2 + 50+(150-textwidth("HighestScores")) / 2, WINDOWS_HEIGHT - 200 + textheight("HighestScores") / 2, "HighestScores");
	char score_h[100];
	if (historyScore() > score) sprintf(score_h, "%d", historyScore());
	else sprintf(score_h, "%d", score);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, "黑体");
	outtextxy(WINDOWS_WIDTH / 2 + 50 + (150 - textwidth(score_h)) / 2, WINDOWS_HEIGHT - 170 + textheight(score_h) / 2, score_h);

	// 设置当前分数显示
	char score_c[100];
	sprintf(score_c, "%d", score);
	solidrectangle(WINDOWS_WIDTH / 2 -200, WINDOWS_HEIGHT - 200, WINDOWS_WIDTH / 2 - 50, WINDOWS_HEIGHT - 120);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, "黑体");
	outtextxy(WINDOWS_WIDTH / 2 - 200 + (150 - textwidth("Scores")) / 2, WINDOWS_HEIGHT - 210 + textheight("Scores") / 2, "Scores");
	outtextxy(WINDOWS_WIDTH / 2 - 200 + (150 - textwidth(score_c)) / 2, WINDOWS_HEIGHT - 170 + textheight(score_c) / 2, score_c);
	
	// 其他提示文字
	const char TIPE[30] = "Press \"Esc\" to quit game.";
	outtextxy((WINDOWS_WIDTH-textwidth(TIPE)) / 2, WINDOWS_HEIGHT -120 + textheight(TIPE) / 2, TIPE);
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
					score += nums[temp][j] * 2;
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
	//printf("left\n");
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
					score += nums[temp][j] * 2;
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
	//printf("right\n");
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
					score += nums[i][temp] * 2;
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
	//printf("up\n");
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
					score += nums[i][temp] * 2;
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
	//printf("down\n");
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
		}
		else if (!processed && GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('a') || GetAsyncKeyState('A')) {
			left();
			processed = true;
		}
		else if (!processed && GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('s') || GetAsyncKeyState('S')) {
			down();
			processed = true;
		}
		else if (!processed && GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('d') || GetAsyncKeyState('D')) {
			right();
			processed = true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			flaghistory = true;
			saveGameState();
			exit(0);
		}
	}
	flushmessage(EX_KEY);
	
	//Sleep(100); // 防止过多的输入误触
}

// 游戏结果展示
void showResult(const char* message) {
	// 创建矩形窗口
	setfillcolor(RGB(0,0,0)); //设置填充颜色
	int x1 = (WINDOWS_WIDTH - RECWIDTH) / 2;
	int y1 = (WINDOWS_HEIGHT - RECHEIGHT) / 2-50;
	int x2 = x1 + RECWIDTH;
	int y2 = y1 + RECHEIGHT-50;
	solidrectangle(x1-5,y1-5,x2+5,y2+5); // 填充显示结果矩形
	setfillcolor(BK);
	solidrectangle(x1, y1, x2, y2);


	// 输出结束信息
	settextcolor(RGB(255, 0, 0)); // 设置颜色
	settextstyle(50, 0, "Times New Roman"); // 设置字号与字体
	setbkmode(TRANSPARENT); // 设置文字背景色为透明
	outtextxy(x1 + (RECWIDTH - textwidth(message))/2, y1 - 100 + RECHEIGHT/2- textheight(message), message);
	settextcolor(RGB(0, 0, 0)); // 设置颜色
	settextstyle(25, 0, "Times New Roman"); // 设置字号与字体
	const char TIP1[100] = "Press \"R\" or \"r\" to restart.";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP1)) / 2, y1 - 50 + RECHEIGHT / 2 - textheight(TIP1), TIP1);
	const char TIP2[100] = "Press \"Esc\" to quit.";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP2)) / 2, y1 - 20 + RECHEIGHT / 2 - textheight(TIP2), TIP2);
	const char TIP3[100] = "Press \"L\" or \"l\" to show the ranking.";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP3)) / 2, y1 +10 + RECHEIGHT / 2 - textheight(TIP3), TIP3);

	// 输出分数
	settextstyle(30, 0, "Times New Roman"); // 设置字号与字体
	char score1[100];
	sprintf(score1, "Scores: %d", score);
	outtextxy(x1 + (RECWIDTH - textwidth(score1)) / 2, y1 + 50  + RECHEIGHT / 2 - textheight(score1), score1);

	char score_c[20];
	sprintf(score_c, "Highest Scores:%d", historyScore());
	//sprintf(score2, "Highest Scores: %d", historyScore());
	outtextxy(x1 + (RECWIDTH - textwidth(score_c)) / 2, y1 + 80 + RECHEIGHT / 2 - textheight(score_c), score_c);

	// 修改并输出排名
	insertRanking(cur_name, score); // 更新排名
	saveRankings(); // 保存到文件
	
	
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
		}
		else if (GetAsyncKeyState('L') || GetAsyncKeyState('l')) {
			while(!displayRankings());
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
	int is_load = 0;
	initWindows();
	is_load = loadSaveData();
	loadRankings();
	if (!is_load) {
		printf("请输入您的姓名：");
		scanf("%s", cur_name);
	}
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
			// 每次都判断一下最高分数
			if (historyScore() < score) {
				score_f = fopen("score.txt", "w");
				fprintf(score_f, "%d", score); // 写入新的最高分
				fclose(score_f);
			}
			saveGameState();
			
		}
	}
	
	freeChain(); // 释放链表内存

	//system("pause"); // 使窗口保持开启
	return 0;
}