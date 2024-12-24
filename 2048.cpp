#include <stdio.h>
#include <graphics.h> // EasyXͼ�ο�ͷ�ļ�
#include <time.h> // ���ڳ�ʼ�������������
#include <conio.h> // ���ڼ��̲���

// �궨�崰�ڳߴ�����
#define GRID_WIDTH 100 // ���ӿ��
#define INTERVALS 15 // �����С
#define SIZE 4 // ��������
#define WINDOWS_WIDTH (GRID_WIDTH*SIZE+INTERVALS*(SIZE+1)) // ���ڿ�
#define WINDOWS_HEIGHT (GRID_WIDTH*SIZE+INTERVALS*(SIZE+1))+120 //���ڸ�

// ��ö���������ø��������Լ�������ɫ
enum color {
	BK = RGB(255, 224, 172), // ����ɫ
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
// ��¼����
struct pos {
	long long x;
	long long y;
}grid_pos[SIZE][SIZE];
// ��¼��������
int nums[SIZE][SIZE];
// �ж��Ƿ������������
bool flag = false;

// �����������
int randomNums() {
	time_t t1; // ��ʱ����Ϊsrand��seed����ֹÿ���������һ��
	srand((unsigned)time(&t1)); // ��ʼ�������������
	if (rand() % 8 == 1) return 4;
	else return 2;
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

// �ж��Ƿ�ʤ��
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

// �ж��Ƿ����
bool isGameOver() {
	for (int i = 1; i < SIZE-1; i++) {
		for (int j = 1; j < SIZE-1; j++) {
			// ����пո��ӣ���Ϸδ����
			if (nums[i][j] == 0) {
				return false;
			}
			// ��鵱ǰ�������ĸ������Ƿ����
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
			solidrectangle(grid_pos[i][j].x,grid_pos[i][j].y,grid_pos[i][j].x+GRID_WIDTH,grid_pos[i][j].y+GRID_WIDTH);
			// ����ÿһ�������
			if (nums[i][j]) {
				char number[5]; // ���������ַ���
				if (nums[i][j] == 2 || nums[i][j] == 4) settextcolor(RGB(119, 110, 101));  // ��2��4��һ����ɫ
				settextstyle(40, 0, "Times New Roman"); // �����ֺ�������
				setbkmode(TRANSPARENT); // �������ֱ���ɫΪ͸��
				sprintf(number, "%d", nums[i][j]); // �������е�������Ϊ�ַ����洢
				int text_width = GRID_WIDTH/2 - textwidth(number)/2; // �����ַ����
				int text_height = GRID_WIDTH / 2 - textheight(number) / 2; // �����ַ��߶�
				outtextxy(grid_pos[i][j].x + text_width, grid_pos[i][j].y + text_height, number); // ������ʾ����
			}

		}
	}
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
// ���������ƶ�
void stdControl() {
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY)) {
		// �Ӵ����ж�ȡ������Ϣ(wasd��WASD�����)
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
	
	//Sleep(100); // ��ֹ�����������
}

// ��Ϸ���չʾ
void showResult(const char* message) {
	settextcolor(RGB(255, 0, 0)); // ������ɫ
	settextstyle(50, 0, "Times New Roman"); // �����ֺ�������
	setbkmode(TRANSPARENT); // �������ֱ���ɫΪ͸��
	outtextxy(WINDOWS_WIDTH / 2 - textwidth(message) / 2, WINDOWS_HEIGHT / 2 - textheight(message) / 2 - 40, message);
	settextcolor(RGB(0, 0, 0)); // ������ɫ
	settextstyle(25, 0, "Times New Roman"); // �����ֺ�������
	const char TIP[100] = "Press \"R\" or \"r\" to restart, press \"Esc\" to quit.";
	outtextxy(WINDOWS_WIDTH / 2 - textwidth(TIP) / 2, WINDOWS_HEIGHT / 2 -textheight(TIP)/2, TIP);
}

// ��Ϸ������Ĳ����ж�
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
			while (!overInput()); // �ȴ��û�����
		}
		else if (isGameOver()) {
			showResult("Game Over");
			while (!overInput()); // �ȴ��û�����
		}
		else {
			stdControl();
			numSummon();
		}
	}
	

	//system("pause"); // ʹ���ڱ��ֿ���
	return 0;
}