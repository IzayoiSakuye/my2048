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
}


// ������Ϸ����
void drawBoard() {
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
				settextstyle(40, 0, "Times New Roman"); // �����ֺ�������
				setbkmode(TRANSPARENT); // �������ֱ���ɫΪ͸��
				sprintf(number, "%d", nums[i][j]); // �������е�������Ϊ�ַ����洢
				int text_width = GRID_WIDTH/2 - textwidth(number)/2; // �����ַ����
				int text_height = GRID_WIDTH / 2 - textheight(number) / 2; // �����ַ��߶�
				outtextxy(grid_pos[i][j].x + text_width, grid_pos[i][j].y + text_height, number); // ������ʾ����
			}

		}
	}

	
}

// ����
void up() {
	for (int i = 0; i < SIZE; i++) {
		int tep = 0;
		for (int j = 1; j < SIZE; j++) {
			if (nums[j][i] != 0) {
				if (nums[tep][i] == 0) {
					nums[tep][i] = nums[j][i];
					nums[j][i] = 0;
				}
				else if (nums[tep][i] == nums[j][i]) {
					nums[tep][i] += nums[j][i];
					nums[j][i] = 0;
				}
				else {
					nums[tep + 1][i] = nums[j][i];
					if (tep + 1 != j) {
						nums[j][i] = 0;
					}
				}
			}
			tep++;
		}
	}
	printf("up");

}
// ����
void down() {
	printf("down");
}
// ����
void left() {
	printf("left");
}

// ����
void right() {
	printf("right");
}
// ���������ƶ�
void stdControl() {
	char key = _getch(); // ��ȡ��������
	// ���������жϲ���
	switch (key)
	{
		case 'w':
		case 'W':
		case 72:
			up();
			
			break;
		case 'a':
		case 'A':
		case 75:
			left();
			

			break;
		case 's':
		case 'S':
		case 80:
			down();
			

			break;
		case 'd':
		case 'D':
		case 77:
			right();
			

			break;
	default:
		break;
	}

}
int main(){
	initWindows();
	while (1) {
		drawBoard();
		stdControl();
	}
	

	system("pause"); // ʹ���ڱ��ֿ���
	return 0;
}