#include <stdio.h>
#include <graphics.h> // EasyXͼ�ο�ͷ�ļ�
#include <time.h> // ���ڳ�ʼ�������������
#include <conio.h> // ���ڼ��̲���
#include <string.h> // �����ַ��������

/*�궨��ȫ�ֳ���*/
// �������
#define true 1
#define false 0
// �궨�崰�ڳߴ�����
#define GRID_WIDTH 100 // ���ӿ��
#define INTERVALS 15 // �����С
#define SIZE 4 // ��������
#define WINDOWS_WIDTH (GRID_WIDTH*SIZE+INTERVALS*(SIZE+1)) // ���ڿ�
#define WINDOWS_HEIGHT (GRID_WIDTH*SIZE+INTERVALS*(SIZE+1))+200 //���ڸ�
// �궨������ʾ��ߴ�
#define RECWIDTH 400
#define RECHEIGHT 400
// �궨��������ֳ���
#define NAME 50

// ��ö���������ø��������Լ�������ɫ
enum color {
	REC = RGB(187, 173, 160), // ����ɫ
	BK = RGB(255, 224, 172), // ����ɫ
	ZERO = RGB(205, 193, 180), // û������
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

/*��������ȫ�ֱ���*/

// ʹ�������¼�����Ϣ
typedef struct Player {
	char name[NAME];
	int score;
	struct Player *next;
} Player;
Player *ranking_head = NULL; // ��ʼ������ͷ

// ��¼����
struct pos {
	long long x;
	long long y;
} grid_pos[SIZE][SIZE];
// �����ļ�ָ��
FILE *score_f;
FILE *history;
FILE *gamestate;
// ��¼��������
int nums[SIZE][SIZE];
// ��ʱ��¼����
int score;
// ��ʱ��¼�����
char cur_name[NAME];
// �ж��Ƿ������������
int flag = false;
int flaghistory = false;

/*��������������õ��ĺ���*/
// �����������
int randomNums() {
	time_t t1; // ��ʱ����Ϊsrand��seed����ֹÿ���������һ��
	srand((unsigned)time(&t1)); // ��ʼ�������������
	if (rand() % 8 == 1)
		return 4; // 20%�ĸ��ʲ���4
	else
		return 2; // 80%�ĸ��ʲ���2
}

// �Ѳ��������ַŵ����λ��
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

// �ж��Ƿ���Բ������֣����������ƶ����������
void numSummon() {
	if (flag) {
		randomPos();
		flag = false;
	}
}

/*��Ϸ��¼�洢���ȡ��غ���*/
// �洢��Ϸ��¼
void saveGameState() {
	gamestate = fopen("game_state.txt", "w");
	fprintf(gamestate, "%d\n", flaghistory);// ������ʷ��¼״̬
	fprintf(gamestate, "%s\n", cur_name);// �����������
	fprintf(gamestate, "%d\n", score);// �������

	// ���������ϵ�����
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			fprintf(gamestate, "%d ", nums[i][j]);
		}
		fprintf(gamestate, "\n");
	}
	fclose(gamestate);
}

