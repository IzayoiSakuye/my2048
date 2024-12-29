#include <stdio.h>
#include <graphics.h> // EasyXͼ�ο�ͷ�ļ�
#include <time.h> // ���ڳ�ʼ�������������
#include <conio.h> // ���ڼ��̲���
#include <string.h> // �������ļ����ݱȽ�

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
	BK = RGB(255,224,172), // ����ɫ
	ZERO = RGB(205,193,180), // û������
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
// ʹ�������¼�����Ϣ
typedef struct Player {
	char name[NAME];
	int score;
	struct Player* next;
} Player;
Player* ranking_head = NULL; // ��ʼ������ͷ
// ��¼����
struct pos {
	long long x;
	long long y;
}grid_pos[SIZE][SIZE];
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

// �����������
int randomNums() {
	time_t t1; // ��ʱ����Ϊsrand��seed����ֹÿ���������һ��
	srand((unsigned)time(&t1)); // ��ʼ�������������
	if (rand() % 8 == 1) return 4; // 20%�ĸ��ʲ���4
	else return 2; // 80%�ĸ��ʲ���2
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

// �ж��Ƿ���Բ������֣������Ա��������
void numSummon() {
	if (flag) {
		randomPos();
		flag = false;
	}
}

// ��ȡ��ʷ����
int historyScore() {
	int h_score=0;
	score_f = fopen("score.txt", "r");
	if (score_f) {
		if (fscanf(score_f, "%d", &h_score) != 1) {
			h_score = 0; // �����ȡʧ�ܣ�Ĭ�Ϸ���Ϊ0
		}
		else fscanf(score_f, "%d", &h_score); // ��ȡ����
		fclose(score_f);
	}
	else {
		score_f = fopen("score.txt", "w");
		fprintf(score_f, "0"); // ��ʼ���ļ�
		fclose(score_f);
	}
	return h_score;

}
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

// �洢��Ϸ��¼
void saveGameState() {
	gamestate = fopen("game_state.txt", "w");
	fprintf(gamestate, "%d\n", flaghistory);// ������ʷ��¼״̬
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

// ��������
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

// ��������
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
			// ����ҵ���ͬ���ֵ���ң�����³ɼ�
			if (current->score < i_score) {
				current->score = i_score;
			}
			return;
		}
		prev = current;
		current = current->next;
	}

	// ���û���ҵ���ͬ���ֵ���ң�����������
	Player* new_node = (Player*)malloc(sizeof(Player));
	strcpy(new_node->name, i_name);
	new_node->score = i_score;
	new_node->next = NULL;

	// ���뵽����ĺ���λ��
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

// ��ʾ����
int displayRankings() {
	BeginBatchDraw(); // ��ʼ������ͼ
	cleardevice(); // ����

	// ��ʾ����
	settextcolor(BLACK);
	settextstyle(40, 0, "����");
	outtextxy(WINDOWS_WIDTH / 2 - textwidth("����") / 2, 20, "����");

	// ����������ʾǰ MAX_DISPLAY �����
	settextstyle(20, 0, "����");
	Player* current = ranking_head;
	int rank_h = 80; 
	int rank = 1;
	while (current != NULL && rank <= 10) {
		char display_text[100];
		sprintf(display_text, "No%d. %s ------- %d��", rank, current->name, current->score);
		outtextxy(50, rank_h, display_text);
		rank_h += 30;
		rank++;
		current = current->next;
	}

	// �ȴ��û������ر�
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
	EndBatchDraw(); // ����������ͼ
	return false;
	
	

}

// �ͷ������ڴ�
void freeChain() {
	Player* current = ranking_head;
	while (current) {
		Player* temp = current;
		current = current->next;
		free(temp);
	}
	ranking_head = NULL;
}

