#include <stdio.h>
#include <graphics.h> // EasyXͼ�ο�ͷ�ļ�


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
}grid_position[SIZE][SIZE];
// ��¼��������
int nums[SIZE][SIZE];

// ��ʼ����Ϸ����
void initWindows() {
	initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT, EX_SHOWCONSOLE); // ��ʼ������
	setbkcolor(BK); // ���ô��ڱ���ɫ
	cleardevice(); // �ñ���ɫ�������

	// Ԥ����ÿ�����ӵ�����
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			grid_position[i][j].x = i * GRID_WIDTH + (i + 1) * INTERVALS;
			grid_position[i][j].y = j * GRID_WIDTH + (j + 1) * INTERVALS;
		}
	}
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
			case 0: grid_color = ZERO;
			case 2: grid_color = TWO;

			default:
				break;
			}
			setfillcolor(ZERO); //���������ɫ
			// ����ޱ߿���Σ�ע���ά��������Ͻǿ�ʼ�������ĸ������ֱ�Ϊ���Ͻ�xy�����½�xy
			solidrectangle(grid_position[i][j].x,grid_position[i][j].y,grid_position[i][j].x+GRID_WIDTH,grid_position[i][j].y+GRID_WIDTH);

		}
	}

	
}
int main(){
	initWindows();
	drawBoard();

	system("pause"); // ʹ���ڱ��ֿ���
	return 0;
}