// ��ȡ��߷���
int getScore(const char *i_name) {
	FILE *file = fopen("ranking.txt", "r");
	if (!file) {
		// ����ļ������ڣ�����Ĭ��ֵ
		return 0;
	}
	char name[NAME];
	int score = 0;
	int highest_score = 0;

	// �����ļ�����
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

// ������ʷ����
int loadSaveData() {
	int state = 0;
	gamestate = fopen("game_state.txt", "r");
	if (!gamestate) {
		// �ļ������ڣ���ʼ��״̬Ϊδ����
		state = 0;
		return state;
	}
	fscanf(gamestate, "%d\n", &state); // ��ȡ��һ�е���ʷ��¼��־
	fclose(gamestate);
	// �������ʷ��¼
	if (state == 1) {
		// ��������ѯ���û��Ƿ�Ҫ������ʷ��¼
		setfillcolor(RGB(0, 0, 0)); // ���������ɫ
		int x1 = (WINDOWS_WIDTH - RECWIDTH) / 2;
		int y1 = (WINDOWS_HEIGHT - RECHEIGHT) / 2;
		int x2 = x1 + RECWIDTH;
		int y2 = y1 + RECHEIGHT - 100;
		solidrectangle(x1 - 5, y1 - 5, x2 + 5, y2 + 5); // �����ʾ�������
		setfillcolor(BK);
		solidrectangle(x1, y1, x2, y2);

		// ��ʾ��Ϣ
		settextcolor(RGB(255, 0, 0)); // ������ɫ
		settextstyle(35, 0, "����");
		setbkmode(TRANSPARENT); // �������ֱ���ɫΪ͸��
		const char *message = "�Ƿ���ϴεĽ��ȿ�ʼ��";
		outtextxy(x1 + (RECWIDTH - textwidth(message)) / 2, y1 - 100 + RECHEIGHT / 2 - textheight(message), message);

		// ��ʾ����
		settextcolor(RGB(0, 0, 0)); // ������ɫ
		settextstyle(25, 0, "����");
		const char *TIP1 = "��\"Y\"��\"y\"��ȡ����";
		const char *TIP2 = "��\"N\"��\"n\"���¿�ʼ";
		outtextxy(x1 + (RECWIDTH - textwidth(TIP1)) / 2, y1 + RECHEIGHT / 2 - textheight(TIP1), TIP1);
		outtextxy(x1 + (RECWIDTH - textwidth(TIP2)) / 2, y1 + 50 + RECHEIGHT / 2 - textheight(TIP2), TIP2);

		// �ȴ��û�����
		ExMessage msg;
		while (1) {
			if (peekmessage(&msg, EX_KEY)) {
				// ������ʷ��¼
				if (GetAsyncKeyState('Y') || GetAsyncKeyState('y')) {
					gamestate = fopen("game_state.txt", "r");
					int state = 0;
					fscanf(gamestate, "%d\n", &state); // ��ȡ״̬
					fscanf(gamestate, "%s\n", cur_name); // ��ȡ�ǳ�
					fscanf(gamestate, "%d\n", &score); // ��ȡ����

					// ��ȡ��������
					for (int i = 0; i < SIZE; i++) {
						for (int j = 0; j < SIZE; j++) {
							fscanf(gamestate, "%d ", &nums[i][j]);
						}
					}
					fclose(gamestate);
					flaghistory = state; // �ָ���ʷ״̬
					break;
				}
				// �����أ���ʼ����Ϸ
				else if (GetAsyncKeyState('N') || GetAsyncKeyState('n')) {
					state = 0;
					break;
				}
			}
			flushmessage(EX_KEY);
		}
	} else {
		return state; // ����Ҫ���ؽ��� ֱ�����¿�ʼ
	}
	return state;
}

/*������غ���*/
// ��������
void loadRankings() {
	FILE *file = fopen("ranking.txt", "r");
	if (!file) return; // �������ļ��򷵻�
	char name[NAME]; // ��ʱ��¼�ǳ�
	int l_score = 0; // ��ʱ��¼����

	while (fscanf(file, "%s %d", name, &l_score) == 2) { // ���سɹ�ƥ���븳ֵ�ĸ���
		Player *new_node = (Player *)malloc(sizeof(Player)); // ����һ���������ڶ�ȡ�ļ��е����ݲ����ٿռ�
		strcpy(new_node->name, name); // ����ȡ�������ִ�������
		new_node->score = l_score; // ����ȡ���ķ���������������
		new_node->next = ranking_head; // ����������ͷ�Ƶ���һλ
		ranking_head = new_node; // ��ǰλ����������
	}
	fclose(file);
}

// ��������
void saveRankings() {
	FILE *file = fopen("ranking.txt", "w");
	if (!file)
		return; // �������ļ��򷵻�
	Player *current = ranking_head; // ������ʱ����ָ��ǰ��������ͷ

	// �����������е�����д���ļ���ͬʱ�ƶ���ʱ����
	while (current) {
		fprintf(file, "%s %d\n", current->name, current->score);
		current = current->next;
	}
	fclose(file);
}

// ð���������������
void listSort(Player *head) {
	if (head == NULL || head->next == NULL) {
		return; // ����Ϊ�ջ�ֻ��һ���ڵ㣬��������
	}

	Player *tail = NULL; // ÿ��ð������ı߽�
	Player *cur;

	while (head != tail) {
		cur = head;
		while (cur->next != tail) {
			if (cur->score < cur->next->score) { // �Ӹߵ�������
				// ��������
				int tempScore = cur->score;
				cur->score = cur->next->score;
				cur->next->score = tempScore;

				// ��������
				char tempName[50];
				strcpy(tempName, cur->name);
				strcpy(cur->name, cur->next->name);
				strcpy(cur->next->name, tempName);
			}
			cur = cur->next;
		}
		tail = cur; // ����β���߽�
	}
}

// ����(�µ�)�����(����)��������
void insertRanking(const char *i_name, int i_score) {
	Player *current = ranking_head;
	Player *prev = NULL;

	// ����Ƿ��Ѿ�����ͬ�����
	while (current != NULL) {
		if (strcmp(current->name, i_name) == 0) {
			// �����ǰ�������ߣ����·���
			if (current->score < i_score) {
				current->score = i_score;
			}
			// ��������
			listSort(ranking_head);
			return;
		}
		prev = current;
		current = current->next;
	}

	// ���û���ҵ�ͬ����ң������½ڵ�
	Player *new_node = (Player *)malloc(sizeof(Player));
	// �����������½ڵ�
	strncpy(new_node->name, i_name, sizeof(new_node->name) - 1);
	new_node->name[sizeof(new_node->name) - 1] = '\0';
	new_node->score = i_score;
	new_node->next = NULL;

	// ���뵽����β��
	if (!ranking_head) {
		ranking_head = new_node; // �������Ϊ�������ͷ��
	} else {
		prev->next = new_node; // ����ǰ������һλ
	}
	// ��������
	listSort(ranking_head);
}

// ��ʾ����
int displayRankings() {
	// ˫����
	BeginBatchDraw(); // ��ʼ������ͼ
	cleardevice(); // ����

	// ��ʾ����
	settextcolor(BLACK);
	settextstyle(40, 0, "����");
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("����") / 2, 20, "����");

	// ����������ʾǰ10�����
	settextstyle(20, 0, "����");
	Player *current = ranking_head; // �ṹ��ָ������ͷ
	int rank_h = 80; // ����������ʾλ��
	int rank = 1; // ��ʾ����
	while (current != NULL && rank <= 10) {
		char display_text[100];
		sprintf(display_text, "No%d. %s ------- %d��", rank, current->name, current->score); // �������д洢���������
		outtextxy(50, rank_h, display_text);
		rank_h += 30; // ÿ��+30������ʾ��һ��
		rank++;
		current = current->next; // ����ͷ����
	}

	// �ȴ��û������ر�
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("��R��r���¿�ʼ") / 2, WINDOWS_HEIGHT - 100, "��R��r���¿�ʼ");
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("��Esc�˳���Ϸ") / 2, WINDOWS_HEIGHT - 60, "��Esc�˳���Ϸ");
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY)) {
		flushmessage(EX_KEY);
		if (GetAsyncKeyState('R') || GetAsyncKeyState('r')) { // ��r���¿�ʼ
			return true;
		} else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // ��esc�˳�
			exit(0);
		}
	}
	EndBatchDraw(); // ����������ͼ
	return false;
}