// ��ʼ����Ϸ����
void initWindows() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT, EX_SHOWCONSOLE); // ��ʼ������
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
		// ���� GUI ѯ���û��Ƿ�Ҫ������ʷ��¼
		setfillcolor(RGB(0, 0, 0)); // ���������ɫ
		int x1 = (WINDOWS_WIDTH - RECWIDTH) / 2;
		int y1 = (WINDOWS_HEIGHT - RECHEIGHT) / 2 ;
		int x2 = x1 + RECWIDTH;
		int y2 = y1 + RECHEIGHT - 100;
		solidrectangle(x1 - 5, y1 - 5, x2 + 5, y2 + 5); // �����ʾ�������
		setfillcolor(BK);
		solidrectangle(x1, y1, x2, y2);

		// ��ʾ��Ϣ
		settextcolor(RGB(255, 0, 0)); // ������ɫ
		settextstyle(35, 0, "����");
		setbkmode(TRANSPARENT); // �������ֱ���ɫΪ͸��
		const char* message = "�Ƿ���ϴεĽ��ȿ�ʼ��";
		outtextxy(x1 + (RECWIDTH - textwidth(message)) / 2, y1 - 100 + RECHEIGHT / 2 - textheight(message), message);

		// ��ʾ����
		settextcolor(RGB(0, 0, 0)); // ������ɫ
		settextstyle(25, 0, "����");
		const char* TIP1 = "��\"Y\"��\"y\"��ȡ����";
		const char* TIP2 = "��\"N\"��\"n\"���¿�ʼ";
		outtextxy(x1 + (RECWIDTH - textwidth(TIP1)) / 2, y1  + RECHEIGHT / 2 - textheight(TIP1), TIP1);
		outtextxy(x1 + (RECWIDTH - textwidth(TIP2)) / 2, y1 +50 + RECHEIGHT / 2 - textheight(TIP2), TIP2);

		// �ȴ��û�����
		ExMessage msg;
		while (1) {
			if (peekmessage(&msg, EX_KEY)) {
				// �� 'Y' ����ʾ������ʷ��¼
				if (GetAsyncKeyState('Y') || GetAsyncKeyState('y')) {
					gamestate = fopen("game_state.txt", "r");
					int state = 0;
					fscanf(gamestate, "%d\n", &state);

					// ��ȡ����
					fscanf(gamestate, "%d\n", &score);

					// ��ȡ��������
					for (int i = 0; i < SIZE; i++) {
						for (int j = 0; j < SIZE; j++) {
							fscanf(gamestate, "%d ", &nums[i][j]);
						}
					}
					fclose(gamestate);

					// �ָ���ʷ״̬
					flaghistory = state;
					break;
				}
				// �� 'N' ����ʾ�����أ���ʼ����Ϸ
				else if (GetAsyncKeyState('N') || GetAsyncKeyState('n')) {
					state = 0;
					break;
				}
			}
			flushmessage(EX_KEY);
		}
	}
	else {
		return state; // ����Ҫ���ؽ��� ֱ�����¿�ʼ
	}
	return state;
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
				setfillcolor(grid_color); //���������ɫ

				// ����ޱ߿���Σ�ע���ά��������Ͻǿ�ʼ�������ĸ������ֱ�Ϊ���Ͻ�xy�����½�xy
				solidrectangle(grid_pos[i][j].x, grid_pos[i][j].y, grid_pos[i][j].x + GRID_WIDTH, grid_pos[i][j].y + GRID_WIDTH);
				// ����ÿһ�������
				if (nums[i][j]) {
					char number[5]; // ���������ַ���
					settextcolor(RGB(255, 255, 255));
					if (nums[i][j] == 2 || nums[i][j] == 4) settextcolor(RGB(119, 110, 101));  // ��2��4��һ����ɫ
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
	outtextxy(WINDOWS_WIDTH / 2 + 50+(150-textwidth("HighestScores")) / 2, WINDOWS_HEIGHT - 200 + textheight("HighestScores") / 2, "HighestScores");
	char score_h[100];
	if (historyScore() > score) sprintf(score_h, "%d", historyScore());
	else sprintf(score_h, "%d", score);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, "����");
	outtextxy(WINDOWS_WIDTH / 2 + 50 + (150 - textwidth(score_h)) / 2, WINDOWS_HEIGHT - 170 + textheight(score_h) / 2, score_h);

	// ���õ�ǰ������ʾ
	char score_c[100];
	sprintf(score_c, "%d", score);
	solidrectangle(WINDOWS_WIDTH / 2 -200, WINDOWS_HEIGHT - 200, WINDOWS_WIDTH / 2 - 50, WINDOWS_HEIGHT - 120);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, "����");
	outtextxy(WINDOWS_WIDTH / 2 - 200 + (150 - textwidth("Scores")) / 2, WINDOWS_HEIGHT - 210 + textheight("Scores") / 2, "Scores");
	outtextxy(WINDOWS_WIDTH / 2 - 200 + (150 - textwidth(score_c)) / 2, WINDOWS_HEIGHT - 170 + textheight(score_c) / 2, score_c);
	
	// ������ʾ����
	const char TIPE[30] = "Press \"Esc\" to quit game.";
	outtextxy((WINDOWS_WIDTH-textwidth(TIPE)) / 2, WINDOWS_HEIGHT -120 + textheight(TIPE) / 2, TIPE);
	EndBatchDraw(); // ����������ͼ
}
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
// ���������ƶ�
void stdControl() {
	ExMessage msg;
	int processed = false; // ����Ƿ��Ѿ��ƶ���
	
	while (peekmessage(&msg, EX_KEY)) {
		// �Ӵ����ж�ȡ������Ϣ(wasd��WASD��������˳�)
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
	
	//Sleep(100); // ��ֹ�����������
}

// ��Ϸ���չʾ
void showResult(const char* message) {
	// �������δ���
	setfillcolor(RGB(0,0,0)); //���������ɫ
	int x1 = (WINDOWS_WIDTH - RECWIDTH) / 2;
	int y1 = (WINDOWS_HEIGHT - RECHEIGHT) / 2-50;
	int x2 = x1 + RECWIDTH;
	int y2 = y1 + RECHEIGHT-50;
	solidrectangle(x1-5,y1-5,x2+5,y2+5); // �����ʾ�������
	setfillcolor(BK);
	solidrectangle(x1, y1, x2, y2);


	// ���������Ϣ
	settextcolor(RGB(255, 0, 0)); // ������ɫ
	settextstyle(50, 0, "Times New Roman"); // �����ֺ�������
	setbkmode(TRANSPARENT); // �������ֱ���ɫΪ͸��
	outtextxy(x1 + (RECWIDTH - textwidth(message))/2, y1 - 100 + RECHEIGHT/2- textheight(message), message);
	settextcolor(RGB(0, 0, 0)); // ������ɫ
	settextstyle(25, 0, "Times New Roman"); // �����ֺ�������
	const char TIP1[100] = "Press \"R\" or \"r\" to restart.";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP1)) / 2, y1 - 50 + RECHEIGHT / 2 - textheight(TIP1), TIP1);
	const char TIP2[100] = "Press \"Esc\" to quit.";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP2)) / 2, y1 - 20 + RECHEIGHT / 2 - textheight(TIP2), TIP2);
	const char TIP3[100] = "Press \"L\" or \"l\" to show the ranking.";
	outtextxy(x1 + (RECWIDTH - textwidth(TIP3)) / 2, y1 +10 + RECHEIGHT / 2 - textheight(TIP3), TIP3);

	// �������
	settextstyle(30, 0, "Times New Roman"); // �����ֺ�������
	char score1[100];
	sprintf(score1, "Scores: %d", score);
	outtextxy(x1 + (RECWIDTH - textwidth(score1)) / 2, y1 + 50  + RECHEIGHT / 2 - textheight(score1), score1);

	char score_c[20];
	sprintf(score_c, "Highest Scores:%d", historyScore());
	//sprintf(score2, "Highest Scores: %d", historyScore());
	outtextxy(x1 + (RECWIDTH - textwidth(score_c)) / 2, y1 + 80 + RECHEIGHT / 2 - textheight(score_c), score_c);

	// �޸Ĳ��������
	insertRanking(cur_name, score); // ��������
	saveRankings(); // ���浽�ļ�
	
	
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
		printf("����������������");
		scanf("%s", cur_name);
	}
	while (1) {
		drawBoard();
		if (isGameWin()) {
			showResult("You Win!");
			while (!overInput()); // �ȴ��û�����
		}
		else if (isGameOver()) {
			showResult("Game Over");
			while (!overInput()); // �ȴ��û�����
		}
		else {
			stdControl();
			numSummon();
			// ÿ�ζ��ж�һ����߷���
			if (historyScore() < score) {
				score_f = fopen("score.txt", "w");
				fprintf(score_f, "%d", score); // д���µ���߷�
				fclose(score_f);
			}
			saveGameState();
			
		}
	}
	
	freeChain(); // �ͷ������ڴ�

	//system("pause"); // ʹ���ڱ��ֿ���
	return 0;
}