// �ͷ������ڴ�
void freeChain() {
	Player *current = ranking_head;
	while (current) {
		Player *temp = current;
		current = current->next;
		free(temp);
	}
	ranking_head = NULL;
}

/*�ж���Ϸ������������*/
// �ж��Ƿ�ʤ��
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

// �ж��Ƿ����
int isGameOver() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			// ����пո��ӣ���Ϸδ����
			if (nums[i][j] == 0) {
				return false;
			}
			// ��鵱ǰ�������ĸ������Ƿ����
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

/*��Ϸ���������غ���*/
// ��ʼ����Ϸ����
void initWindows() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT); // ��ʼ������
	setbkcolor(BK); // ���ô��ڱ���ɫ
	cleardevice(); // �ñ���ɫ�������

	// Ԥ����ÿ�����ӵ�����
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			grid_pos[i][j].x = i * GRID_WIDTH + (i + 1) * INTERVALS;
			grid_pos[i][j].y = j * GRID_WIDTH + (j + 1) * INTERVALS;
		}
	}

	// ���������������(2��4)
	randomPos();
	randomPos();
	nums[2][2] = 1024, nums[2][3] = 1024;

	// ��������
	score = 0;
}

// ������Ϸ����
void drawBoard() {
	// ˫���崦����
	BeginBatchDraw(); // ��ʼ������ͼ
	cleardevice(); // ����

	// ��ά���������������ɫ,��ʼ��Ϊû�����ֵ���ɫ
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			// ����������������ɫ
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
			setfillcolor(grid_color); //���������ɫ

			// ����ޱ߿���Σ�ע���ά��������Ͻǿ�ʼ�������ĸ������ֱ�Ϊ���Ͻ�xy�����½�xy
			solidrectangle(grid_pos[i][j].x, grid_pos[i][j].y, grid_pos[i][j].x + GRID_WIDTH, grid_pos[i][j].y + GRID_WIDTH);
			// ����ÿһ�������
			if (nums[i][j]) {
				char number[5]; // ���������ַ���
				settextcolor(RGB(255, 255, 255));
				if (nums[i][j] == 2 || nums[i][j] == 4)
					settextcolor(RGB(119, 110, 101));  // ��2��4��һ����ɫ
				settextstyle(40, 0, "Times New Roman"); // �����ֺ�������
				setbkmode(TRANSPARENT); // �������ֱ���ɫΪ͸��
				sprintf(number, "%d", nums[i][j]); // �������е�������Ϊ�ַ����洢
				int text_width = GRID_WIDTH / 2 - textwidth(number) / 2; // �����ַ����
				int text_height = GRID_WIDTH / 2 - textheight(number) / 2; // �����ַ��߶�
				outtextxy(grid_pos[i][j].x + text_width, grid_pos[i][j].y + text_height, number); // ������ʾ����
			}

		}
	}

	// ������߷�����ʾ
	setfillcolor(REC);
	solidrectangle(WINDOWS_WIDTH / 2 + 50, WINDOWS_HEIGHT - 200, WINDOWS_WIDTH / 2 + 200, WINDOWS_HEIGHT - 120);
	settextcolor(RGB(0, 0, 0));
	settextstyle(20, 0, "����");
	outtextxy(WINDOWS_WIDTH / 2 + 50 + (150 - textwidth("HighestScores")) / 2,
	          WINDOWS_HEIGHT - 200 + textheight("HighestScores") / 2, "HighestScores");
	char score_h[100];
	if (getScore(cur_name) > score)
		sprintf(score_h, "%d", getScore(cur_name));
	else
		sprintf(score_h, "%d", score);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, "����");
	outtextxy(WINDOWS_WIDTH / 2 + 50 + (150 - textwidth(score_h)) / 2, WINDOWS_HEIGHT - 170 + textheight(score_h) / 2,
	          score_h);

	// ���õ�ǰ������ʾ
	char score_c[100];
	sprintf(score_c, "%d", score);
	solidrectangle(WINDOWS_WIDTH / 2 - 200, WINDOWS_HEIGHT - 200, WINDOWS_WIDTH / 2 - 50, WINDOWS_HEIGHT - 120);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, "����");
	outtextxy(WINDOWS_WIDTH / 2 - 200 + (150 - textwidth("Scores")) / 2, WINDOWS_HEIGHT - 210 + textheight("Scores") / 2,
	          "Scores");
	outtextxy(WINDOWS_WIDTH / 2 - 200 + (150 - textwidth(score_c)) / 2, WINDOWS_HEIGHT - 170 + textheight(score_c) / 2,
	          score_c);

	// ������ʾ����
	const char TIPE[30] = "��Esc���沢�˳���Ϸ";
	outtextxy((WINDOWS_WIDTH - textwidth(TIPE)) / 2, WINDOWS_HEIGHT - 120 + textheight(TIPE) / 2, TIPE);
	EndBatchDraw(); // ����������ͼ
}

/*�����ƶ���غ���*/
// ����
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

// ����
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

// ����
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

// ����
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

// ���������ƶ�
void stdControl() {
	ExMessage msg;
	int processed = false; // ����Ƿ��Ѿ��ƶ���
	while (peekmessage(&msg, EX_KEY)) {
		// �Ӵ����ж�ȡ������Ϣ(wasd��WASD��������˳�)
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

/*����������غ���*/
// ��Ϸ���չʾ
void showResult(const char *message) {
	// �������δ���
	setfillcolor(RGB(0, 0, 0)); //���������ɫ
	int x1 = (WINDOWS_WIDTH - RECWIDTH) / 2;
	int y1 = (WINDOWS_HEIGHT - RECHEIGHT) / 2 - 50;
	int x2 = x1 + RECWIDTH;
	int y2 = y1 + RECHEIGHT - 50;
	solidrectangle(x1 - 5, y1 - 5, x2 + 5, y2 + 5); // �����ʾ�������
	setfillcolor(BK);
	solidrectangle(x1, y1, x2, y2);

	// ���������Ϣ
	settextcolor(RGB(255, 0, 0)); // ������ɫ
	settextstyle(50, 0, "Times New Roman"); // �����ֺ�������
	setbkmode(TRANSPARENT); // �������ֱ���ɫΪ͸��
	outtextxy(x1 + (RECWIDTH - textwidth(message)) / 2, y1 - 100 + RECHEIGHT / 2 - textheight(message), message);
	settextcolor(RGB(0, 0, 0)); // ������ɫ
	settextstyle(25, 0, "Times New Roman"); // �����ֺ�������
	const char TIP1[100] = "��R��r���¿�ʼ";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP1)) / 2, y1 - 50 + RECHEIGHT / 2 - textheight(TIP1), TIP1);
	const char TIP2[100] = "��Esc�˳�";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP2)) / 2, y1 - 20 + RECHEIGHT / 2 - textheight(TIP2), TIP2);
	const char TIP3[100] = "��L��l��ʾ����";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP3)) / 2, y1 + 10 + RECHEIGHT / 2 - textheight(TIP3), TIP3);

	// �޸Ĳ��������
	insertRanking(cur_name, score); // ��������
	saveRankings(); // ���浽�ļ�

	// �������
	settextstyle(30, 0, "Times New Roman"); // �����ֺ�������
	char score1[100];
	sprintf(score1, "Scores: %d", score);
	outtextxy(x1 + (RECWIDTH - textwidth(score1)) / 2, y1 + 50  + RECHEIGHT / 2 - textheight(score1), score1);

	char score_c[20];
	sprintf(score_c, "Highest Scores:%d", getScore(cur_name));
	//sprintf(score2, "Highest Scores: %d", historyScore());
	outtextxy(x1 + (RECWIDTH - textwidth(score_c)) / 2, y1 + 80 + RECHEIGHT / 2 - textheight(score_c), score_c);
}

// ��Ϸ������Ĳ����ж�
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
	int is_load = 0; // �ж�������ʷ����
	initWindows(); // ��ʼ������
	is_load = loadSaveData(); // ����������ʷ�����ж��Ƿ���Ҫ����
	loadRankings(); // ��������
	// ����Ҫ����ʱ��������
	if (!is_load) {
		printf("�����������ǳƣ�");
		scanf("%s", cur_name);
	}
	// ��Ϸ����
	while (1) {
		drawBoard();
		if (isGameWin()) { // ��Ϸʤ��
			showResult("You Win!");
			while (!overInput()); // �ȴ��û�����
		} else if (isGameOver()) { // ��Ϸ����
			showResult("Game Over");
			while (!overInput()); // �ȴ��û�����
		} else {
			stdControl(); // ����
			numSummon(); // ��������
			saveGameState(); // �����¼
		}
	}
	freeChain(); // �ͷ������ڴ�
	return 0;
}